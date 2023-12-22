// Databricks notebook source
// MAGIC %md
// MAGIC # COMP.CS.320 Data-Intensive Programming, Exercise 5
// MAGIC
// MAGIC This exercise demonstrates different file formats (CSV, Parquet, Delta) and some of the operations that can be used with them. The tasks can be done in either Scala or Python. This is the **Scala** version, switch to the Python version if you want to do the tasks in Python.
// MAGIC
// MAGIC - Tasks 1-5 concern reading and writing operations with CSV and Parquet
// MAGIC - Tasks 6-8 introduces the Delta format
// MAGIC
// MAGIC Each task has its own cell for the code. Add your solutions to the cells. You are free to add more cells if you feel it is necessary. There are cells with example outputs following most of the tasks.
// MAGIC
// MAGIC Don't forget to submit your solutions to Moodle.

// COMMAND ----------

// some imports that might be required in the tasks
import io.delta.tables.DeltaTable
import org.apache.spark.sql.DataFrame
import org.apache.spark.sql.functions._
import org.apache.spark.sql.types._


// COMMAND ----------

// some helper functions used in this exercise

def sizeInMB(sizeInBytes: Long): Double = (sizeInBytes.toDouble/1024/1024*100).round/100.0
def sizeInKB(filePath: String) = ((dbutils.fs.ls(filePath).map(_.size).sum).toDouble/1024*100).round/100.0

// print the files and their sizes from the target path
def printStorage(path: String): Unit = {
    def getStorageSize(currentPath: String): Long = {
        // using Databricks utilities to get the list of the files in the path
        val fileInformation = dbutils.fs.ls(currentPath)
        fileInformation.map(
            fileInfo => {
                if (fileInfo.isDir) {
                    getStorageSize(fileInfo.path)
                }
                else {
                    println(s"${sizeInMB(fileInfo.size)} MB --- ${fileInfo.path}")
                    fileInfo.size
                }
            }
        ).sum
    }

    val sizeInBytes = getStorageSize(path)
    println(s"Total size: ${sizeInMB(sizeInBytes)} MB")
}

// remove all files and folders from the target path
def cleanTargetFolder(path: String): Unit = {
    dbutils.fs.rm(path, true)
}

// Print column types in a nice format
def printColumnTypes(inputDF: DataFrame): Unit = {
    inputDF.dtypes.foreach({case (columnName, columnType) => println(s"${columnName}: ${columnType}")})
}

// Returns a limited sample of the input data frame
def getTestDF(inputDF: DataFrame, ids: Seq[String] = Seq("Z1", "Z2"), limitRows: Int = 2): DataFrame = {
    val origSample: DataFrame = inputDF
        .filter(!col("ID").contains("_"))
        .limit(limitRows)
    val extraSample: DataFrame = ids
        .map(id => inputDF
            .filter(col("ID").endsWith(id))
            .limit(limitRows)
        )
        .reduceLeft((df1, df2) => df1.union(df2))

    origSample.union(extraSample)
}


// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 1 - Read data in two formats
// MAGIC
// MAGIC In the [Shared container](https://portal.azure.com/#view/Microsoft_Azure_Storage/ContainerMenuBlade/~/overview/storageAccountId/%2Fsubscriptions%2Fe0c78478-e7f8-429c-a25f-015eae9f54bb%2FresourceGroups%2Ftuni-cs320-f2023-rg%2Fproviders%2FMicrosoft.Storage%2FstorageAccounts%2Ftunics320f2023gen2/path/shared/etag/%220x8DBB0695B02FFFE%22/defaultEncryptionScope/%24account-encryption-key/denyEncryptionScopeOverride~/false/defaultId//publicAccessVal/None) the `exercises/ex5` folder contains data about car accidents in USA. The same data is given in multiple formats and it is a subset of the dataset in [Kaggle](https://www.kaggle.com/datasets/sobhanmoosavi/us-accidents).
// MAGIC
// MAGIC In this task read the data into data frames from both CSV and Parquet source format. The CSV files use `|` as the column separator.
// MAGIC
// MAGIC Code for displaying the data and information about the source files is already included.
// MAGIC

// COMMAND ----------

// Fill in your name (or some other unique identifier). This will be used to identify your target folder for the exercise.
val student_name = "mslv"

val source_path = "abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex5/"
val target_path = s"abfss://students@tunics320f2023gen2.dfs.core.windows.net/${student_name}/ex5/"
val data_file = "accidents"


// COMMAND ----------

val source_csv_folder = source_path + s"${data_file}_csv"

// create and display the data from CSV source
//val df_csv = ???
val df_csv: DataFrame = spark.read
.option("header" , "true")
.option("sep", "|")
.option("inferSchema", "true")
.csv(source_csv_folder)    

//retailerDataFrame.printSchema()
df_csv.show(5)
//df_csv.printSchema


// COMMAND ----------

// Typically, a some more suitable file format would be used, like Parquet.
// With Parquet column format is stored in the file itself, so it does not need to be given.
val source_parquet_folder = source_path + s"${data_file}_parquet"

// create and display the data from Parquet source
//val df_parquet = ???

