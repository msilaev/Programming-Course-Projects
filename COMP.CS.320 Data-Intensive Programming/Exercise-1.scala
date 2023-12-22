// Databricks notebook source
// MAGIC %md
// MAGIC # COMP.CS.320 Data-Intensive Programming, Exercise 1
// MAGIC
// MAGIC This exercise is mostly introduction to the Azure Databricks notebook system.
// MAGIC These are some basic programming tasks that can be done in either Scala or Python. This is the **Scala** version, switch to the Python version if you want to do the task in Python.
// MAGIC
// MAGIC Each task has its own cell for the code. Add your solutions to the cells. You are free to add more cells if you feel it is necessary. There are cells with test code following most of the tasks that involve producing code.
// MAGIC
// MAGIC Don't forget to submit your solutions to Moodle.

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 1 - Read tutorial
// MAGIC
// MAGIC Read the "[Basics of using Databricks notebooks](https://adb-5736551434993186.6.azuredatabricks.net/?o=5736551434993186#notebook/1892052735998707/command/1892052735998713)" tutorial notebook, at least the initial information and the first code examples. Clone the tutorial notebook to your own workspace and run at least those first code examples.
// MAGIC
// MAGIC To get a point from this task, add "done" (or something similar) to the following cell (after you have read the tutorial).

// COMMAND ----------

// MAGIC %md
// MAGIC Task 1 is done

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 2 - Basic function
// MAGIC
// MAGIC In the following cell write a simple function `mySum`, that takes two integer as parameters and returns their sum.

// COMMAND ----------

def mySum(x: Int, y: Int ) = { x  + y }

// COMMAND ----------

// you can test your function by running both the previous and this cell

val sum41 = mySum(20, 21)
sum41 == 41 match {
    case true => println(s"correct result: 20+21 = ${sum41}")
    case false => println(s"wrong result: ${sum41} != 41")
}


// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 3 - Fibonacci numbers
// MAGIC
// MAGIC The Fibonacci numbers, `F_n`, are defined such that each number is the sum of the two preceding numbers. The first two Fibonacci numbers are:
// MAGIC
// MAGIC $$F_0 = 0 \qquad F_1 = 1$$
// MAGIC
// MAGIC In the following cell, write a **recursive** function, `fibonacci`, that takes in the index and returns the Fibonacci number. (no need for any optimized solution here)
// MAGIC

// COMMAND ----------

def fibonacci(n: Int) : Long= 
{ if (n < 2 ) {
 n 
}
else { 
 fibonacci(n-1) + fibonacci(n-2) 
}
}

// COMMAND ----------

val fibo6 = fibonacci(6)
fibo6 == 8 match {
    case true => println("correct result: fibonacci(6) == 8")
    case false => println(s"wrong result: ${fibo6} != 8")
}

val fibo11 = fibonacci(11)
fibo11 == 89 match {
    case true => println("correct result: fibonacci(11) == 89")
    case false => println(s"wrong result: ${fibo11} != 89")
}


// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 4 - Higher order functions 1
// MAGIC
// MAGIC Use functions `map` and `reduce` to compute the sum of cubes of the values in the given list.

// COMMAND ----------

val myList = List(2, 3, 5, 7, 11, 13, 17, 19)

val cubeSum = myList.map(x=> x*x*x).reduce( (x,y)=> x+y )

// COMMAND ----------

cubeSum == 15803 match {
    case true => println(s"correct result: ${cubeSum} == 15803")
    case false => println(s"wrong result: ${cubeSum} != 15803")
}

// COMMAND ----------

"sheena is a punk rocker she is a punk punk".split(" ").map(s => (s, 1)).groupBy(p => p._1).mapValues(v => v.length)

// COMMAND ----------

"sheena is a punk rocker she is a punk punk"
    .split(" ")
    .map((_, 1))
    .groupBy(_._1)
    .mapValues(v => v.map(_._2).reduce(_+_))

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 5 - Higher order functions 2
// MAGIC
// MAGIC Explain the following code snippet. You can try the snippet piece by piece in a notebook cell or search help from Scaladoc ([https://www.scala-lang.org/api/2.12.x/](https://www.scala-lang.org/api/2.12.x/)).
// MAGIC
// MAGIC ```scala
// MAGIC "sheena is a punk rocker she is a punk punk"
// MAGIC     .split(" ")
// MAGIC     .map(s => (s, 1))
// MAGIC     .groupBy(p => p._1)
// MAGIC     .mapValues(v => v.length)
// MAGIC ```
// MAGIC
// MAGIC What about?
// MAGIC
// MAGIC ```scala
// MAGIC "sheena is a punk rocker she is a punk punk"
// MAGIC     .split(" ")
// MAGIC     .map((_, 1))
// MAGIC     .groupBy(_._1)
// MAGIC     .mapValues(v => v.map(_._2).reduce(_+_))
// MAGIC ```
// MAGIC

