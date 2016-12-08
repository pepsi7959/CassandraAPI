package com.ais.damocles.spark.demo;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

import scala.Tuple2;

import com.ais.damocles.spark.schema.demo.SummaryByApp;
//import com.ais.damocles.spark.schema.Usage;
import com.ais.damocles.spark.schema.demo.Exhibition;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraJavaUtil;
import com.datastax.spark.connector.japi.CassandraRow;

public class AggregationExhibition {
	public static JavaSparkContext sc;

	public static void main(String[] args) throws Exception {

		// read Spark and Cassandra properties and create SparkConf
		Properties prop = null;
		if (args.length == 0) {
			prop = PropertyFileReader.readPropertyFile();
		} else if (args.length == 1) {
			prop = PropertyFileReader.readPropertyFile(args[0]);
		} else {
			prop = PropertyFileReader.readPropertyFile();
		}
		SparkConf conf = new SparkConf()
				.setAppName(
						prop.getProperty("com.ais.damocles.spark.damocles.name"))
				.setMaster(prop.getProperty("com.ais.damocles.spark.master"))
				.set("spark.cassandra.connection.host",
						prop.getProperty("com.ais.damocles.cassandra.host"))
				.set("spark.cassandra.connection.port",
						prop.getProperty("com.ais.damocles.cassandra.port"))
				.set("spark.cassandra.connection.keep_alive_ms",
						prop.getProperty("com.ais.damocles.cassandra.keep_alive"));

		// batch interval of 5 seconds for incoming stream
		JavaStreamingContext jssc = new JavaStreamingContext(conf,
				Durations.seconds(5));

		// add check point directory
		jssc.checkpoint(prop
				.getProperty("com.ais.damocles.spark.checkpoint.dir"));

		AggregateUsage(jssc);

		jssc.start();
		System.out.println("start jssc");
		jssc.awaitTermination();
		System.out.println("terminate jssc");
	}

	/*
	 * AggregationExhibition
	 */
	public static void AggregateUsage(JavaStreamingContext jssc) {

		sc = jssc.sparkContext();
		JavaRDD<CassandraRow> allUsage = javaFunctions(sc).cassandraTable(
				"damocles", "exhibition_by_app");

		JavaRDD<Exhibition> usageRDD = allUsage.map(x -> new Exhibition(x.getString(0), x
				.getString(1), x.getString(2), x.getInt(3)));

		//System.out.println("All table : " + allUsage.count());
		//usageRDD.foreach(f -> System.out.println("|" + f.getAppName() + "|"
				//+ f.getTimeEvent() + "|" + f.getUsage()));

		/*
		 * |user|type|usage| ----------------- |aaaa|mobile|20 | |bbbb|net |30 |
		 * |aaaa|net |50 |
		 */

		System.out.println("Summay");
		JavaPairRDD<String, Integer> flatMappedUsage = allUsage
				.mapToPair(f -> new Tuple2<>(f.getString(0)+f.getString(1),f.getInt(3)));
		JavaPairRDD<String, Integer> reducedUsage_by_user = flatMappedUsage
				.reduceByKey((a, b) -> a + b);
		reducedUsage_by_user.foreach(f -> System.out.println("User: " + f._1()
				+ " Usage: " + f._2()));

		/* Inser Sumary to Cassandra */
		Map<String, String> columnNameMappings = new HashMap<String, String>();
		columnNameMappings.put("user", "user");
		columnNameMappings.put("date", "date");
		columnNameMappings.put("usage", "all_usages");
		JavaRDD<SummaryByApp> summary = reducedUsage_by_user
				.map(f -> new SummaryByApp(f._1(),"NULL", f._2()));
		summary.foreach(f -> System.out.println("User: " + f.getUser()
				+ " Usage: " + f.getUsage()));
		javaFunctions(summary).writerBuilder(
				"damocles",
				"summary_by_app",
				CassandraJavaUtil.mapToRow(SummaryByApp.class,
						columnNameMappings)).saveToCassandra();

	}
}