val df_parquet: DataFrame = spark.read
//.option("header" , "true")
//.option("inferSchema", "true")
.parquet(source_parquet_folder)    

//display(df_parquet)
df_parquet.show(5)

// COMMAND ----------

// print the list of files for the different file formats
println("CSV files:")
printStorage(source_csv_folder)

println("\nParquet files:")
printStorage(source_parquet_folder)

// The schemas for both data frames should be the same
println("\n== CSV types ==")
printColumnTypes(df_csv)
println("\n== Parquet types ==")
printColumnTypes(df_parquet)

// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 1 (the same output for both displays, only the first few lines shown here when using `show` to view the data frame):
// MAGIC
// MAGIC ```text
// MAGIC +---------+-------------------+-------------------+--------------------+---------+------+-----+-------------+
// MAGIC |       ID|         Start_Time|           End_Time|         Description|     City|County|State|Temperature_F|
// MAGIC +---------+-------------------+-------------------+--------------------+---------+------+-----+-------------+
// MAGIC |A-3558690|2016-01-14 20:18:33|2017-01-30 13:25:19|Closed at Fullert...|Whitehall|Lehigh|   PA|         31.0|
// MAGIC |A-3558700|2016-01-14 20:18:33|2017-01-30 13:34:02|Closed at Fullert...|Whitehall|Lehigh|   PA|         31.0|
// MAGIC |A-3558713|2016-01-14 20:18:33|2017-01-30 13:55:44|Closed at Fullert...|Whitehall|Lehigh|   PA|         31.0|
// MAGIC |A-3572241|2016-01-14 20:18:33|2017-02-17 23:22:00|Closed at Fullert...|Whitehall|Lehigh|   PA|         31.0|
// MAGIC |A-3572395|2016-01-14 20:18:33|2017-02-19 00:38:00|Closed at Fullert...|Whitehall|Lehigh|   PA|         31.0|
// MAGIC +---------+-------------------+-------------------+--------------------+---------+------+-----+-------------+
// MAGIC only showing top 5 rows
// MAGIC ```
// MAGIC
// MAGIC and
// MAGIC
// MAGIC ```text
// MAGIC CSV files:
// MAGIC 31.97 MB --- abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex5/accidents_csv/us_traffic_accidents.csv
// MAGIC Total size: 31.97 MB
// MAGIC
// MAGIC Parquet files:
// MAGIC 9.56 MB --- abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex5/accidents_parquet/us_traffic_accidents.parquet
// MAGIC Total size: 9.56 MB
// MAGIC
// MAGIC == CSV types ==
// MAGIC ID: StringType
// MAGIC Start_Time: TimestampType
// MAGIC End_Time: TimestampType
// MAGIC Description: StringType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: DoubleType
// MAGIC
// MAGIC == Parquet types ==
// MAGIC ID: StringType
// MAGIC Start_Time: TimestampType
// MAGIC End_Time: TimestampType
// MAGIC Description: StringType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: DoubleType
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 2 - Write the data to new storage
// MAGIC
// MAGIC Write the data from `df_csv` and `df_parquet` to the [student container](https://portal.azure.com/#view/Microsoft_Azure_Storage/ContainerMenuBlade/~/overview/storageAccountId/%2Fsubscriptions%2Fe0c78478-e7f8-429c-a25f-015eae9f54bb%2FresourceGroups%2Ftuni-cs320-f2023-rg%2Fproviders%2FMicrosoft.Storage%2FstorageAccounts%2Ftunics320f2023gen2/path/students/etag/%220x8DBB0695B02FFFE%22/defaultEncryptionScope/%24account-encryption-key/denyEncryptionScopeOverride~/false/defaultId//publicAccessVal/None) in both CSV and Parquet formats.
// MAGIC
// MAGIC Note, since the data in both data frames should be the same, you should be able to use either one as the source when writing it to a new folder (regardless of the target format).
// MAGIC

// COMMAND ----------

// remove all previously written files from the target folder first
cleanTargetFolder(target_path)

// the target paths for both CSV and Parquet
val target_file_csv = target_path + data_file + "_csv"
val target_file_parquet = target_path + data_file + "_parquet"

// write the data from task 1 in CSV format to the path given by target_file_csv
//???
df_csv.write
  .option("header", "true")  // Include headers in the output
  .option("sep", "|")
  .csv(target_file_csv)

// write the data from task 1 in Parquet format to the path given by target_file_parquet
//???
df_parquet.write
 // .option("header", "true")  // Include headers in the output
 // .option("sep", "|")
  .parquet(target_file_parquet)

// Check the written files:
printStorage(target_file_csv)
printStorage(target_file_parquet)

