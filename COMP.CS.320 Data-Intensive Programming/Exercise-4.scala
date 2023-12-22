// Databricks notebook source
// MAGIC %md
// MAGIC # COMP.CS.320 Data-Intensive Programming, Exercise 4
// MAGIC
// MAGIC This exercise is in two parts and contains tasks related to the use of Spark machine learning library and the low-level distributed datasets, RDDs. The tasks can be done in either Scala or Python. This is the **Scala** version, switch to the Python version if you want to do the tasks in Python.
// MAGIC
// MAGIC - Tasks 1-4 goes through an ML example by training a linear regression model to 2-dimensional data and then using the trained model.
// MAGIC - Tasks 5-7 provides some examples for the usage of RDDs.
// MAGIC
// MAGIC Each task has its own cell for the code. Add your solutions to the cells. You are free to add more cells if you feel it is necessary. There are cells with example outputs or test code following most of the tasks.
// MAGIC
// MAGIC Don't forget to submit your solutions to Moodle.

// COMMAND ----------

// some imports that might be required in the tasks

import org.apache.spark.ml.evaluation.RegressionEvaluator
import org.apache.spark.ml.feature.VectorAssembler
import org.apache.spark.ml.regression.LinearRegression
import org.apache.spark.ml.regression.LinearRegressionModel
import org.apache.spark.rdd.RDD
import org.apache.spark.sql.Row
import org.apache.spark.sql.DataFrame
import org.apache.spark.sql.functions._
import org.apache.spark.sql.types._


// COMMAND ----------

// the initial data for the linear regression tasks
val hugeSequenceOfXYData: Seq[Row] = Seq(
    Row(0.0, 0.0), Row(0.3, 0.5), Row(0.9, 0.8), Row(1.0, 0.8), Row(5.2, 5.8), Row(6.9, 7.2),
    Row(6.4, 6.8), Row(2.0, 2.2), Row(2.2, 2.4), Row(3.0, 3.7), Row(4.0, 4.3), Row(3.8, 4.0),
    Row(7.1, 9.0), Row(1.9, 1.9), Row(1.5, 1.4), Row(3.2, 3.9), Row(3.5, 4.1), Row(1.2, 1.1),
    Row(5.9, 5.7), Row(6.1, 6.7), Row(4.4, 4.6), Row(5.5, 5.2), Row(6.6, 7.2), Row(6.9, 7.9),
    Row(3.1, 4.0), Row(0.1, 0.2), Row(7.8, 8.8), Row(4.7, 5.0), Row(4.8, 5.0), Row(7.5, 8.2)
)
val dataRDD: RDD[Row] = spark.sparkContext.parallelize(hugeSequenceOfXYData)



//val myManualSchema = new StructType(Array(
//new StructField("X", DoubleType, true),
//new StructField("Y", DoubleType, true)))


// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 1 - Linear regression - Creating data frame
// MAGIC
// MAGIC ### Background
// MAGIC
// MAGIC Wikipedia defines: [Simple Linear Regression](https://en.wikipedia.org/wiki/Simple_linear_regression)
// MAGIC
// MAGIC In statistics, simple linear regression is a linear regression model with a single explanatory variable.
// MAGIC That is, it concerns two-dimensional sample points with one independent variable and one dependent variable
// MAGIC (conventionally, the x and y coordinates in a Cartesian coordinate system) and finds a linear function (a non-vertical straight line)
// MAGIC that, as accurately as possible, predicts the dependent variable values as a function of the independent variable.
// MAGIC The adjective simple refers to the fact that the outcome variable is related to a single predictor.
// MAGIC
// MAGIC You are given an `dataRDD` of Rows (the first element are the `x` and the second the `y` values). We are aiming at finding simple linear regression model
// MAGIC for the dataset using Spark ML library. I.e. find a function `f` so that `y = f(x)` (for the 2-dimensional case `f(x)=ax+b`).
// MAGIC
// MAGIC ### Task instructions
// MAGIC
// MAGIC Transform the given `dataRDD` to a DataFrame `dataDF`, with two columns `X` (of type Double) and `label` (of type Double).
// MAGIC (`label`used here because that is the default dependent variable name in Spark ML library)
// MAGIC
// MAGIC Then split the rows in the data frame into training and testing data frames.
// MAGIC

// COMMAND ----------

val MySchema = new StructType( Array ( 
    new StructField ("X", DoubleType, true), 
    new StructField ("label", DoubleType, true)  ) ) 

val dataDF: DataFrame = spark.createDataFrame(dataRDD, MySchema)

