package com.ais.damocles.spark;

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

import com.ais.damocles.spark.schema.SummaryByUser;
import com.ais.damocles.spark.schema.Usage;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraJavaUtil;
import com.datastax.spark.connector.japi.CassandraRow;

public class SparkAggregation {
	public static JavaSparkContext sc;

	public static void main(String[] args) throws Exception {

		// read Spark and Cassandra properties and create SparkConf
		Properties prop = null;
		if( args.length == 0 ){
			prop = PropertyFileReader.readPropertyFile();
		}else if(args.length == 1){
			prop = PropertyFileReader.readPropertyFile(args[0]);
		}else{
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
	 * Aggrate Usage
	 */
	public static void AggregateUsage(JavaStreamingContext jssc) {

		sc = jssc.sparkContext();
		JavaRDD<CassandraRow> allUsage = javaFunctions(sc).cassandraTable(
				"damocles", "usage");

		JavaRDD<Usage> usageRDD = allUsage.map(x -> new Usage(x.getString(0), x
				.getString(1), x.getInt(2)));

		System.out.println("All table : " + allUsage.count());
		usageRDD.foreach(f -> System.out.println("|" + f.getUser() + "|"
				+ f.getType() + "|" + f.getUsage()));

		/*
		 * |user|type|usage| ----------------- |aaaa|mobile|20 | |bbbb|net |30 |
		 * |aaaa|net |50 |
		 */

		System.out.println("Summay");
		JavaPairRDD<String, Integer> flatMappedUsage = allUsage
				.mapToPair(f -> new Tuple2<>(f.getString(0), Integer.parseInt(f
						.getString(2))));
		JavaPairRDD<String, Integer> reducedUsage_by_user = flatMappedUsage
				.reduceByKey((a, b) -> a + b);
		reducedUsage_by_user.foreach(f -> System.out.println("User: " + f._1()
				+ " Usage: " + f._2()));

		/* Inser Sumary to Cassandra */
		Map<String, String> columnNameMappings = new HashMap<String, String>();
		columnNameMappings.put("user", "user");
		columnNameMappings.put("usage", "all_usages");
		JavaRDD<SummaryByUser> summary = reducedUsage_by_user
				.map(f -> new SummaryByUser(f._1(), f._2()));
		summary.foreach(f -> System.out.println("User: " + f.getUser()
				+ " Usage: " + f.getUsage()));
		javaFunctions(summary).writerBuilder(
				"damocles",
				"summary_by_user",
				CassandraJavaUtil.mapToRow(SummaryByUser.class,
						columnNameMappings)).saveToCassandra();

	}
}