// Both with CSV and Parquet, the data can be divided into multiple files depending on how many workers were doing the writing.
// If a single file is needed, you can force the output into a single file with "coalesce(1)" before the write command
// This will make the writing less efficient, especially for larger datasets. (and is not needed in this exercise)
// There are some additional small metadata files (_SUCCESS, _committed, _started) that you can ignore in this exercise.
println("===== End of output =====")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 2 (note that the exact filenames will be different for you):
// MAGIC
// MAGIC ```text
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/_SUCCESS
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/_committed_3113733412107253362
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/_started_3113733412107253362
// MAGIC 8.99 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/part-00000-tid-3113733412107253362-c2b4319b-cb7f-499b-95d4-87feef106b82-8254-1-c000.csv
// MAGIC 8.99 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/part-00001-tid-3113733412107253362-c2b4319b-cb7f-499b-95d4-87feef106b82-8255-1-c000.csv
// MAGIC 8.99 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/part-00002-tid-3113733412107253362-c2b4319b-cb7f-499b-95d4-87feef106b82-8256-1-c000.csv
// MAGIC 4.99 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_csv/part-00003-tid-3113733412107253362-c2b4319b-cb7f-499b-95d4-87feef106b82-8257-1-c000.csv
// MAGIC Total size: 31.97 MB
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_parquet/_SUCCESS
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_parquet/_committed_6192047308321304174
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_parquet/_started_6192047308321304174
// MAGIC 9.56 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_parquet/part-00000-tid-6192047308321304174-c238e085-402c-4378-b03b-83a3fe26db1f-8258-1-c000.snappy.parquet
// MAGIC Total size: 9.56 MB
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 3 - Add new rows to storage
// MAGIC
// MAGIC First create a new data frame based on the task 1 data that contains the `42` latest incidents (based on the starting time) in the city of `New York`. The ids of the incidents of this data frame should have an added postfix `_Z1`, e.g., `A-3877306` should be replaced with `A-3877306_Z1`.
// MAGIC
// MAGIC Then append these new 42 rows to both the CSV storage and Parquet storage. I.e., write the new rows in append mode in CSV format to folder given by `target_file_csv` and in Parquet format to folder given by `target_file_parquet`.
// MAGIC
// MAGIC Finally, read the data from the storages again to check that the appending was successful.
// MAGIC

// COMMAND ----------

val more_rows_count = 42

// Create a data frame that holds some new rows that will be appended to the storage

val df_new = df_parquet.filter($"city" === "New York").orderBy(desc("Start_Time")).limit(more_rows_count).withColumn("ID_new", 
concat(col("ID"), lit("_Z1")) ).drop("ID").withColumnRenamed("ID_new", "ID")
//.select("ID", df_new.columns.filter(_ != "ID"): _*)

val df_new_rows = df_new.select("ID", df_new.columns.filter(_ != "ID"): _*)

df_new_rows.show(2)

df_new_rows.write.mode(SaveMode.Append).option("sep", "|").option("header" , "true").csv(target_file_csv)

df_new_rows.write.mode(SaveMode.Append).parquet(target_file_parquet)

// COMMAND ----------

// Read the merged data from the CSV files to check that the new rows have been stored
val df_new_csv = spark.read.option("header" , "true").option("sep", "|").option("inferSchema", "true").csv(target_file_csv)    

// Read the merged data from the Parquet files to check that the new rows have been stored
val df_new_parquet =  spark.read.option("header" , "true").parquet(target_file_parquet)    

val old_rows = df_parquet.count()
val new_rows = df_new_rows.count()
println(s"Old DF had ${old_rows} rows and we are adding ${new_rows} rows => we should have ${old_rows + new_rows} in the merged data.")

println(s"Old CSV DF had ${df_csv.count()} rows and new DF has ${df_new_csv.count()}.")
println(s"Old Parquet DF had ${df_parquet.count()} rows and new DF has ${df_new_parquet.count()}.")
println("===== End of output =====")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 3:
// MAGIC
// MAGIC ```text
// MAGIC +------------+-------------------+-------------------+--------------------+--------+--------+-----+-------------+
// MAGIC |          ID|         Start_Time|           End_Time|         Description|    City|  County|State|Temperature_F|
// MAGIC +------------+-------------------+-------------------+--------------------+--------+--------+-----+-------------+
// MAGIC |A-3877306_Z1|2023-02-16 20:48:16|2023-02-16 22:00:00|Closed road from ...|New York|New York|   NY|         58.0|
// MAGIC |A-4076884_Z1|2023-02-12 07:54:11|2023-02-12 10:05:18|Port Authority NY...|New York|New York|   NY|         40.0|
// MAGIC +------------+-------------------+-------------------+--------------------+--------+--------+-----+-------------+
// MAGIC only showing top 2 rows
// MAGIC ```
// MAGIC
// MAGIC and
// MAGIC
// MAGIC ```text
// MAGIC Old DF had 198082 rows and we are adding 42 rows => we should have 198124 in the merged data.
// MAGIC Old CSV DF had 198082 rows and new DF has 198124.
// MAGIC Old Parquet DF had 198082 rows and new DF has 198124.
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 4 - Append modified rows
// MAGIC
// MAGIC In the previous task, appending new rows was successful because the new data had the same schema as the original data.
// MAGIC
// MAGIC In this task we try to append data with a modified schema to the CSV and Parquet storages.
// MAGIC
// MAGIC First create a new data frame based on `df_new_rows` from task 3. The data frame should be modified in the following way:
// MAGIC
// MAGIC - The values in the `ID` column should have a postfix `_Z2` instead of `_Z1`. E.g., `A-3877306_Z1` should be replaced with `A-3877306_Z2`.
// MAGIC - A new column `AddedColumn1` should be added with values `"prefix-CITY"` where `CITY` is replaced by the city of the incident.
// MAGIC - A new column `AddedColumn2` should be added with a constant value `"New column"`.
// MAGIC - The column `Description` should be dropped.
// MAGIC
// MAGIC Then append these modified rows to both the CSV storage and Parquet storage.
// MAGIC

