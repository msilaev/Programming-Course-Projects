// Databricks notebook source
// MAGIC %md
// MAGIC # COMP.CS.320 Data-Intensive Programming, Exercise 3
// MAGIC
// MAGIC This exercise has three parts:
// MAGIC
// MAGIC - tasks 1-3 concern data queries for static data
// MAGIC - tasks 4-5 are examples of using typed Dataset instead of DataFrame
// MAGIC - tasks 6-8 concern the same data query as in the first two tasks but handled as streaming data
// MAGIC
// MAGIC The tasks can be done in either Scala or Python. This is the **Scala** version, switch to the Python version if you want to do the tasks in Python.
// MAGIC
// MAGIC Each task has its own cell for the code. Add your solutions to the cells. You are free to add more cells if you feel it is necessary. There are cells with example outputs or test code following most of the tasks.
// MAGIC
// MAGIC Don't forget to submit your solutions to Moodle.
// MAGIC

// COMMAND ----------

// some imports that might be required in the tasks

import org.apache.spark.sql.DataFrame
import org.apache.spark.sql.Dataset
import org.apache.spark.sql.functions._
import org.apache.spark.sql.types.IntegerType

import spark.implicits._


// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 1 - Create a DataFrame for retailer data
// MAGIC
// MAGIC In the [Shared container](https://portal.azure.com/#view/Microsoft_Azure_Storage/ContainerMenuBlade/~/overview/storageAccountId/%2Fsubscriptions%2Fe0c78478-e7f8-429c-a25f-015eae9f54bb%2FresourceGroups%2Ftuni-cs320-f2023-rg%2Fproviders%2FMicrosoft.Storage%2FstorageAccounts%2Ftunics320f2023gen2/path/shared/etag/%220x8DBB0695B02FFFE%22/defaultEncryptionScope/%24account-encryption-key/denyEncryptionScopeOverride~/false/defaultId//publicAccessVal/None) in the `exercises/ex3` folder is file `sales_data_sample.csv` that contains retailer sales data in CSV format.
// MAGIC The direct address for the data file is: `abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex3/sales_data_sample.csv`
// MAGIC
// MAGIC Read the data from the CSV file into DataFrame called retailerDataFrame. Let Spark infer the schema for the data. Note, that this CSV file uses semicolons (`;`) as the column separator instead of the default commas (`,`).
// MAGIC
// MAGIC Print out the schema, the resulting DataFrame should have 24 columns. The data contains information about the item price and the number of items ordered for each day.
// MAGIC

// COMMAND ----------

val fname = "abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex3/sales_data_sample.csv"
val retailerDataFrame: DataFrame = spark.read
.option("header" , "true")
.option("sep", ";")
.option("inferSchema", "true")
.csv(fname)    

retailerDataFrame.printSchema()


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 1 (only the first few lines):
// MAGIC
// MAGIC ```text
// MAGIC root
// MAGIC  |-- ORDERNUMBER: integer (nullable = true)
// MAGIC  |-- QUANTITYORDERED: integer (nullable = true)
// MAGIC  |-- PRICEEACH: double (nullable = true)
// MAGIC  |-- ORDERLINENUMBER: integer (nullable = true)
// MAGIC  |-- ORDERDATE: date (nullable = true)
// MAGIC  ...
// MAGIC  ```

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 2 - The best selling days
// MAGIC
// MAGIC Find the best **12** selling days using the retailer data frame from task 1. That is the days for which `QUANTITYORDERED * PRICEEACH` gets the highest values.
// MAGIC

// COMMAND ----------

val best12DaysDF = retailerDataFrame.withColumn("total_sales", col("QUANTITYORDERED") *col("PRICEEACH")).groupBy("ORDERDATE").agg(sum("total_sales").alias("total_sales")).orderBy(desc("total_sales")).limit(12)

best12DaysDF.show()

// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 2:
// MAGIC
// MAGIC ```text
// MAGIC +----------+------------------+
// MAGIC | ORDERDATE|       total_sales|
// MAGIC +----------+------------------+
// MAGIC |2004-11-24|115033.48000000003|
// MAGIC |2003-11-14|109509.87999999999|
// MAGIC |2003-11-12|          90218.58|
// MAGIC |2003-12-02| 87445.18000000001|
// MAGIC |2004-10-16|          86320.39|
// MAGIC |2003-11-06|          84731.32|
// MAGIC |2004-11-17|          82125.62|
// MAGIC |2004-11-04|          80807.93|
// MAGIC |2004-08-20| 80247.84000000001|
// MAGIC |2004-11-05| 78324.73000000001|
// MAGIC |2003-11-20|          76973.93|
// MAGIC |2004-12-10|          76380.08|
// MAGIC +----------+------------------+
// MAGIC ```

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 3 - The products with the most sale value
// MAGIC
// MAGIC Find the product codes for the **8** products that have the most total sale value in year 2003.
// MAGIC
// MAGIC **Note**, in this task (and only in this task) all sales done in **January** should be counted **twice**.
// MAGIC
// MAGIC Hint: use the MONTH_ID and YEAR_ID columns to recognize the month and year of each sale.
// MAGIC

// COMMAND ----------

val productDF: DataFrame = retailerDataFrame.where(col("YEAR_ID") === 2003).withColumn("total_sales", col("QUANTITYORDERED") *col("PRICEEACH")).withColumn("total_sales", when(col("MONTH_ID") === 1, col("total_sales") * 2).otherwise(col("total_sales"))).groupBy("PRODUCTCODE").agg( sum(col("total_sales")).alias("total_sales")).orderBy(desc("total_sales")).limit(8)

productDF.show()

// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 3:
// MAGIC
// MAGIC ```text
// MAGIC +-----------+------------------+
// MAGIC |PRODUCTCODE|       total_sales|
// MAGIC +-----------+------------------+
// MAGIC |   S18_3232|           69500.0|
// MAGIC |   S18_2319|           45600.0|
// MAGIC |   S18_4600|           44400.0|
// MAGIC |   S50_1392|39813.920000000006|
// MAGIC |   S18_1342|39661.149999999994|
// MAGIC |   S12_4473|          39084.36|
// MAGIC |   S24_3856|           38900.0|
// MAGIC |   S24_2300|           38800.0|
// MAGIC +-----------+------------------+
// MAGIC ```

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 4 - Dataset 1
// MAGIC
// MAGIC The classes that takes a type parameter are known to be Generic classes in Scala. Dataset is an example of a generic class. Actually, DataFrame is a type alias for Dataset[Row], where Row is the type parameter (Row being general object that can represent any row in a Spark data frame).
// MAGIC
// MAGIC Declare your own case class Sales with two members: `year` and `euros`, with both being of integer types.
// MAGIC
// MAGIC Then instantiate an object of type `Dataset[Sales]` with data from the variable `salesList` and query for the sales on 2017 and the year with the highest amount of sales.
// MAGIC

// COMMAND ----------

package dip.ex3   // Create a package.

// Create the Sales case class in this cell with the package definition
case class Sales(val year: Int, val euros: Int) {}



// COMMAND ----------

import dip.ex3.Sales

val salesList = List(Sales(2015, 325), Sales(2016, 100), Sales(2017, 15), Sales(2018, 900),
                     Sales(2019, 50), Sales(2020, 750), Sales(2021, 950), Sales(2022, 400))

val salesDS: Dataset[Sales] = salesList.toDS()
val sales2017: Sales = salesDS.where ($"year" === 2017).first()

println(sales2017.euros)

// COMMAND ----------

// import the defined case class so that it can be used inside Spark queries
import dip.ex3.Sales

val salesList = List(Sales(2015, 325), Sales(2016, 100), Sales(2017, 15), Sales(2018, 900),
                     Sales(2019, 50), Sales(2020, 750), Sales(2021, 950), Sales(2022, 400))

