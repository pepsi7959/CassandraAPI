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
import com.ais.damocles.spark.schema.alltrade.RequestGoodsDetailReport;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraRow;


public class AlltradeRequestGoodsDetailReport {

    public static final String DAMOCLES_KEYSPACE = "damocles";
    public static final String REQUESTGOODS_TABLE = "requestgoods";
    public static final String ORDERTRANSFER_TABLE = "ordertransfer";
    public static final String REQUESTGOODSDETAIL_TABLE = "requestgoodsdetail_report";

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

        JavaSparkContext sc = jssc.sparkContext();

		/* Load RequestGoods from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowRequestGoods = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE);

        JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
                .mapToPair(f -> {

                    RequestGoods requestGoods = new RequestGoods();
                    requestGoods.setCompany(f.getString(11));
                    requestGoods.setToLocationCode(f.getString(37));
                    requestGoods.setToLocationName(f.getString(38));
                    requestGoods.setForSubStock(f.getString(15));
                    requestGoods.setCreateDateTime(f.getString(13));
                    requestGoods.setRequestNo(f.getString(1));
                    requestGoods.setRequestStatus(f.getString(2));
                    requestGoods.setReservedNo(f.getString(28));
                    requestGoods.setDoNo(f.getString(14));
                    requestGoods.setPickingDateTime(f.getString(24));
                    /*requestGoods.setQuotaFlag(f.getString());*/
                    /*requestGoods.setItemNo(f.getString());*/
                    requestGoods.setBrand_key(f.getList(4));
                    requestGoods.setModel_key(f.getList(5));
                    requestGoods.setMatCode_key(f.getList(3)); /*3*/
                    /*requestGoods.setMatDescription_key(f.getString());*/
                    requestGoods.setCommercialName_key(f.getList(8));
                    requestGoods.setRemark(f.getString(27));
                    requestGoods.setShipToCode(f.getString(32));
                    requestGoods.setShipToProvince(f.getString(35));
                    return new Tuple2<>(requestGoods.getRequestNo(), requestGoods);
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
                    orderTransferIn.setTransferDateTime(f.getString(38));
                    orderTransferIn.setCreateBy(f.getString(11));
                    orderTransferIn.setTransferNo(f.getString(40));
                    orderTransferIn.setDocRef(f.getString(14));
                    orderTransferIn.setTransactionType(f.getString(1));
                    orderTransferIn.setTransferDetail(f.getString(39));
                    return new Tuple2<>(orderTransferIn.getDocRef(), orderTransferIn);
                });

		/* Load TransferOut */
        JavaPairRDD<String, OrderTransfer> transferOutPairRDD = cassandraRowTransferOut
                .mapToPair(f -> {
                    OrderTransfer orderTransferOut = new OrderTransfer();
                    orderTransferOut.setTransferDateTime(f.getString(38));
                    orderTransferOut.setCreateBy(f.getString(11));
                    orderTransferOut.setTransferNo(f.getString(40));
                    orderTransferOut.setDocRef(f.getString(14));
                    orderTransferOut.setTransactionType(f.getString(1));
                    orderTransferOut.setTransferDetail(f.getString(39));
                    orderTransferOut.setMmDocNo(f.getString(23));
                    orderTransferOut.setCreateDateTime(f.getString(12));
                    return new Tuple2<>(orderTransferOut.getDocRef(), orderTransferOut);
                });

		/* show Request Goods */
        System.out.println("===== Request Goods =====");
        requestGoodsPairRDD.foreach(f -> System.out.println("RequestNo : "
                + f._1()));

		/* show TransferOut */
        System.out.println("===== TransferOut =====");
        transferOutPairRDD.foreach(f -> System.out.println("TransferNo : "
                + f._1()));

		/* show TransferIn */
        System.out.println("===== TransferIn =====");
        transferInPairRDD.foreach(f -> System.out.println("TransferNo : "
                + f._1()));

        /* join RequestGoods and TransferOut  */
        JavaPairRDD<String, Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>> joinTransferRequestGoods = requestGoodsPairRDD
                .leftOuterJoin(transferOutPairRDD);

        System.out.println("===== Join RequestGoods and TransferOut  =====");
        joinTransferRequestGoods.foreach(f -> {
            try {
                System.out.println("Key : " + f._1() + "CreatedBy : "
                        + f._2()._2().get().getCreateBy());
            } catch (IllegalStateException ex) {
                System.out.println(ex.toString());
            }
        });

        /* change key of RequestGoodsTransferOut */
        JavaPairRDD<String, Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>> joinTransferOutRequestGoodsByDocRef = joinTransferRequestGoods
                .mapToPair(f -> new Tuple2<>(
                        f._2()._2().get().getTransferNo(), f._2()));

        System.out.println("===== Change Key of RequestGoodsTransferOut =====");
        joinTransferOutRequestGoodsByDocRef.foreach(f -> {
            try {
                System.out.println("Key : " + f._1() + "CreatedBy : "
                        + f._2()._2().get().getCreateBy());
            } catch (Exception ex) {
                System.out.println(ex);
            }
        });

        /* join RequestGoodsTransferOut and TransferIn */
        JavaPairRDD<String, Tuple2<Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>, com.google.common.base.Optional<OrderTransfer>>>
                allAggregation = joinTransferOutRequestGoodsByDocRef.leftOuterJoin(transferInPairRDD);

        System.out.println("======== All aggregation ========");
        allAggregation.foreach(f -> {

            /*	f._1() = String
                    f._2() = Tuple2<Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>, com.google.common.base.Optional<OrderTransfer>>>
					f._2()._1() = Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>  //join RequestGoods and TransferOut
					f._2()._1()._1() = RequestGoods //requestGoods
					f._2()._1()._2() = com.google.common.base.Optional<OrderTransfer> //TransferOut
					f._2()._2() = com.google.common.base.Optional<OrderTransfer> //TransferIn */

            /*Condition*/
            String createdBy = f._2()._1()._2().isPresent() ? f._2()._1()._2().get().getCreateBy() : null;
            String transferInNo = f._2()._2().isPresent() ? f._2()._2().get().getTransferNo() : null;
            String transferindatetime = f._2()._2().isPresent() ? f._2()._2().get().getTransferDateTime() : null;

            System.out.println("key : " + f._1()
                    + "TransferNo : " + f._2()._1()._2().get().getTransferNo() + "\n"
                    + "RequestNo : " + f._2()._1()._1().getRequestNo() + "\n"
                    + "Company : " + f._2()._1()._1().getCompany() + "\n"
                    + "ToLocationCode : " + f._2()._1()._1().getToLocationCode() + "\n"
                    + "ToLocationName : " + f._2()._1()._1().getToLocationName() + "\n"
                    + "ForSubStock : " + f._2()._1()._1().getForSubStock() + "\n"
                    + "CreateDateTime : " + f._2()._1()._1().getCreateDateTime() + "\n"
                    + "RequestStatus : " + f._2()._1()._1().getRequestStatus() + "\n"
                    + "ReservedNo : " + f._2()._1()._1().getReservedNo() + "\n"
                    + "DoNo : " + f._2()._1()._1().getDoNo() + "\n"
                    + "PickingDateTime : " + f._2()._1()._1().getPickingDateTime() + "\n"
                    + "TransferOutNo : " + f._2()._1()._2().get().getTransferNo() + "\n"
                    + "TransferOutDateTime : " + f._2()._1()._2().get().getTransferDateTime() + "\n"
                    + "CreateBy : " + createdBy + "\n"
                    + "TransferInNo : " + transferInNo + "\n"
                    + "TransferInDateTime : " + transferindatetime + "\n"
                    /* + "QuotaFlag : " + f._2()._1()._1().getQuotaFlag()+"\n"*/
                    /*+ "ItemNo : " + f._2()._1()._1().getItemNo()+"\n"*/
                    + "Brand_key : " + f._2()._1()._1().getBrand_key() + "\n"
                    + "Model : " + f._2()._1()._1().getModel_key() + "\n"
                    + "MatCode_key : " + f._2()._1()._1().getMatCode_key() + "\n"
                    /*+ "MatDescription_key : " + f._2()._1()._1().getMatDescription_key()+"\n"*/
                    + "CommercialName_key : " + f._2()._1()._1().getCommercialName_key() + "\n"
                    + "Remark : " + f._2()._1()._1().getRemark() + "\n"
                    + "ShipToCode : " + f._2()._1()._1().getShipToCode() + "\n"
                    + "ShipToProvince : " + f._2()._1()._1().getShipToProvince());
        });

        /*MapColumn schema to cassandra*/
        Map<String, String> columnNameMappings = new HashMap<String, String>();

        columnNameMappings.put("requestNo", "requestno");
        columnNameMappings.put("company", "company");
        columnNameMappings.put("toLocationCode", "tolocationcode");
        columnNameMappings.put("toLocationName", "tolocationname");
        columnNameMappings.put("forSubStock", "forsubstock");
        columnNameMappings.put("createDateTime", "createdatetime");
        columnNameMappings.put("requestStatus", "requeststatus");
        columnNameMappings.put("reservedNo", "reservedno");
        columnNameMappings.put("doNo", "dono");
        columnNameMappings.put("pickingDateTime", "pickingdatetime");
        columnNameMappings.put("transferOutNo", "transferoutno");
        columnNameMappings.put("transferOutDateTime", "transferoutdatetime");
        columnNameMappings.put("createBy", "createby");
        columnNameMappings.put("transferInNo", "transferinno");
        columnNameMappings.put("transferindatetime", "transferindatetime");
        columnNameMappings.put("matCode_key", "matcode_key");
        columnNameMappings.put("brand_key", "brand_key");
        columnNameMappings.put("model_key", "model_key");
        columnNameMappings.put("commercialName_key", "commercialname_key");
        columnNameMappings.put("remark", "remark");
        columnNameMappings.put("shipToCode", "shiptocode");
        columnNameMappings.put("shipToProvince", "shiptoprovince");


        /*insert data to cassandra*/
        JavaRDD<RequestGoodsDetailReport> requestGoodsRDD = allAggregation
                .map(f -> {

                    /*Condition*/
                    String createdBy = f._2()._1()._2().isPresent() ? f._2()._1()._2().get().getCreateBy() : null;
                    String transferInNo = f._2()._2().isPresent() ? f._2()._2().get().getTransferNo() : null;
                    String transferindatetime = f._2()._2().isPresent() ? f._2()._2().get().getTransferDateTime() : null;


                    RequestGoodsDetailReport requestGoods = new RequestGoodsDetailReport();

                    requestGoods.setRequestNo(f._2()._1()._1().getRequestNo());
                    requestGoods.setCompany(f._2()._1()._1().getCompany());
                    requestGoods.setToLocationCode(f._2()._1()._1().getToLocationCode());
                    requestGoods.setToLocationName(f._2()._1()._1().getToLocationName());
                    requestGoods.setForSubStock(f._2()._1()._1().getForSubStock());
                    requestGoods.setCreateDateTime( f._2()._1()._1().getCreateDateTime());
                    requestGoods.setRequestStatus(f._2()._1()._1().getRequestStatus());
                    requestGoods.setReservedNo(f._2()._1()._1().getReservedNo());
                    requestGoods.setDoNo(f._2()._1()._1().getDoNo());
                    requestGoods.setPickingDateTime(f._2()._1()._1().getPickingDateTime());
                    requestGoods.setTransferOutNo(f._2()._1()._2().get().getTransferNo());
                    requestGoods.setTransferOutDateTime(f._2()._1()._2().get().getTransferDateTime());
                    requestGoods.setCreateBy(createdBy);
                    requestGoods.setTransferInNo(transferInNo);
                    requestGoods.setTransferindatetime(transferindatetime);
                    requestGoods.setMatCode_key(f._2()._1()._1().getMatCode_key());
                    requestGoods.setBrand_key(f._2()._1()._1().getBrand_key());
                    requestGoods.setModel_key(f._2()._1()._1().getModel_key());
                    requestGoods.setCommercialName_key(f._2()._1()._1().getCommercialName_key());
                    requestGoods.setRemark(f._2()._1()._1().getRemark());
                    requestGoods.setShipToCode(f._2()._1()._1().getShipToCode());
                    requestGoods.setShipToProvince(f._2()._1()._1().getShipToProvince());

                    return requestGoods;
                });

        /* show insert data to cassandra */
        System.out.println("===== insert data to cassandra =====");
        requestGoodsRDD.foreach(f -> System.out.println("RequestNo: " + f.getRequestNo()
                /*+ "transferOutNo : " + f.getTransferOutNo()*/
                        + "transferInNo : " + f.getTransferInNo()
                        + "TransferInDateTime : " + f.getTransferindatetime()
                /*+ "createBy : " + f.getCreateBy()
                + "matCode_key : " + f.getMatCode_key()*/
        ));

        javaFunctions(requestGoodsRDD).writerBuilder(
                DAMOCLES_KEYSPACE,
                REQUESTGOODSDETAIL_TABLE,
                CassandraJavaUtil.mapToRow(RequestGoodsDetailReport.class,
                        columnNameMappings)).saveToCassandra();
    }
}