// COMMAND ----------

val df_modified = df_new_rows.withColumn("ID", regexp_replace(col("ID"), "_Z1$", "_Z2")).
withColumn("AddedColumn1", concat( lit("prefix-"),   col("city")) ).withColumn("AddedColumn2", lit("New column")).drop("Description")

df_modified.show(2)

df_modified.write.mode(SaveMode.Append).option("sep", "|").option("header" , "true").csv(target_file_csv)

df_modified.write.mode(SaveMode.Append).parquet(target_file_parquet)

// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 4:
// MAGIC
// MAGIC ```text
// MAGIC +------------+-------------------+-------------------+--------+--------+-----+-------------+---------------+------------+
// MAGIC |          ID|         Start_Time|           End_Time|    City|  County|State|Temperature_F|   AddedColumn1|AddedColumn2|
// MAGIC +------------+-------------------+-------------------+--------+--------+-----+-------------+---------------+------------+
// MAGIC |A-3877306_Z2|2023-02-16 20:48:16|2023-02-16 22:00:00|New York|New York|   NY|         58.0|prefix-New York|  New column|
// MAGIC |A-4076884_Z2|2023-02-12 07:54:11|2023-02-12 10:05:18|New York|New York|   NY|         40.0|prefix-New York|  New column|
// MAGIC +------------+-------------------+-------------------+--------+--------+-----+-------------+---------------+------------+
// MAGIC only showing top 2 rows
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 5 - Check the merged data
// MAGIC
// MAGIC In this task we check the contents of the CSV and Parquet storages after the two data append operations, the new rows with the same schema in task 3, and the new rows with a modified schema in task 4.
// MAGIC
// MAGIC Your task is to first write the code that loads the data from the storages again. And then run the given test code that shows the number of rows, columns, schema, and some sample rows.
// MAGIC
// MAGIC Finally, answer the questions in the final cell of this task.
// MAGIC

// COMMAND ----------

// CSV should have been broken in some way
// Read in the CSV data again from the CSV storage: target_file_csv
val modified_csv_df = spark.read.option("header", "true").option("inferSchema", "true").option("sep", "|").csv( target_file_csv)

println("== CSV storage:")
println(s"The number of rows should be correct: ${modified_csv_df.count()} (i.e., ${df_csv.count()}+2*${more_rows_count})")
println(s"However, the original data had ${df_csv.columns.length} columns, inserted data had ${df_modified.columns.length} columns. Afterwards we have ${modified_csv_df.columns.length} columns while we should have ${df_csv.columns.length + 2} distinct columns.")

getTestDF(modified_csv_df).show()

printColumnTypes(modified_csv_df)


// COMMAND ----------

val modified_parquet_df = spark.read.parquet(target_file_parquet)

println("== Parquet storage:")
println(s"The count for number of rows seems wrong: ${df_parquet.count()} (should be: ${df_parquet.count()}+2*${more_rows_count})")
println(s"Actually all ${df_parquet.count()+2*more_rows_count} rows should be included but the 2 conflicting schemas can cause the count to be incorrect.")
println(s"The original data had ${df_parquet.columns.length} columns, inserted data had ${df_modified.columns.length} columns. Afterwards we have ${modified_parquet_df.columns.length} columns while we should have ${df_parquet.columns.length + 2} distinct columns.")

getTestDF(modified_parquet_df).show()

println("Unlike the CSV case, the data types for the columns have not been affected. But some columns are just ignored.")
printColumnTypes(modified_parquet_df)


// COMMAND ----------