// Split the data into training and testing datasets (roughly 80% for training, 20% for testing)
val trainTestArray: Array[DataFrame] = dataDF.randomSplit( Array(0.8, 0.2), seed = 123 )

val trainingDF: DataFrame = trainTestArray(0)
val testDF: DataFrame = trainTestArray(1)

println(s"Training set size: ${trainingDF.count()}")
println(s"Test set size: ${testDF.count()}")
println("Training set (showing only the first 6 points):")
trainingDF.show(6)

// register the data to views to be used in a plot at the end of task 3
trainingDF.createOrReplaceTempView("training_data")
testDF.createOrReplaceTempView("test_data")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 1 (the data splitting done by using seed value of 1 for data frames random splitting method, different values are possible even with the same seed):
// MAGIC
// MAGIC ```text
// MAGIC Training set size: 24
// MAGIC Test set size: 6
// MAGIC Training set (showing only the first 6 points):
// MAGIC +---+-----+
// MAGIC |  X|label|
// MAGIC +---+-----+
// MAGIC |0.0|  0.0|
// MAGIC |0.3|  0.5|
// MAGIC |0.9|  0.8|
// MAGIC |1.0|  0.8|
// MAGIC |6.4|  6.8|
// MAGIC |6.9|  7.2|
// MAGIC +---+-----+
// MAGIC only showing top 6 rows
// MAGIC ```
// MAGIC
// MAGIC Your output does not have to match this exactly, not even with the sizes of the training and test sets.
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 2 - Linear regression - Training the model
// MAGIC
// MAGIC To be able to use the ML algorithms in Spark the input data must be given as vectors in one column. To make it easy to transform the input data into this vector format, Spark offers VectorAssembler objects.
// MAGIC
// MAGIC Create a `VectorAssembler` for mapping input column `X` to `features`column and apply it to training data frame, `trainingDF,` in order to create assembled training data frame.
// MAGIC
// MAGIC Then create a `LinearRegression` object use it and the assembled training data frame to train a linear regression model.
// MAGIC

// COMMAND ----------

val vectorAssembler: VectorAssembler = new VectorAssembler().setInputCols(Array("X")).setOutputCol("features")

val assembledTrainingDF: DataFrame = vectorAssembler.transform(trainingDF)
assembledTrainingDF.show(6)


// COMMAND ----------

val lr: LinearRegression = new LinearRegression().setFeaturesCol("features").setPredictionCol("prediction").setLabelCol("label")

// you can print explanations for all the parameters that can be used for linear regression by uncommenting the following:
 println(lr.explainParams())

// COMMAND ----------

val lrModel: LinearRegressionModel =  lr.fit(assembledTrainingDF)
lrModel.summary.predictions.show(6)

// COMMAND ----------

// MAGIC %md
// MAGIC Example outputs for task 2:
// MAGIC
// MAGIC ```text
// MAGIC +---+-----+--------+
// MAGIC |  X|label|features|
// MAGIC +---+-----+--------+
// MAGIC |0.0|  0.0|   [0.0]|
// MAGIC |0.3|  0.5|   [0.3]|
// MAGIC |0.9|  0.8|   [0.9]|
// MAGIC |1.0|  0.8|   [1.0]|
// MAGIC |6.4|  6.8|   [6.4]|
// MAGIC |6.9|  7.2|   [6.9]|
// MAGIC +---+-----+--------+
// MAGIC only showing top 6 rows
// MAGIC ```
// MAGIC
// MAGIC and
// MAGIC
// MAGIC ```text
// MAGIC +---+-----+--------+--------------------+
// MAGIC |  X|label|features|          prediction|
// MAGIC +---+-----+--------+--------------------+
// MAGIC |0.0|  0.0|   [0.0]|-0.09967829559252293|
// MAGIC |0.3|  0.5|   [0.3]| 0.23199581909447237|
// MAGIC |0.9|  0.8|   [0.9]|  0.8953440484684629|
// MAGIC |1.0|  0.8|   [1.0]|  1.0059020866974615|
// MAGIC |6.4|  6.8|   [6.4]|  6.9760361510633775|
// MAGIC |6.9|  7.2|   [6.9]|    7.52882634220837|
// MAGIC +---+-----+--------+--------------------+
// MAGIC only showing top 6 rows
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 3 - Linear regression - Test the model
// MAGIC
// MAGIC Apply the trained linear regression model from task 2 to the test dataset.
// MAGIC
// MAGIC Then calculate the RMSE (root mean square error) for the test dataset predictions using `RegressionEvaluator` from Spark ML library.
// MAGIC
// MAGIC (The Python cell after the example output can be used to visualize the linear regression tasks)
// MAGIC