// COMMAND ----------

// MAGIC %md
// MAGIC In the first example the string is split to a list of string using the space symbol " " as splitter. Then each element of the list is mapped to a tuple where first element is the value and the second is the number 1. Then the list of tuples is grouped by the first elements of tuples. The result is Map (analoug of Pithon dict) with keys are the strings and the values are lists of tuples containing these strings as first elements. Finally, mapValues transfroms the map values keeping the keys unchanges. Here it changes the list to its length.
// MAGIC
// MAGIC In the second example there are two differences. First, the anonymous arguments _ are used to go through the collection while implementing map. Second, in the final step the sum of second values (1) in the tuples intering each list in the map is calculated using reduce. It gives the number of elements in each list simular to the length method. 

// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 6 - Cube root
// MAGIC
// MAGIC Write a (recursive) function, `cubeRoot`, that returns an approximate value for the cube root of the input. Use the Newton's method, [https://en.wikipedia.org/wiki/Newton's_method](https://en.wikipedia.org/wiki/Newton%27s_method), with the initial guess of 1. For the cube root this Newton's method translates to:
// MAGIC
// MAGIC $$y_0 = 1$$
// MAGIC $$y_{n+1} = \frac{1}{3}\bigg(2y_n + \frac{x}{y_n^2}\bigg) $$
// MAGIC
// MAGIC where `x` is the input value and `y_n` is the guess for the cube root after `n` iterations.
// MAGIC
// MAGIC Example steps when `x=8`:
// MAGIC
// MAGIC $$y_0 = 1$$
// MAGIC $$y_1 = \frac{1}{3}\big(2*1 + \frac{8}{1^2}\big) = 3.33333$$
// MAGIC
// MAGIC $$y_2 = \frac{1}{3}\big(2*3.33333 + \frac{8}{3.33333^2}\big) = 2.46222$$
// MAGIC
// MAGIC $$y_3 = \frac{1}{3}\big(2*2.46222 + \frac{8}{2.46222^2}\big) = 2.08134$$
// MAGIC
// MAGIC $$...$$
// MAGIC
// MAGIC You will have to decide yourself on what is the condition for stopping the iterations. (you can add parameters to the function if you think it is necessary)
// MAGIC

// COMMAND ----------

def cubeRoot(x: Double,  precision: Double, y: Double =1): Double = 
{ 
  if (scala.math.abs(y -  (2*y + x/(y*y))/3)  < precision) 
  { 
   y
  }
  else 
  {  
    cubeRoot(x, precision, (2*y + x/(y*y))/3)
  }
} 




// COMMAND ----------

def handleCheck(expectedOutput: Double, precision: Double): Unit = {
    val inputValue = scala.math.pow(expectedOutput, 3)
    val rootValue = cubeRoot(inputValue, precision)
    Math.abs(rootValue - expectedOutput) < precision match {
        case true => println(s"correct result: ${inputValue}^(1/3) == ${rootValue}")
        case false => println(s"wrong result: ${rootValue} != ${expectedOutput}")
    }
}

handleCheck(2.0, 1e-6)
handleCheck(3.0, 1e-6)
handleCheck(2023.0, 1e-6)
handleCheck(1.0/42, 1e-6)



// COMMAND ----------

// MAGIC %md
// MAGIC ## Task 7 - First Spark task
// MAGIC
// MAGIC Create and display a DataFrame with your own data similarly as was done in the tutorial notebook.
// MAGIC
// MAGIC Then fetch the number of rows from the DataFrame.
// MAGIC

// COMMAND ----------

import org.apache.spark.sql.DataFrame

val myData = Seq(
  ("Denmark", 5894725, 42952),
  ("Sweden", 10576799, 447425),
  ("Norway", 5451674, 385207),
  ("Finland", 5545475, 338462)
)

val myDF: DataFrame = spark.createDataFrame(myData).toDF("Country", "Population", "Area (km²)")
val numberOfRows: Long = myDF.count()


// COMMAND ----------

myData.size == numberOfRows match {
    case true => println("Correct, the data and the DataFrame have the same number of rows.")
    case false => println(s"Wrong, the data has ${myData.size} items while the DataFrame has ${numberOfRows} rows.")
}