// MAGIC %md
// MAGIC *Did you get similar output for the data in CSV storage? If not, what was the difference?*
// MAGIC If I correctly understand question, I got the same output as in the example below. 
// MAGIC
// MAGIC *What is your explanation/guess for why the CSV seems broken and the schema cannot be inferred anymore?*
// MAGIC When adding new data no checks of data type is implemented. It leads to the misalignment in case if columns do not match old schema. 
// MAGIC
// MAGIC *Did you get similar output for the data in Parquet storage, and which of the 2 alternatives? If not, what was the difference?*
// MAGIC If I correctly understand question, I got the same output as in the example below, Alternative 2. New columns are added with NA autofill. But the no autofill is applied to the added data, so that the column missing in the new data is dropped.
// MAGIC
// MAGIC *What is your explanation/guess for why not all 10 distinct columns are included in the data frame in the Parquet case?*
// MAGIC Missing columns are dropped by default to maintain schema consistency. 
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC Example output for task 5:
// MAGIC
// MAGIC For CSV:
// MAGIC
// MAGIC ```text
// MAGIC == CSV storage:
// MAGIC The number of rows should be correct: 198166 (i.e., 198082+2*42)
// MAGIC However, the original data had 8 columns, inserted data had 9 columns. Afterwards we have 8 columns while we should have 10 distinct columns.
// MAGIC +------------+--------------------+--------------------+--------------------+---------+--------+-----+---------------+
// MAGIC |          ID|          Start_Time|            End_Time|         Description|     City|  County|State|  Temperature_F|
// MAGIC +------------+--------------------+--------------------+--------------------+---------+--------+-----+---------------+
// MAGIC |   A-3558690|2016-01-14T20:18:...|2017-01-30T13:25:...|Closed at Fullert...|Whitehall|  Lehigh|   PA|           31.0|
// MAGIC |   A-3558700|2016-01-14T20:18:...|2017-01-30T13:34:...|Closed at Fullert...|Whitehall|  Lehigh|   PA|           31.0|
// MAGIC |A-3877306_Z1|2023-02-16T20:48:...|2023-02-16T22:00:...|Closed road from ...| New York|New York|   NY|           58.0|
// MAGIC |A-4076884_Z1|2023-02-12T07:54:...|2023-02-12T10:05:...|Port Authority NY...| New York|New York|   NY|           40.0|
// MAGIC |A-3877306_Z2|2023-02-16T20:48:...|2023-02-16T22:00:...|            New York| New York|      NY| 58.0|prefix-New York|
// MAGIC |A-4076884_Z2|2023-02-12T07:54:...|2023-02-12T10:05:...|            New York| New York|      NY| 40.0|prefix-New York|
// MAGIC +------------+--------------------+--------------------+--------------------+---------+--------+-----+---------------+
// MAGIC
// MAGIC Also the data types cannot be properly inferred anymore, for example temperature is now given as a string instead of a double.
// MAGIC (The reason should be visible in the sample output.):
// MAGIC ID: StringType
// MAGIC Start_Time: StringType
// MAGIC End_Time: StringType
// MAGIC Description: StringType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: StringType
// MAGIC ```
// MAGIC
// MAGIC and for Parquet (alternative 1):
// MAGIC
// MAGIC ```text
// MAGIC == Parquet storage:
// MAGIC The count for number of rows seems wrong: 198082 (should be: 198082+2*42)
// MAGIC Actually all 198166 rows should be included but the 2 conflicting schemas can cause the count to be incorrect.
// MAGIC The original data had 8 columns, inserted data had 9 columns. Afterwards we have 8 columns while we should have 10 distinct columns.
// MAGIC +------------+-------------------+-------------------+--------------------+---------+--------+-----+-------------+
// MAGIC |          ID|         Start_Time|           End_Time|         Description|     City|  County|State|Temperature_F|
// MAGIC +------------+-------------------+-------------------+--------------------+---------+--------+-----+-------------+
// MAGIC |   A-3558690|2016-01-14 20:18:33|2017-01-30 13:25:19|Closed at Fullert...|Whitehall|  Lehigh|   PA|         31.0|
// MAGIC |   A-3558700|2016-01-14 20:18:33|2017-01-30 13:34:02|Closed at Fullert...|Whitehall|  Lehigh|   PA|         31.0|
// MAGIC |A-3877306_Z1|2023-02-16 20:48:16|2023-02-16 22:00:00|Closed road from ...| New York|New York|   NY|         58.0|
// MAGIC |A-4076884_Z1|2023-02-12 07:54:11|2023-02-12 10:05:18|Port Authority NY...| New York|New York|   NY|         40.0|
// MAGIC |A-3877306_Z2|2023-02-16 20:48:16|2023-02-16 22:00:00|                NULL| New York|New York|   NY|         58.0|
// MAGIC |A-4076884_Z2|2023-02-12 07:54:11|2023-02-12 10:05:18|                NULL| New York|New York|   NY|         40.0|
// MAGIC +------------+-------------------+-------------------+--------------------+---------+--------+-----+-------------+
// MAGIC
// MAGIC Unlike the CSV case, the data types for the columns have not been affected. The added columns are just ignored.
// MAGIC ID: StringType
// MAGIC Start_Time: TimestampType
// MAGIC End_Time: TimestampType
// MAGIC Description: StringType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: DoubleType
// MAGIC ```
// MAGIC
// MAGIC Parquet (alternative 2):
// MAGIC
// MAGIC ```text
// MAGIC == Parquet storage:
// MAGIC The count for number of rows seems wrong: 198082 (should be: 198082+2*42)
// MAGIC Actually all 198166 rows should be included but the 2 conflicting schemas can cause the count to be incorrect.
// MAGIC The original data had 8 columns, inserted data had 9 columns. Afterwards we have 9 columns while we should have 10 distinct columns.
// MAGIC +------------+-------------------+-------------------+---------+--------+-----+-------------+---------------+------------+
// MAGIC |          ID|         Start_Time|           End_Time|     City|  County|State|Temperature_F|   AddedColumn1|AddedColumn2|
// MAGIC +------------+-------------------+-------------------+---------+--------+-----+-------------+---------------+------------+
// MAGIC |   A-3558690|2016-01-14 20:18:33|2017-01-30 13:25:19|Whitehall|  Lehigh|   PA|         31.0|           NULL|        NULL|
// MAGIC |   A-3558700|2016-01-14 20:18:33|2017-01-30 13:34:02|Whitehall|  Lehigh|   PA|         31.0|           NULL|        NULL|
// MAGIC |A-3877306_Z1|2023-02-16 20:48:16|2023-02-16 22:00:00| New York|New York|   NY|         58.0|           NULL|        NULL|
// MAGIC |A-4076884_Z1|2023-02-12 07:54:11|2023-02-12 10:05:18| New York|New York|   NY|         40.0|           NULL|        NULL|
// MAGIC |A-3877306_Z2|2023-02-16 20:48:16|2023-02-16 22:00:00| New York|New York|   NY|         58.0|prefix-New York|  New column|
// MAGIC |A-4076884_Z2|2023-02-12 07:54:11|2023-02-12 10:05:18| New York|New York|   NY|         40.0|prefix-New York|  New column|
// MAGIC +------------+-------------------+-------------------+---------+--------+-----+-------------+---------------+------------+
// MAGIC
// MAGIC Unlike the CSV case, the data types for the columns have not been affected. The added columns are just ignored.
// MAGIC ID: StringType
// MAGIC Start_Time: TimestampType
// MAGIC End_Time: TimestampType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: DoubleType
// MAGIC AddedColumn1: StringType
// MAGIC AddedColumn2: StringType
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 6 - Delta - Reading and writing data
// MAGIC
// MAGIC [Delta](https://docs.databricks.com/en/delta/index.html) tables are a storage format that are more advanced. They can be used somewhat like databases.
// MAGIC
// MAGIC This is not native in Spark, but open format which is more and more commonly used.
// MAGIC
// MAGIC Delta is more strict with data. We cannot for example have whitespace in column names as you can have in Parquet and CSV. However, in this exercise the example data is given with column names where these additional requirements have already been fulfilled. And thus you don't have to worry about them in this exercise.
// MAGIC
// MAGIC Delta technically looks more or less like Parquet with some additional metadata files.
// MAGIC
// MAGIC In this this task read the source data given in Delta format into a data frame. And then write a copy of the data into the students container to allow modifications in the following tasks.
// MAGIC

