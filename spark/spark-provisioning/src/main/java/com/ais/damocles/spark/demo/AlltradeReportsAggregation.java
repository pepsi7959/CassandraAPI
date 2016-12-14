package com.ais.damocles.spark.demo;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Date;
import java.text.*;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

import scala.Tuple2;
import com.ais.damocles.spark.schema.alltrade.RequestGoods;
import com.ais.damocles.spark.schema.demo.SummaryByApp;
import com.ais.damocles.spark.schema.alltrade.OrderTransfer;
import com.ais.damocles.spark.schema.demo.Exhibition;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraJavaUtil;
import com.datastax.spark.connector.japi.CassandraRow;

public class AlltradeReportsAggregation {
	public static JavaSparkContext sc;
	public static final String DAMOCLES_KEYSPACE = "damocles";
	public static final String REQUESTGOODS_TABLE = "reqeustgoods";
	public static final String ORDERTRANSFER_TABLE = "orderTransfer";

	public static void main(String[] args) throws Exception {

		/* read Spark and Cassandra properties and create SparkConf */
		Properties prop = null;
		if (args.length == 0) {
			prop = PropertyFileReader.readPropertyFile();
		} else if (args.length == 1) {
			prop = PropertyFileReader.readPropertyFile(args[0]);
		} else {
			prop = PropertyFileReader.readPropertyFile();
		}

		SparkConf conf = new SparkConf()
				.setAppName(prop.getProperty("com.ais.damocles.spark.name"))
				.setMaster(prop.getProperty("com.ais.damocles.spark.master"))
				.set("spark.cassandra.connection.host",
						prop.getProperty("com.ais.damocles.cassandra.host"))
				.set("spark.cassandra.connection.port",
						prop.getProperty("com.ais.damocles.cassandra.port"))
				.set("spark.cassandra.connection.keep_alive_ms",
						prop.getProperty("com.ais.damocles.cassandra.keep_alive"));

		/* batch interval of 5 seconds for incoming stream */
		JavaStreamingContext jssc = new JavaStreamingContext(conf,
				Durations.seconds(5));

		/* add check point directory */
		jssc.checkpoint(prop
				.getProperty("com.ais.damocles.spark.checkpoint.dir"));

		AggregateTransfer(jssc);

		jssc.start();
		System.out.println("start jssc");
		jssc.awaitTermination();
		System.out.println("terminate jssc");
	}

	/*
	 * Aggregate Transfer
	 */
	public static void AggregateTransfer(JavaStreamingContext jssc) {

		sc = jssc.sparkContext();

		/* Load ReqeustGoods from the Cassandra */
		JavaRDD<CassandraRow> cassandraRowRequestGoods = javaFunctions(sc)
				.cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE).select("requestNo", "requestStatus");
		JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
				.mapToPair(f -> new Tuple2<>("key", new RequestGoods()));

		/* Load OrderTransfer from the Cassandra */
		JavaRDD<CassandraRow> cassandraRowTransferIn = javaFunctions(sc)
				.cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE).select("transferNo", "transferStatus").where(cqlWhereClause, args);
		
		/* Load TransferIn */
		JavaPairRDD<String, OrderTransfer> transferInPairRDD = cassandraRowOrderTransfer
				.mapToPair(f -> new Tuple2<>("Key", new OrderTransfer()));

		/* Load TransferOut */
		JavaPairRDD<String, OrderTransfer> transferOutPairRDD = cassandraRowOrderTransfer
				.mapToPair(f -> new Tuple2<>("key", new OrderTransfer()));

		JavaRDD<Exhibition> usageRDD = allUsage.map(x -> new Exhibition(x
				.getString(0), x.getString(1), x.getString(2), x.getInt(3)));

		System.out.println("All table : " + allUsage.count());
		usageRDD.foreach(f -> System.out.println("|" + f.getAppName() + "|"
				+ f.getTimeEvent() + "|" + f.getEventType() + "|"
				+ f.getUsage()));

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
