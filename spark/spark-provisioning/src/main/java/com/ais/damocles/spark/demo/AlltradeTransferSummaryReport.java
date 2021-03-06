package com.ais.damocles.spark.demo;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import com.datastax.spark.connector.japi.CassandraJavaUtil;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaStreamingContext;

import scala.Tuple2;

import com.ais.damocles.spark.schema.alltrade.RequestGoods;
import com.ais.damocles.spark.schema.alltrade.OrderTransfer;
import com.ais.damocles.spark.schema.alltrade.TransferSummaryReport;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraRow;

public class AlltradeTransferSummaryReport {

    private static final String DAMOCLES_KEYSPACE = "damocles";
    private static final String REQUESTGOODS_TABLE = "requestgoods";
    private static final String ORDERTRANSFER_TABLE = "ordertransfer";
    private static final String TRANSFERSUMMARYREPORT_TABLE = "transfersummary_report";


    public static void main(String[] args) throws Exception {

		/* read Spark and Cassandra properties and create SparkConf */
        Properties prop;
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
    private static void AggregateTransfer(JavaStreamingContext jssc) {

        JavaSparkContext sc = jssc.sparkContext();

		/* Load RequestGoods from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowRequestGoods = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE);
        JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
                .mapToPair(f -> {
                    RequestGoods requestGoods = new RequestGoods();

                    requestGoods.setRequestNo(f.getString(1));
                    requestGoods.setForSubStock(f.getString(17));
                    requestGoods.setShipToCode(f.getString(36));
                    requestGoods.setShipToName(f.getString(38));
                    requestGoods.setDoNo(f.getString(16));
                    requestGoods.setPickingDateTime(f.getString(27));

                    requestGoods.setCompany(f.getString(11));
                    requestGoods.setToLocationCode(f.getString(43));
                    requestGoods.setToLocationName(f.getString(44));
                    requestGoods.setCreateDateTime(f.getString(13));
                    requestGoods.setRequestStatus(f.getString(2));
                    requestGoods.setReservedNo(f.getString(32));
                    requestGoods.setBrand_key(f.getList(4));
                    requestGoods.setModel_key(f.getList(5));
                    requestGoods.setMatCode_key(f.getList(3)); /*3*/
                    /*requestGoods.setMatDescription_key(f.getString());*/
                    requestGoods.setCommercialName_key(f.getList(8));
                    requestGoods.setRemark(f.getString(31));
                    requestGoods.setShipToProvince(f.getString(39));
                    requestGoods.setCreateBy(f.getString(12));

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
                    OrderTransfer orderTransferIn = new OrderTransfer();
                    orderTransferIn.setCompany(f.getString(10));
                    orderTransferIn.setFromLocationCode(f.getString(16));
                    orderTransferIn.setFromLocationName(f.getString(17));
                    orderTransferIn.setToLocationCode(f.getString(31));
                    orderTransferIn.setToLocationName(f.getString(32));
                    orderTransferIn.setTransferDateTime(f.getString(38));
                    orderTransferIn.setTransferNo(f.getString(40));
                    orderTransferIn.setTransferStatus(f.getString(2));
                    orderTransferIn.setBrand_key(f.getList(4));
                    orderTransferIn.setModel_key(f.getList(5));
                    orderTransferIn.setModel_key(f.getList(3));
                    orderTransferIn.setFromSubStock(f.getString(22));
                    orderTransferIn.setToSubStock(f.getString(37));
                    orderTransferIn.setCreateBy(f.getString(11));
                    orderTransferIn.setUpdateBy(f.getString(41));

                    orderTransferIn.setTransactionType(f.getString(1));
                    orderTransferIn.setTransferDetail(f.getString(39));
                    orderTransferIn.setDocRef(f.getString(14));
                    return new Tuple2<>(orderTransferIn.getDocRef(), orderTransferIn);
                });