// COMMAND ----------

val source_delta_folder = source_path + s"${data_file}_delta"

// Read the original data in Delta format to a data frame
val df_delta = spark.read.format("delta").load(source_delta_folder)

println(s"== Number or rows: ${df_delta.count()}")
println("== Columns:")
printColumnTypes(df_delta)
println("== Storage files:")
printStorage(source_delta_folder)
println("===== End of output =====")


// COMMAND ----------

val target_file_delta = target_path + data_file + "_delta"
df_delta.write.format("delta").save(target_file_delta)

// write the data from df_delta using the Delta format to the path given by target_file_delta
//???

// Check the written files:
printStorage(target_file_delta)
println("===== End of output =====")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output from task 6:
// MAGIC
// MAGIC ```text
// MAGIC == Number or rows: 198082
// MAGIC == Columns:
// MAGIC ID: StringType
// MAGIC Start_Time: TimestampType
// MAGIC End_Time: TimestampType
// MAGIC Description: StringType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: DoubleType
// MAGIC == Storage files:
// MAGIC 0.0 MB --- abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex5/accidents_delta/_delta_log/00000000000000000000.crc
// MAGIC 0.0 MB --- abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex5/accidents_delta/_delta_log/00000000000000000000.json
// MAGIC 9.56 MB --- abfss://shared@tunics320f2023gen2.dfs.core.windows.net/exercises/ex5/accidents_delta/part-00000-35a096d7-a0ee-439a-85e0-aa78e2935f39-c000.snappy.parquet
// MAGIC Total size: 9.56 MB
// MAGIC ```
// MAGIC
// MAGIC and (the actual file names will be different)
// MAGIC
// MAGIC ```text
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_delta/_delta_log/00000000000000000000.crc
// MAGIC 0.0 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_delta/_delta_log/00000000000000000000.json
// MAGIC 9.56 MB --- abfss://students@tunics320f2023gen2.dfs.core.windows.net/some_unique_folder/ex5/accidents_delta/part-00000-576356db-5737-43ef-b11c-163e91229175-c000.snappy.parquet
// MAGIC Total size: 9.56 MB
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC
// MAGIC ## Task 7 - Delta - Appending data
// MAGIC
// MAGIC Add the new rows using the same schema, `df_new_rows` from task 3, and the new rows using the modified schema, `df_modified` from task 4, to the Delta storage.
// MAGIC
// MAGIC Then, read the merged data and study whether the result with Delta is correct without lost or invalid data.
// MAGIC

// COMMAND ----------

//df_modified = df_new_rows
df_new_rows.write.mode(SaveMode.Append).format("delta").save(target_file_delta)

df_modified.write.mode(SaveMode.Append).format("delta").save(target_file_delta)

// COMMAND ----------

// Append the new rows using the same schema, df_new_rows, to the Delta storage:
//???
df_new_rows.write.mode(SaveMode.Append).format("delta").save(target_file_delta)


// On default, Delta is similar to Parquet. However, we can enable it to handle schema modifications.
spark.conf.set("spark.databricks.delta.schema.autoMerge.enabled", "true")