// COMMAND ----------

val assembledTestDF: DataFrame = vectorAssembler.transform(testDF)

val testPredictions: DataFrame = lrModel.transform(assembledTestDF)
testPredictions.show()

val testEvaluator: RegressionEvaluator = new RegressionEvaluator().setLabelCol("label").setPredictionCol("prediction").setMetricName("rmse")

// you can print explanations for all the parameters that can be used for the regression evaluator by uncommenting the following:
// println(testEvaluator.explainParams())

val testError: Double = testEvaluator.evaluate(testPredictions)
println(s"The RMSE for the model is ${testError}")
println("============================================")

// register the predictions to a view for the plot in the next cell
testPredictions.createOrReplaceTempView("test_predictions")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 3:
// MAGIC
// MAGIC ```text
// MAGIC +---+-----+--------+--------------------+
// MAGIC |  X|label|features|          prediction|
// MAGIC +---+-----+--------+--------------------+
// MAGIC |5.2|  5.8|   [5.2]|   5.649339692315396|
// MAGIC |3.0|  3.7|   [3.0]|    3.21706285127743|
// MAGIC |3.5|  4.1|   [3.5]|  3.7698530424224224|
// MAGIC |6.1|  6.7|   [6.1]|   6.644362036376381|
// MAGIC |0.1|  0.2|   [0.1]|0.010879742636475509|
// MAGIC |7.8|  8.8|   [7.8]|   8.523848686269353|
// MAGIC +---+-----+--------+--------------------+
// MAGIC
// MAGIC The RMSE for the model is 0.2828560983793194
// MAGIC ```
// MAGIC
// MAGIC 0 for RMSE would indicate perfect fit and the more deviations there are the larger RMSE will be.
// MAGIC
// MAGIC You can try different seed for dividing the data and different parameters for the linear regression to get different results.
// MAGIC

// COMMAND ----------

// MAGIC %python
// MAGIC # visualization of the linear regression exercise (can be done since there are only limited number of source data points)
// MAGIC from matplotlib import pyplot
// MAGIC
// MAGIC training_data = spark.sql("""SELECT * FROM training_data""").collect()
// MAGIC test_data = spark.sql("""SELECT * FROM test_data""").collect()
// MAGIC test_predictions = spark.sql("""SELECT * FROM test_predictions""").collect()
// MAGIC
// MAGIC x_values_training = [row[0] for row in training_data]
// MAGIC y_values_training = [row[1] for row in training_data]
// MAGIC x_values_test = [row[0] for row in test_data]
// MAGIC y_values_test = [row[1] for row in test_data]
// MAGIC x_predictions = [row[0] for row in test_predictions]
// MAGIC y_predictions = [row[3] for row in test_predictions]
// MAGIC
// MAGIC pyplot.xlabel("x")
// MAGIC pyplot.ylabel("y")
// MAGIC pyplot.xlim(min(x_values_training+x_values_test), max(x_values_training+x_values_test))
// MAGIC pyplot.ylim(min(y_values_training+y_values_test), max(y_values_training+y_values_test))
// MAGIC pyplot.plot(x_values_training, y_values_training, 'bo')
// MAGIC pyplot.plot(x_values_test, y_values_test, 'go')
// MAGIC pyplot.plot(x_predictions, y_predictions, 'r-')
// MAGIC pyplot.legend(["training data", "test data", "prediction line"])
// MAGIC pyplot.show()
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 4 - Linear regression - making predictions
// MAGIC
// MAGIC Use the trained `LinearRegressionModel` from task 2 to predict the `y` values for the following `x` values: `-1.5`, `2.5`, `8.5`, `31.4`
// MAGIC

// COMMAND ----------

