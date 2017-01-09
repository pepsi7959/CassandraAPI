cd target
$SPARK_HOME/bin/spark-submit  --master spark://10.252.169.15:7077 --class "com.ais.damocles.spark.demo.AlltradeReportsAggregation" spark-provisioning-0.0.1.jar $1


