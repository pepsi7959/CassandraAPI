package com.ais.damocles.spark.demo;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

import java.util.Optional;
import java.util.Properties;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

import scala.Tuple2;
import scala.Tuple3;

import com.ais.damocles.spark.schema.alltrade.RequestGoods;
import com.ais.damocles.spark.schema.alltrade.OrderTransfer;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraRow;

public class AlltradeReportsAggregation {
	public static JavaSparkContext sc;
	public static final String DAMOCLES_KEYSPACE = "damocles";
	public static final String REQUESTGOODS_TABLE = "requestgoods";
	public static final String ORDERTRANSFER_TABLE = "ordertransfer";

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
				.cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE);
		JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
				.mapToPair(f -> {
					RequestGoods requestGoods = new RequestGoods();
					requestGoods.setRequestNo(f.getString(0));
					requestGoods.setCreateBy(f.getString(1));
					requestGoods.setRequestStatus(f.getString(2));
					return new Tuple2<>(requestGoods.getRequestNo(),
							requestGoods);
				});

		cassandraRowRequestGoods.foreach(f -> System.out.println("column 1 : "
				+ f.getString(0)));

		/* Load OrderTransferIn from the Cassandra */
		JavaRDD<CassandraRow> cassandraRowTransferIn = javaFunctions(sc)
				.cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE).where(
						"transactionType=?", "TransferIn");

		/* Load OrderTransferOut from the Cassandra */
		JavaRDD<CassandraRow> cassandraRowTransferOut = javaFunctions(sc)
				.cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE).where(
						"transactionType=?", "TransferOut");

		/* Load TransferIn */
		JavaPairRDD<String, OrderTransfer> transferInPairRDD = cassandraRowTransferIn
				.mapToPair(f -> {
					OrderTransfer orderTransfer = new OrderTransfer();
					orderTransfer.setTransferNo(f.getString(0));
					orderTransfer.setDocRef(f.getString(1));
					orderTransfer.setTransactionType(f.getString(2));
					orderTransfer.setTransferDetail(f.getString(3));
					return new Tuple2<>(orderTransfer.getDocRef(),
							orderTransfer);
				});

		/* Load TransferOut */
		JavaPairRDD<String, OrderTransfer> transferOutPairRDD = cassandraRowTransferOut
				.mapToPair(f -> {
					OrderTransfer orderTransferOut = new OrderTransfer();
					orderTransferOut.setTransferNo(f.getString(0));
					orderTransferOut.setDocRef(f.getString(1));
					orderTransferOut.setTransactionType(f.getString(2));
					orderTransferOut.setTransferDetail(f.getString(3));
					return new Tuple2<>(orderTransferOut.getDocRef(),
							orderTransferOut);
				});

		/* show Request Goods */
		System.out.println("===== Request Goods =====");
		requestGoodsPairRDD.foreach(f -> System.out.println("RequstNo : "
				+ f._1()));

		/* show TransferOut */
		System.out.println("===== TransferOut =====");
		transferOutPairRDD.foreach(f -> System.out.println("TransferNo : "
				+ f._1()));

		/* show TransferIn */
		System.out.println("===== TransferIn =====");
		transferInPairRDD.foreach(f -> System.out.println("TransferNo : "
				+ f._1()));

		/* join TransferOut and RequestGoods */
		JavaPairRDD<String, Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>>> joinTransferRequestGoods = transferOutPairRDD
				.leftOuterJoin(requestGoodsPairRDD);

		System.out.println("===== Join TransferOut and RequestGoods");
		joinTransferRequestGoods.foreach(f -> {
			try {
				System.out.println("Key : " + f._1() + "CreatedBy : "
						+ f._2()._2().get().getCreateBy());
			} catch (IllegalStateException ex) {
				System.out.println(ex.toString());
			}
		});

		/* change key of tranferOutRequestGoods */
		JavaPairRDD<String, Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>>> joinTransferOutRequestGoodsByDocRef = joinTransferRequestGoods
				.mapToPair(f -> new Tuple2<String, Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>>>(
						f._2()._1().getTransferNo(), f._2()));

		System.out.println("===== Change Key of transferOutRequestGoods =====");
		joinTransferOutRequestGoodsByDocRef.foreach(f -> {
			try {
				System.out.println("Key : " + f._1() + "CreatedBy: "
						+ f._2()._2().get().getCreateBy());
			} catch (Exception ex) {
				System.out.println(ex);
			}
		});

		/* join TransferOutRequestGoods and TransferIn */
		JavaPairRDD<String, Tuple2<Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>>, com.google.common.base.Optional<OrderTransfer>>> allAggregation = joinTransferOutRequestGoodsByDocRef
				.leftOuterJoin(transferInPairRDD);

		System.out.println("======== All aggregation ========");
		allAggregation.foreach(f -> {
			
			String createdBy = f._2()._1()._2()
					.isPresent() ? f._2()._1()._2().get()
					.getCreateBy() : null;
			String transferDetail = f._2()._2().isPresent() ? f._2()._2().get()
					.getTransferDetail() : null;
			System.out.println("key : " + f._1() + " TransferNo : "
					+ f._2()._1()._1().getTransferNo() + " transferDetail : "
					+ f._2()._1()._1().getTransferDetail() + " CreateBy : "
					+ createdBy + " TransferDetail : " + transferDetail);
		});
	}
}