val dF1 = List(-1.5, 2.5, 8.5, 31.4).toDF.withColumnRenamed("value", "X")
//val dF2 = dF1.withColumnRenamed("value", "X")
val assembledNewData1 = vectorAssembler.transform(dF1)
val newPredictions = lrModel.transform(assembledNewData1).drop("features")
newPredictions.show()


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 4:
// MAGIC
// MAGIC ```text
// MAGIC +----+-------------------+
// MAGIC |   X|         prediction|
// MAGIC +----+-------------------+
// MAGIC |-1.5|-1.7580488690274994|
// MAGIC | 2.5|  2.664272660132438|
// MAGIC | 8.5|  9.297754953872344|
// MAGIC |31.4|  34.61554570831298|
// MAGIC +----+-------------------+
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 5 - RDD - Loading text data
// MAGIC
// MAGIC There are three scientific articles in the [shared container](https://portal.azure.com/#view/Microsoft_Azure_Storage/ContainerMenuBlade/~/overview/storageAccountId/%2Fsubscriptions%2Fe0c78478-e7f8-429c-a25f-015eae9f54bb%2FresourceGroups%2Ftuni-cs320-f2023-rg%2Fproviders%2FMicrosoft.Storage%2FstorageAccounts%2Ftunics320f2023gen2/path/shared/etag/%220x8DBB0695B02FFFE%22/defaultEncryptionScope/%24account-encryption-key/denyEncryptionScopeOverride~/false/defaultId//publicAccessVal/None) at folder `exercises/ex4/`. The direct address to the folder is: `abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex4`.
// MAGIC
// MAGIC Load the text from the articles into a single RDD.
// MAGIC
// MAGIC Then count the total number of lines in the articles and print out the first 10 lines from the RDD.
// MAGIC

// COMMAND ----------

import org.apache.spark.sql.functions.input_file_name

val path = "abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex4"

val rddFromFile = spark.sparkContext.textFile(path)
  println(rddFromFile.getClass)

  println("##Get data Using collect")
  rddFromFile.collect().length


// COMMAND ----------

val path = "abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex4"
val articlesRdd: RDD[String] =  spark.sparkContext.textFile(path);
val numberOfLines: Long = articlesRdd.count();
println(s"numberOfLines: ${numberOfLines}")

val lines10: Array[String] = articlesRdd.take(10).toArray

println("=============================================================")
lines10.foreach(println)
println("=============================================================")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 5:
// MAGIC
// MAGIC ```text
// MAGIC numberOfLines: 2919
// MAGIC =============================================================
// MAGIC Journal of Universal Computer Science, vol. 7, no. 1 (2001), 3-18
// MAGIC submitted: 1/9/00, accepted: 13/10/00, appeared: 28/1/01  Springer Pub. Co.
// MAGIC
// MAGIC DisCo Toolset - The New Generation
// MAGIC Timo Aaltonen
// MAGIC (Tampere University of Technology, Finland
// MAGIC timo.aaltonen@tut.fi)
// MAGIC
// MAGIC Mika Katara
// MAGIC (Tampere University of Technology, Finland
// MAGIC =============================================================
// MAGIC ```
// MAGIC
// MAGIC If you load the articles in a different order, you might get different output for the first 10 lines here.
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 6 - RDD - Counting elements
// MAGIC
// MAGIC Using the `articlesRDD` defined in task 5 to count the total number of words in the articles. (you can assume that words are separated from each by white space characters, '` `')
// MAGIC
// MAGIC Then count the total number of non-white space characters in the articles.
// MAGIC

// COMMAND ----------

val numberOfWords: Long = articlesRdd.flatMap(l => l.split(" ")).filter(char => !char.trim.isEmpty).count()
println(s"numberOfWords: ${numberOfWords}")

val numberOfCharacters: Long =  articlesRdd.flatMap( l=> l.split("")).filter(char => !char.trim.isEmpty).count()
println(s"numberOfCharacters: ${numberOfCharacters}")
println("=============================================================")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 6:
// MAGIC
// MAGIC ```text
// MAGIC numberOfWords: 17333
// MAGIC numberOfCharacters: 95171
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 7 - RDD - Most common word
// MAGIC
// MAGIC For the final RDD task in this exercise find the answers to the following questions.
// MAGIC
// MAGIC - How many 6-letter words are there in the corpus?
// MAGIC - What is the most often appearing 6-letter word and how many times does it appear?
// MAGIC

// COMMAND ----------

val words6Count: Long =  articlesRdd.flatMap(l => l.split(" ")).filter(char => char.length ==6).count()
println(s"6-letter words: ${words6Count}")

val wordCount =articlesRdd.flatMap(l => l.split(" ")).filter(char => char.length ==6).map(s => (s, 1))
    .groupBy(p => p._1)
    .mapValues(v => v.size).sortBy(-_._2).take(1).head
    
val commonWord = wordCount._1
val commonWordCount = wordCount._2
    
// Display the word with the maximum count
println(s"Word: ${wordWithMaxCount._1}, Count: ${wordWithMaxCount._2}")

println(s"The most common 6-letter word is '${commonWord}' and it appears ${commonWordCount} times")
println("=============================================================")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 7:
// MAGIC
// MAGIC ```text
// MAGIC 6-letter words: 1188
// MAGIC The most common 6-letter word is 'action' and it appears 102 times
// MAGIC ```
// MAGIC