		/* Load TransferOut */
        JavaPairRDD<String, OrderTransfer> transferOutPairRDD = cassandraRowTransferOut
                .mapToPair(f -> {
                    OrderTransfer orderTransferOut = new OrderTransfer();
                    orderTransferOut.setCompany(f.getString(10));
                    orderTransferOut.setFromLocationCode(f.getString(16));
                    orderTransferOut.setFromLocationName(f.getString(17));
                    orderTransferOut.setToLocationCode(f.getString(31));
                    orderTransferOut.setToLocationName(f.getString(32));
                    orderTransferOut.setTransferDateTime(f.getString(38));
                    orderTransferOut.setTransferNo(f.getString(40));
                    orderTransferOut.setTransferStatus(f.getString(2));
                    orderTransferOut.setBrand_key(f.getList(4));
                    orderTransferOut.setModel_key(f.getList(5));
                    orderTransferOut.setMatCode_key(f.getList(3));
                    orderTransferOut.setFromSubStock(f.getString(22));
                    orderTransferOut.setToSubStock(f.getString(37));
                    orderTransferOut.setCreateBy(f.getString(11));
                    orderTransferOut.setUpdateBy(f.getString(41));

                    orderTransferOut.setTransactionType(f.getString(1));
                    orderTransferOut.setTransferDetail(f.getString(39));
                    orderTransferOut.setDocRef(f.getString(14));
                    return new Tuple2<>(orderTransferOut.getDocRef(), orderTransferOut);
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

		/* change key of transferOutRequestGoods */
        JavaPairRDD<String, Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>>> joinTransferOutRequestGoodsByDocRef = joinTransferRequestGoods
                .mapToPair(f -> new Tuple2<>(
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

                /*	f._1() = String
					f._2() = Tuple2<Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>>, com.google.common.base.Optional<OrderTransfer>>
					f._2()._1() = Tuple2<Tuple2<OrderTransfer, com.google.common.base.Optional<RequestGoods>> //join TransferOut and RequestGoods
					f._2()._1()._1() = OrderTransfer ////TransferOut
					f._2()._1()._2() = com.google.common.base.Optional<RequestGoods> //RequestGoods
					f._2()._2() = com.google.common.base.Optional<OrderTransfer //TransferIn */

            //String createdBy = f._2()._1()._2().isPresent() ? f._2()._1()._2().get().getCreateBy() : null;
            //String transferDetail = f._2()._2().isPresent() ? f._2()._2().get().getTransferDetail() : null;
            String transferInNo = f._2()._2().isPresent() ? f._2()._2().get().getTransferNo() : null;
            String transferindatetime = f._2()._2().isPresent() ? f._2()._2().get().getTransferDateTime() : null;

            System.out.println("key : " + f._1()+"\n"
                    + "company : " + f._2()._1()._1().getCompany()+"\n"
                    + "fromLocationCode : " + f._2()._1()._1().getFromLocationCode()+"\n"
                    + "fromLocationName :" + f._2()._1()._1().getFromLocationName()+"\n"
                    + "toLocationCode :" + f._2()._1()._1().getToLocationCode()+"\n"
                    + "toLocationName :" + f._2()._1()._1().getToLocationName()+"\n"
                    + "transferOutDateTime :" + f._2()._1()._1().getTransferDateTime()+"\n"
                    + "transferOutNo :" + f._2()._1()._1().getTransferNo()+"\n"
                    + "transferOutStatus :" + f._2()._1()._1().getTransferStatus()+"\n"
                    + "brand_key :" + f._2()._1()._1().getBrand_key()+"\n"
                    + "model_key :" + f._2()._1()._1().getModel_key()+"\n"
                    + "matCode_key :" + f._2()._1()._1().getMatCode_key()+"\n"
                    + "fromSubStock :" + f._2()._1()._1().getFromSubStock()+"\n"
                    + "toSubStock :" + f._2()._1()._1().getToSubStock()+"\n"
                    + "createBy :" + f._2()._1()._1().getCreateBy()+"\n"
                    + "updateBy :" + f._2()._1()._1().getUpdateBy()+"\n"

                    + "transferInDateTime : " + transferindatetime+"\n"
                    + "transferInNo : " + transferInNo+"\n"

                    + "requestNo : " + f._2()._1()._2().get().getRequestNo()+"\n"
                    + "forSubStock : " + f._2()._1()._2().get().getForSubStock()+"\n"
                    + "shipToCode : " + f._2()._1()._2().get().getShipToCode()+"\n"
                    + "shipToName : " + f._2()._1()._2().get().getShipToName()+"\n"
                    + "doNo : " + f._2()._1()._2().get().getDoNo()+"\n"
                    + "pickingDateTime : " + f._2()._1()._2().get().getPickingDateTime()+"\n" );
        });

            /*MapColumn schema to cassandra*/
        Map<String, String> columnNameMappings = new HashMap<>();

        columnNameMappings.put("company","company");
        columnNameMappings.put("fromLocationCode","fromlocationcode");
        columnNameMappings.put("fromLocationName","fromlocationname");
        columnNameMappings.put("toLocationCode","tolocationcode");
        columnNameMappings.put("toLocationName","tolocationname");
        columnNameMappings.put("toLocationName","tolocationname");
        columnNameMappings.put("transferOutDateTime","transferoutdatetime");
        columnNameMappings.put("transferOutNo","transferoutno");
        columnNameMappings.put("transferOutStatus","transferoutstatus");
        columnNameMappings.put("brand_key","brand_key");
        columnNameMappings.put("model_key","model_key");
        columnNameMappings.put("matCode_key","matcode_key");
        columnNameMappings.put("fromSubStock","fromsubstock");
        columnNameMappings.put("toSubStock","tosubstock");
        columnNameMappings.put("createBy","createby");
        columnNameMappings.put("updateBy","updateby");

        columnNameMappings.put("transferInDateTime","transferindatetime");
        columnNameMappings.put("transferInNo","transferinno");

        columnNameMappings.put("requestNo","requestno");
        columnNameMappings.put("forSubStock","forsubstock");
        columnNameMappings.put("shipToCode","shiptocode");
        columnNameMappings.put("shipToName","shiptoname");
        columnNameMappings.put("doNo","dono");
        columnNameMappings.put("pickingDateTime","pickingdatetime");

        /*insert data to cassandra*/
        JavaRDD<TransferSummaryReport> requestGoodsRDD = allAggregation
                .map(f -> {

                    /*Condition*/
                    //String createdBy = f._2()._1()._2().isPresent() ? f._2()._1()._2().get().getCreateBy() : null;
                    String transferInNo = f._2()._2().isPresent() ? f._2()._2().get().getTransferNo() : null;
                    String transferindatetime = f._2()._2().isPresent() ? f._2()._2().get().getTransferDateTime() : null;

                    TransferSummaryReport requestGoods = new TransferSummaryReport();
                    requestGoods.setCompany(f._2()._1()._1().getCompany());
                    requestGoods.setFromLocationCode(f._2()._1()._1().getFromLocationCode());
                    requestGoods.setFromLocationName(f._2()._1()._1().getFromLocationName());
                    requestGoods.setToLocationCode(f._2()._1()._1().getToLocationCode());
                    requestGoods.setToLocationName(f._2()._1()._1().getToLocationName());
                    requestGoods.setTransferOutDateTime(f._2()._1()._1().getTransferDateTime());
                    requestGoods.setTransferOutNo(f._2()._1()._1().getTransferNo());
                    requestGoods.setTransferOutStatus(f._2()._1()._1().getTransferStatus());
                    requestGoods.setBrand_key(f._2()._1()._1().getBrand_key());
                    requestGoods.setModel_key(f._2()._1()._1().getModel_key());
                    requestGoods.setMatCode_key(f._2()._1()._1().getMatCode_key());
                    requestGoods.setFromSubStock(f._2()._1()._1().getFromSubStock());
                    requestGoods.setToSubStock(f._2()._1()._1().getToSubStock());
                    requestGoods.setCreateBy(f._2()._1()._1().getCreateBy());
                    requestGoods.setUpdateBy(f._2()._1()._1().getUpdateBy());

                    requestGoods.setTransferInDateTime(transferindatetime);
                    requestGoods.setTransferInNo(transferInNo);

                    requestGoods.setRequestNo(f._2()._1()._2().get().getRequestNo());
                    requestGoods.setForSubStock(f._2()._1()._2().get().getForSubStock());
                    requestGoods.setShipToCode(f._2()._1()._2().get().getShipToCode());
                    requestGoods.setShipToName(f._2()._1()._2().get().getShipToName());
                    requestGoods.setDoNo(f._2()._1()._2().get().getDoNo());
                    requestGoods.setPickingDateTime(f._2()._1()._2().get().getPickingDateTime());
                    return requestGoods;
                });

        /* show insert data to cassandra */
        System.out.println("===== insert data to cassandra =====");
        requestGoodsRDD.foreach(f -> System.out.println("RequestNo: " + f.getRequestNo()+"\n"
                + "transferOutNo : " + f.getTransferOutNo()+"\n"
                + "transferOutDateTime : " + f.getTransferOutDateTime()+"\n"
                + "createBy : " + f.getCreateBy()+"\n"
                + "company : " + f.getCompany()+"\n"
                + "transferInNo : " + f.getTransferInNo()+"\n"
                + "transferInDateTime : " + f.getTransferInDateTime()+"\n"
        ));
        javaFunctions(requestGoodsRDD).writerBuilder(
                DAMOCLES_KEYSPACE,
                TRANSFERSUMMARYREPORT_TABLE,
                CassandraJavaUtil.mapToRow(TransferSummaryReport.class,
                        columnNameMappings)).saveToCassandra();

    }
}