// Append the new rows using the modified schema, df_modified, to the Delta storage:
//???
df_modified.write.mode(SaveMode.Append).format("delta").save(target_file_delta)


// COMMAND ----------

// Read the merged data from Delta storage to check that the new rows have been stored
val modified_delta_df = spark.read.format("delta").load(target_file_delta)

println(s"The number of rows should be correct: ${modified_delta_df.count()} (i.e., ${df_delta.count()}+2*${more_rows_count})")
println(s"The original data had ${modified_delta_df.columns.length} columns, inserted data had ${df_modified.columns.length} columns. Afterwards we have ${modified_delta_df.columns.length} columns while we should have ${df_delta.columns.length + 2} distinct columns.")
println("Delta handles these perfectly. The columns which were not given values are available with NULL values.")

getTestDF(modified_delta_df).show()
printColumnTypes(modified_delta_df)
println("===== End of output =====")


// COMMAND ----------

// MAGIC %md
// MAGIC Example output from task 7:
// MAGIC
// MAGIC ```text
// MAGIC The number of rows should be correct: 198166 (i.e., 198082+2*42)
// MAGIC The original data had 10 columns, inserted data had 9 columns. Afterwards we have 10 columns while we should have 10 distinct columns.
// MAGIC Delta handles these perfectly. The columns which were not given values are available with NULL values.
// MAGIC +------------+-------------------+-------------------+--------------------+---------+--------+-----+-------------+---------------+------------+
// MAGIC |          ID|         Start_Time|           End_Time|         Description|     City|  County|State|Temperature_F|   AddedColumn1|AddedColumn2|
// MAGIC +------------+-------------------+-------------------+--------------------+---------+--------+-----+-------------+---------------+------------+
// MAGIC |   A-3558690|2016-01-14 20:18:33|2017-01-30 13:25:19|Closed at Fullert...|Whitehall|  Lehigh|   PA|         31.0|           NULL|        NULL|
// MAGIC |   A-3558700|2016-01-14 20:18:33|2017-01-30 13:34:02|Closed at Fullert...|Whitehall|  Lehigh|   PA|         31.0|           NULL|        NULL|
// MAGIC |A-3877306_Z1|2023-02-16 20:48:16|2023-02-16 22:00:00|Closed road from ...| New York|New York|   NY|         58.0|           NULL|        NULL|
// MAGIC |A-4076884_Z1|2023-02-12 07:54:11|2023-02-12 10:05:18|Port Authority NY...| New York|New York|   NY|         40.0|           NULL|        NULL|
// MAGIC |A-3877306_Z2|2023-02-16 20:48:16|2023-02-16 22:00:00|                NULL| New York|New York|   NY|         58.0|prefix-New York|  New column|
// MAGIC |A-4076884_Z2|2023-02-12 07:54:11|2023-02-12 10:05:18|                NULL| New York|New York|   NY|         40.0|prefix-New York|  New column|
// MAGIC +------------+-------------------+-------------------+--------------------+---------+--------+-----+-------------+---------------+------------+
// MAGIC
// MAGIC ID: StringType
// MAGIC Start_Time: TimestampType
// MAGIC End_Time: TimestampType
// MAGIC Description: StringType
// MAGIC City: StringType
// MAGIC County: StringType
// MAGIC State: StringType
// MAGIC Temperature_F: DoubleType
// MAGIC AddedColumn1: StringType
// MAGIC AddedColumn2: StringType
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 8 - Delta - Full modifications
// MAGIC
// MAGIC Editing existing values is not possible without overwriting the entire dataset.
// MAGIC
// MAGIC Previously we only added new lines. This way of working only supports adding new data. It does **not** support modifying existing data: updating values or deleting rows.
// MAGIC (We could do that manually by adding a primary key and timestamp and always searching for the newest value.)
// MAGIC
// MAGIC Nevertheless, Delta tables take care of this and many more itself.
// MAGIC
// MAGIC This task is divided into multiple cells with separate instructions. In the following cell add the code to write the `df_delta_small` into Delta storage.
// MAGIC

// COMMAND ----------

// Let us first save smaller data so that it is easier to see what is happening
val delta_table_file = target_path + data_file + "_deltatable_small"

// create a small 6 row data frame with only 5 columns
val df_delta_small =  getTestDF(modified_delta_df, Seq("Z1"), 3)
    .drop("Description", "End_Time", "County", "AddedColumn1", "AddedColumn2")

// Write the new small data frame to storage in Delta format to path based on delta_table_file
//???
df_delta_small.write.format("delta").save(delta_table_file)


// Create Delta table based on your target folder
val deltatable = DeltaTable.forPath(spark, delta_table_file)


// COMMAND ----------