val salesDS: Dataset[Sales] = salesList.toDS()
val sales2017: Sales = salesDS.where ($"year" === 2017).first()   
println(s"Sales for 2017 is ${sales2017.euros}")

val maximumSales = salesDS.orderBy(desc("euros")).first()
println(s"Maximum sales: year = ${maximumSales.year}, euros = ${maximumSales.euros}")

println("======================================================")


// COMMAND ----------

// MAGIC %md
// MAGIC
// MAGIC Example output for task 4:
// MAGIC ```text
// MAGIC Sales for 2017 is 15
// MAGIC Maximum sales: year = 2021, euros = 950
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 5 - Dataset 2
// MAGIC
// MAGIC Continuation from task 4.
// MAGIC The new sales list `multiSalesList` contains sales information from multiple sources and thus can contain multiple values for each year. The total sales in euros for a year is the sum of all the individual values for that year.
// MAGIC
// MAGIC Query for the sales on 2016 and the year with the highest amount of sales in this case.
// MAGIC

// COMMAND ----------

import dip.ex3.Sales

val multiSalesList: List[Sales] = List(
    Sales(2015, 325), Sales(2016, 100), Sales(2017, 15), Sales(2018, 900),
    Sales(2019, 50), Sales(2020, 750), Sales(2021, 950), Sales(2022, 400),
    Sales(2016, 250), Sales(2017, 600), Sales(2019, 75), Sales(2016, 5),
    Sales(2018, 127), Sales(2019, 200), Sales(2020, 225), Sales(2016, 350)
)

val multiSalesDS: Dataset[Sales] =  multiSalesList.toDS

val multiSales2016 = multiSalesDS.groupBy("year").agg(sum("euros").cast("Int").alias("euros")).where($"year" === 2016).as[Sales].first();
val maximumMultiSales: Sales = multiSalesDS.groupBy("year").agg(sum("euros").cast("Int").alias("euros")).orderBy($"euros".desc).as[Sales].first();

println(s"Total sales for 2016 is ${multiSales2016.euros}")
println(s"Maximum total sales: year = ${maximumMultiSales.year}, euros = ${maximumMultiSales.euros}")

println("===========================================================")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 5:
// MAGIC
// MAGIC ```text
// MAGIC ...
// MAGIC Total sales for 2016 is 705
// MAGIC Maximum total sales: year = 2018, euros = 1027
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 6 - Streaming data from retail data
// MAGIC
// MAGIC Create a streaming data frame for similar retailer data as was used in tasks 1-3.
// MAGIC
// MAGIC In this exercise, streaming data is simulated by copying CSV files in 10 second intervals from a source folder to a target folder. The script for doing the file copying is given in task 8 and should not be run before the tasks 6 and 7 have been done.
// MAGIC
// MAGIC The target folder will be defined to be in the students storage as `ex3/YOUR_EX3_FOLDER` with the value for YOUR_EX3_FOLDER chosen by you.
// MAGIC
// MAGIC Hint: Spark cannot infer the schema of streaming data, so you have to give it explicitly. You can assume that the streaming data will have the same format as the static data used in tasks 1-3.
// MAGIC
// MAGIC Finally, note that you cannot really test this task before you have also done the tasks 7 and 8.
// MAGIC

// COMMAND ----------

// Put your own unique folder name to the variable (use only letters, numbers, and underscores):
val my_ex3_folder: String = "mslv"

// register the folder name to Spark context, so that it can be used with Python script in task 8
spark.conf.set("my_ex3_folder", my_ex3_folder)

val targetFiles: String = s"abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/${my_ex3_folder}/*"
val sourseFiles: String = s"abfss://students@tunics320f2023gen2.dfs.core.windows.net/Shared/ex3/student/*"

val retailerStreamingDF: DataFrame = spark.readStream.schema(retailerDataFrame.schema).option("sep",";").csv(targetFiles)




