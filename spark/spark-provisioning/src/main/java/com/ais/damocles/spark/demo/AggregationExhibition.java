package com.ais.damocles.spark.demo;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
//import java.util.Date;
//import java.text.*;

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
						prop.getProperty("com.ais.damocles.spark.name"))
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

		JavaRDD<Exhibition> usageRDD = allUsage.map(x -> new Exhibition(x
				.getString(0), x.getString(1), x.getString(2), x.getInt(3)));

		System.out.println("All table : " + allUsage.count());
		usageRDD.foreach(f -> System.out.println("|" + f.getAppName() + "|"
				+ f.getTimeEvent() + "|" + f.getEventType() + "|"
				+ f.getUsage()));

		/*
		 * |user|type|usage| ----------------- |aaaa|mobile|20 | |bbbb|net |30 |
		 * |aaaa|net |50 |
		 */

		/*
		 * -- cassandra --- AMF | 1/1/10 11:22:22 | NET | 100 AMF | 1/1/10
		 * 11:22:24 | MOBILE |300
		 * 
		 * -- class spark AMF | 1/1/10 | 100 AMF | 1/1/10 | 300
		 * 
		 * -- class spark AMF | 1/1/10 | 400
		 */

		System.out.println("Summay");

		// Date myDate = new Date();
		// System.out.println(new
		// SimpleDateFormat("yyyy-MM-dd").format(myDate));
		SummaryByApp sumApp = new SummaryByApp();
		/*
		 * String ss = sumApp.todate("2013-09-08 15:37:50+0700");
		 * System.out.println(ss);
		 */

		JavaPairRDD<String, SummaryByApp> flatMappedUsage = allUsage
				.mapToPair(f -> new Tuple2<>(f.getString(0)
						+ sumApp.format_date(f.getString(1)), new SummaryByApp(
						f.getString(0), sumApp.format_date(f.getString(1)), f
								.getInt(3))));

		JavaPairRDD<String, SummaryByApp> reducedUsage_by_user = flatMappedUsage
				.reduceByKey((a, b) -> {

					a.setUsage(a.getUsage() + b.getUsage());

					return a;
				});

		// reducedUsage_by_user.foreach(f -> System.out.println("key: " + f._1()
		// +" Usage: " + f._2().getUsage()));

		/* Inser Sumary to Cassandra */
		Map<String, String> columnNameMappings = new HashMap<String, String>();
		columnNameMappings.put("user", "user");
		columnNameMappings.put("date", "date");
		columnNameMappings.put("usage", "all_usages");

		JavaRDD<SummaryByApp> summary = reducedUsage_by_user.map(f -> f._2());
		summary.foreach(f -> System.out.println("User: " + f.getUser()
				+ "Date: " + f.getDate() + " Usage: " + f.getUsage()));

		javaFunctions(summary).writerBuilder(
				"damocles",
				"summary_by_app",
				CassandraJavaUtil.mapToRow(SummaryByApp.class,
						columnNameMappings)).saveToCassandra();

	}
}