// MAGIC %md
// MAGIC Delta tables are more like database type tables. We define them based on data and modify the table itself.
// MAGIC
// MAGIC We do this by telling Delta what is the primary key of the data. After this we tell it to "merge" new data to the old one. If primary key matches, we update the information. If primary key is new, add a row.
// MAGIC
// MAGIC In the following cell, add the code to update the `deltatable` with the given updates in `df_delta_update`.
// MAGIC
// MAGIC The rows should be updated when the `ID` columns match. And if the id from the update is a new one, a new row should be inserted into the `deltatable`.
// MAGIC
// MAGIC Documentation page [https://docs.databricks.com/en/delta/merge.html#modify-all-unmatched-rows-using-merge](https://docs.databricks.com/en/delta/merge.html#modify-all-unmatched-rows-using-merge) might be helpful in this task.
// MAGIC

// COMMAND ----------

// create a 5 row data frame with the same columns with updated values for the temperature
val df_delta_update = df_new_rows
    .limit(5)
    .drop("Description", "End_Time", "County")
    .withColumn("Temperature_F", round(random(seed=lit(1)) * 100, 1))

// Show the data before the merge
println(s"== Before merge, the size of Delta file is ${sizeInKB(delta_table_file)} kB and contains ${deltatable.toDF.count()} rows.")
deltatable.toDF.sort(desc("Start_Time")).show()


// code for updating the deltatable with df_delta_update
///???

deltatable.as("target")
  .merge(
    df_delta_update.as("updates"),
    "target.ID = updates.ID"
  )
  .whenMatched()
  .updateAll()
  .whenNotMatched()
  .insertAll()
  .execute()


// Show the data after the merge
println(s"== After merge, the size of Delta file is ${sizeInKB(delta_table_file)} kB and contains ${deltatable.toDF.count()} rows.")
deltatable.toDF.sort(desc("Start_Time")).show()


// COMMAND ----------

// MAGIC %md
// MAGIC Note: If you execute the previous cell multiple times, you can notice that the file size increases every time. However, the amount of rows does not change.

// COMMAND ----------

// We can get rid of additional data by vacuuming
println(s"== Before vacuum, the size of Delta file is ${sizeInKB(delta_table_file)} kB.")

// Typically we do not want to vacuum all the data, only data older than 30 days or so.
// We need to tell Delta that we really want to do something stupid
spark.conf.set("spark.databricks.delta.retentionDurationCheck.enabled", false)
deltatable.vacuum(0)
println(s"== After vacuum, the size of Delta file is ${sizeInKB(delta_table_file)} kB.")

// you can print the files after vacuuming by uncommenting the following
// printStorage(delta_table_file)

// the vacuuming should not change the actual data
// deltatable.toDF.sort(desc("Start_Time")).show()


// COMMAND ----------

// MAGIC %md
// MAGIC Example output from task 8:
// MAGIC
// MAGIC ```text
// MAGIC == Before merge, the size of Delta file is 3.44 kB and contains 6 rows.
// MAGIC +------------+-------------------+---------+-----+-------------+
// MAGIC |          ID|         Start_Time|     City|State|Temperature_F|
// MAGIC +------------+-------------------+---------+-----+-------------+
// MAGIC |A-3877306_Z1|2023-02-16 20:48:16| New York|   NY|         58.0|
// MAGIC |A-4076884_Z1|2023-02-12 07:54:11| New York|   NY|         40.0|
// MAGIC |A-5005008_Z1|2023-01-30 22:00:00| New York|   NY|         50.0|
// MAGIC |   A-3558690|2016-01-14 20:18:33|Whitehall|   PA|         31.0|
// MAGIC |   A-3558700|2016-01-14 20:18:33|Whitehall|   PA|         31.0|
// MAGIC |   A-3558713|2016-01-14 20:18:33|Whitehall|   PA|         31.0|
// MAGIC +------------+-------------------+---------+-----+-------------+
// MAGIC
// MAGIC == After merge, the size of Delta file is 5.2 kB and contains 8 rows.
// MAGIC +------------+-------------------+---------+-----+-------------+
// MAGIC |          ID|         Start_Time|     City|State|Temperature_F|
// MAGIC +------------+-------------------+---------+-----+-------------+
// MAGIC |A-3877306_Z1|2023-02-16 20:48:16| New York|   NY|         63.6|
// MAGIC |A-4076884_Z1|2023-02-12 07:54:11| New York|   NY|         59.9|
// MAGIC |A-5005008_Z1|2023-01-30 22:00:00| New York|   NY|         13.5|
// MAGIC |A-5284521_Z1|2023-01-29 22:05:45| New York|   NY|          7.7|
// MAGIC |A-4949927_Z1|2023-01-28 03:26:49| New York|   NY|         85.4|
// MAGIC |   A-3558690|2016-01-14 20:18:33|Whitehall|   PA|         31.0|
// MAGIC |   A-3558700|2016-01-14 20:18:33|Whitehall|   PA|         31.0|
// MAGIC |   A-3558713|2016-01-14 20:18:33|Whitehall|   PA|         31.0|
// MAGIC +------------+-------------------+---------+-----+-------------+
// MAGIC ```
// MAGIC
// MAGIC and (the numbers might not match exactly)
// MAGIC
// MAGIC ```text
// MAGIC == Before vacuum, the size of Delta file is 5.2 kB.
// MAGIC Deleted 1 files and directories in a total of 1 directories.
// MAGIC == After vacuum, the size of Delta file is 3.5 kB.
// MAGIC ```
// MAGIC