// COMMAND ----------

// MAGIC %md
// MAGIC Note that you cannot really test this task before you have also done the tasks 7 and 8, i.e. there is no checkable output from this task.
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 7 - The best selling days for the streaming data
// MAGIC
// MAGIC Find the best selling days using the streaming data frame from task 6.
// MAGIC
// MAGIC Note that in this task with the streaming data you don't need to limit the content only to the best 12 selling days like was done in task 2.
// MAGIC

// COMMAND ----------

val bestDaysDFStreaming: DataFrame = retailerStreamingDF.withColumn("total_sales", col("QUANTITYORDERED") *col("PRICEEACH")).groupBy("ORDERDATE").agg(sum("total_sales").alias("total_sales")).orderBy(desc("total_sales"))


// COMMAND ----------

// MAGIC %md
// MAGIC Note that you cannot really test this task before you have also done the tasks 6 and 8, i.e. there is no checkable output from this task.
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 8 - Test your streaming data solution
// MAGIC
// MAGIC Test your streaming data solution by following the output from the display command in the next cell while the provided Python script in the following cell is running.
// MAGIC
// MAGIC - The Python script copies files one by one (in ten second intervals) from the [shared container](https://portal.azure.com/#view/Microsoft_Azure_Storage/ContainerMenuBlade/~/overview/storageAccountId/%2Fsubscriptions%2Fe0c78478-e7f8-429c-a25f-015eae9f54bb%2FresourceGroups%2Ftuni-cs320-f2023-rg%2Fproviders%2FMicrosoft.Storage%2FstorageAccounts%2Ftunics320f2023gen2/path/shared/etag/%220x8DBB0695B02FFFE%22/defaultEncryptionScope/%24account-encryption-key/denyEncryptionScopeOverride~/false/defaultId//publicAccessVal/None) folder `exercises/ex3` to the [student container](https://portal.azure.com/#view/Microsoft_Azure_Storage/ContainerMenuBlade/~/overview/storageAccountId/%2Fsubscriptions%2Fe0c78478-e7f8-429c-a25f-015eae9f54bb%2FresourceGroups%2Ftuni-cs320-f2023-rg%2Fproviders%2FMicrosoft.Storage%2FstorageAccounts%2Ftunics320f2023gen2/path/students/etag/%220x8DBB0695B02FFFE%22/defaultEncryptionScope/%24account-encryption-key/denyEncryptionScopeOverride~/false/defaultId//publicAccessVal/None) folder `/ex3/MY_EX3_FOLDER` where `MY_EX3_FOLDER` is the folder name you chose in task 6.
// MAGIC - To properly run the streaming data test, the target folder should be either empty or it should not exist at all. If there are files in the target folder, those are read immediately and the streaming data demostration will not work as intended. The script does try to remove all copied files from the target folder at the end, but that only happens if the script is not interrupted.
// MAGIC
// MAGIC To gain points from this task, answer the questions in the final cell of the notebook.
// MAGIC

// COMMAND ----------

// in Databricks the display function can be used to display also a streaming data frame
// when developing outside this kind of environment, you need to create a query that could then be used to monitor the state of the data frame
// Usually, when using streaming data the results are directed to some data storage, not just displayed like in this exercise.

// There should be no need to edit anything in this cell!
print(s"Starting stream myQuery_${my_ex3_folder}")
display(bestDaysDFStreaming.limit(6), streamName = s"myQuery_${my_ex3_folder}")


// COMMAND ----------

// MAGIC %python
// MAGIC # There should be no need to edit anything in this cell, but you can try to adjust to time variables.
// MAGIC
// MAGIC import glob
// MAGIC import pathlib
// MAGIC import shutil
// MAGIC import time
// MAGIC
// MAGIC # fetch the folder name from Scala context (set in task 6)
// MAGIC my_ex3_folder: str = spark.conf.get("my_ex3_folder")
// MAGIC
// MAGIC initial_wait_time: float = 20.0
// MAGIC time_interval: float = 10.0
// MAGIC post_loop_wait_time: float = 20.0
// MAGIC
// MAGIC time.sleep(initial_wait_time)
// MAGIC input_file_list: list = dbutils.fs.ls("abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex3/streamingData")
// MAGIC for index, csv_file in enumerate(input_file_list, start=1):
// MAGIC     input_file_path = csv_file.path
// MAGIC     input_file = pathlib.Path(input_file_path).name
// MAGIC     output_file_path = f"abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/{my_ex3_folder}/{input_file}"
// MAGIC     dbutils.fs.cp(input_file_path, output_file_path)
// MAGIC     print(f"Copied file {input_file} ({index}/{len(input_file_list)}) to {output_file_path}")
// MAGIC     time.sleep(time_interval)
// MAGIC time.sleep(post_loop_wait_time)
// MAGIC
// MAGIC # stop updating the display for the streaming data frame
// MAGIC for stream in spark.streams.active:
// MAGIC     if stream.name == f"myQuery_{my_ex3_folder}":
// MAGIC         print(f"Stopping stream {stream.name}")
// MAGIC         spark.streams.get(stream.id).stop()
// MAGIC
// MAGIC # remove the copied files from the output folder
// MAGIC for copy_file in dbutils.fs.ls(f"abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/{my_ex3_folder}"):
// MAGIC     dbutils.fs.rm(copy_file.path)
// MAGIC print(f"Removed all copied files from abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/{my_ex3_folder}")
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC Example output from the Python script in task 8:
// MAGIC
// MAGIC ```text
// MAGIC Copied file xaa.csv (1/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xaa.csv
// MAGIC Copied file xab.csv (2/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xab.csv
// MAGIC Copied file xac.csv (3/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xac.csv
// MAGIC Copied file xad.csv (4/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xad.csv
// MAGIC Copied file xae.csv (5/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xae.csv
// MAGIC Copied file xaf.csv (6/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xaf.csv
// MAGIC Copied file xag.csv (7/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xag.csv
// MAGIC Copied file xah.csv (8/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xah.csv
// MAGIC Copied file xai.csv (9/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xai.csv
// MAGIC Copied file xaj.csv (10/10) to abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here/xaj.csv
// MAGIC Stopping stream myQuery_some_folder_here
// MAGIC Removed all copied files from abfss://students@tunics320f2023gen2.dfs.core.windows.net/ex3/some_folder_here
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC **Answer the questions to get the points from task 8.**
// MAGIC
// MAGIC How well did the streaming data example work for you?
// MAGIC
// MAGIC *Answer: It was necessary to launch streaming first and after that run the python script. I don't fully understand why and how this procedure works.*
// MAGIC
// MAGIC What was the final output for the streaming data for you?
// MAGIC
// MAGIC *The final output is the table with 6 rows and 2 columns "ORDERDATE" and "total_sales". This table coincided with the result (first 6 rows) of the entire dataframe processing in taks 2.*
// MAGIC
// MAGIC The data in the streaming tasks is the same as the earlier static data (just divided into multiple files).
// MAGIC Did the final output match the first six rows of the task 2 output?
// MAGIC
// MAGIC *I tried several times and the final output always concided with the first 6 rows of the task 2 output table.*
// MAGIC
// MAGIC If it did not, what do you think could be the reason?
// MAGIC
// MAGIC *We use "display" function which is claimed to have similar behaviour as a streaming quiery in the "Append" mode. Therefore the resulting final output is the same as for the processing the total dataframe in tast 2. Maybe in some cases the behaviour is different but I did not observe that.* 
// MAGIC
// MAGIC If you had problems, what were they?
// MAGIC And what do you think were the causes for those problems?
// MAGIC
// MAGIC *I did not notice any particular problems.*
// MAGIC
