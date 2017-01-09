package com.ais.damocles.spark.demo;

import com.ais.damocles.spark.schema.alltrade.*;
import com.ais.damocles.spark.schema.alltrade.TransferDetailReport;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraJavaUtil;
import com.datastax.spark.connector.japi.CassandraRow;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaStreamingContext;
import scala.Tuple2;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

public class AlltradeTransferDetailReport {

    public static JavaSparkContext sc;
    public static final String DAMOCLES_KEYSPACE = "damocles11";
    public static final String REQUESTGOODS_TABLE = "requestgoods";
    public static final String ORDERTRANSFER_TABLE = "ordertransfer";
    public static final String TRANSFERDETAIL_TABLE = "transferdetail_report";

    public static void main(String[] args) throws Exception {

		/* read Spark and Cassandra properties and create SparkConf */
        Properties prop;
        if (args.length == 0)
            prop = PropertyFileReader.readPropertyFile();
        else if (args.length == 1)
            prop = PropertyFileReader.readPropertyFile(args[0]);
        else
            prop = PropertyFileReader.readPropertyFile();

        SparkConf conf = new SparkConf()
                .setAppName(prop.getProperty("com.ais.damocles.spark.name"))
                .setMaster(prop.getProperty("com.ais.damocles.spark.master"))
                .set("spark.cassandra.connection.host"
                    , prop.getProperty("com.ais.damocles.cassandra.host"))
                .set("spark.cassandra.connection.port"
                    , prop.getProperty("com.ais.damocles.cassandra.port"))
                .set("spark.cassandra.connection.keep_alive_ms"
                    , prop.getProperty("com.ais.damocles.cassandra.keep_alive"));

		/* batch interval of 5 seconds for incoming stream */
        JavaStreamingContext jssc = new JavaStreamingContext(conf,
                Durations.seconds(5));

		/* add check point directory */
        jssc.checkpoint(prop.getProperty("com.ais.damocles.spark.checkpoint.dir"));

        AggregateTransfer(jssc);

        //jssc.start();
        //System.out.println("START JAVA STREAMING CONTEXT");
        //jssc.awaitTermination();
        //System.out.println("TERMINATE JAVA STREAMING CONTEXT");
    }

    /* Aggregate Transfer */
    private static void AggregateTransfer(JavaStreamingContext jssc) {

        sc = jssc.sparkContext();

        /* Load RequestGoods from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowRequestGoods = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE);

        JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
                .mapToPair(f -> {
                    RequestGoods requestGoods = new RequestGoods();

//                    requestGoods.setRequestDateTime(f.getString("null"));
                    requestGoods.setRequestNo(f.getString(1));
                    requestGoods.setForSubStock(f.getString(15));

//                    requestGoods.setContactNo(f.getString("null"));

                    return new Tuple2<>(requestGoods.getRequestNo(), requestGoods);
                });

        cassandraRowRequestGoods.foreach(f ->
                System.out.println("column 1 : " + f.getString(0) + "\n"));

        /* Load OrderTransferIn from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowTransferIn = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE)
                .where("transactionType=?", "TransferIn");

        JavaPairRDD<String, OrderTransfer> transferInPairRDD = cassandraRowTransferIn
                .mapToPair(f -> {
                    OrderTransfer orderTransferIn = new OrderTransfer();

                    orderTransferIn.setTransferDateTime(f.getString(38));
                    orderTransferIn.setTransferNo(f.getString(40));

//                    orderTransferIn.setQtyIn(f.getString("null"));

//                    orderTransferIn.setReceiveBy(f.getString("null"));

                    orderTransferIn.setDocRef(f.getString(14));
                    orderTransferIn.setTransactionType(f.getString(1));
                    orderTransferIn.setTransferDetail(f.getString(39));

                    return new Tuple2<>(orderTransferIn.getDocRef(), orderTransferIn);
                });

        /* Load OrderTransferOut from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowTransferOut = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE)
                .where("transactionType=?", "TransferOut");

        /* Load TransferOut Row*/
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
//                    orderTransferOut.setItem(f.getString("null"));
//                    orderTransferOut.setCommercialName_key(f.getString("null"));
                    orderTransferOut.setMatCode_key(f.getList(3));
//                    orderTransferOut.setMatDescription(f.getString("null"));
//                    orderTransferOut.setMatType(f.getString("null"));
                    orderTransferOut.setFromSubStock(f.getString(22));
                    orderTransferOut.setToSubStock(f.getString(37));
//                    orderTransferOut.setSerialNo(f.getString("null"));
//                    orderTransferOut.setQtyOut(f.getString("null"));

                    orderTransferOut.setCreateBy(f.getString(11));
                    orderTransferOut.setUpdateBy(f.getString(41));
//                    orderTransferOut.setReason(f.getString("null"));

                    orderTransferOut.setRemark(f.getString(26));
                    orderTransferOut.setVendorCode(f.getString(43));
                    orderTransferOut.setVendorName(f.getString(44));

                    orderTransferOut.setDocRef(f.getString(14));
                    orderTransferOut.setTransactionType(f.getString(1));
                    orderTransferOut.setTransferDetail(f.getString(39));

                    return new Tuple2<>(orderTransferOut.getDocRef(), orderTransferOut);
                });

        /* show Request Goods */
        System.out.println("===== Request Goods =====");
        requestGoodsPairRDD.foreach(f ->
                System.out.println("RequestNo : " + f._1() + "\n"));

		/* show TransferOut */
        System.out.println("===== TransferOut =====");
        transferOutPairRDD.foreach(f ->
                System.out.println("TransferNo : " + f._1() + "\n"));

		/* show TransferIn */
        System.out.println("===== TransferIn =====");
        transferInPairRDD.foreach(f ->
                System.out.println("TransferNo : " + f._1() + "\n") );

        /* join RequestGoods and TransferOut  */
        JavaPairRDD<String, Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>>
                joinTransferRequestGoods = requestGoodsPairRDD.leftOuterJoin(transferOutPairRDD);

        System.out.println("===== Join RequestGoods and TransferOut  =====");
        joinTransferRequestGoods.foreach(f -> {
            try {
                System.out.println("Key : " + f._1() + " CreatedBy : "
                        + f._2()._2().get().getCreateBy() + "\n");
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
                System.out.println("Key : " + f._1() + " CreatedBy : "
                        + f._2()._2().get().getCreateBy() + "\n");
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
                    f._2() = Tuple2<Tuple2<RequestGoods, com.google.common.base
                    .Optional<OrderTransfer>>, com.google.common.base.Optional<OrderTransfer>>>
					f._2()._1() = Tuple2<RequestGoods, com.google.common.base
					.Optional<OrderTransfer>>  //join RequestGoods and TransferOut
					f._2()._1()._1() = RequestGoods //requestGoods
					f._2()._1()._2() = com.google.common.base.Optional<OrderTransfer> //TransferOut
					f._2()._2() = com.google.common.base.Optional<OrderTransfer> //TransferIn */

        /*Condition*/
            String createdBy = f._2()._1()._2().isPresent() ?
                    f._2()._1()._2().get().getCreateBy() : null;
            String transferInNo = f._2()._2().isPresent() ?
                    f._2()._2().get().getTransferNo() : null;
            String transferInDateTime = f._2()._2().isPresent() ?
                    f._2()._2().get().getTransferDateTime() : null;

            System.out.println("key : " + f._1()
                    + "Company : " + f._2()._1()._2().get().getCompany() + "\n"
                    + "From Location Code : " + f._2()._1()._2().get().getFromLocationCode() + "\n"
                    + "From Location Name : " + f._2()._1()._2().get().getFromLocationName() + "\n"
                    + "To Location Code : " + f._2()._1()._2().get().getToLocationCode() + "\n"
                    + "To Location Name : " + f._2()._1()._2().get().getToLocationName() + "\n"
                    + "Transfer Out Date Time : " + f._2()._1()._2().get().getTransferDateTime() + "\n"
                    + "Transfer Out No : " + f._2()._1()._2().get().getTransferNo() + "\n"
                    + "Transfer In DateTime : " + transferInDateTime + "\n"
                    + "Transfer In No : " + transferInNo + "\n"
//                    + "Request Date Time : " + f._2()._1()._1().getRequestDateTime() + "\n"
                    + "Request No : " + f._2()._1()._1().getRequestNo() + "\n"
                    + "For Sub Stock : " + f._2()._1()._1().getForSubStock() + "\n"
//                    + "Transfer Out Status : " + f._2()._1()._2().get().getTransferOutStatus() + "\n"
                    + "Brand : " + f._2()._1()._2().get().getBrand_key() + "\n"
                    + "Model : " + f._2()._1()._2().get().getModel_key() + "\n"
//                    + "Item : " + f._2()._1()._2().get().getItem() + "\n"
//                    + "Commercial Name : " + f._2()._1()._2().get().getCommercialName_key() + "\n"
                    + "Mat Code : " + f._2()._1()._2().get().getMatCode_key() + "\n"
//                    + "Mat Description : " + f._2()._1()._2().get().getMatDescription() + "\n"
//                    + "Mat Type : " + f._2()._1()._2().get().getMatType() + "\n"
                    + "From Sub Stock : " + f._2()._1()._2().get().getFromSubStock() + "\n"
                    + "To Sub Stock : " + f._2()._1()._2().get().getToSubStock() + "\n"

//                    + "Serial No : " + f._2()._1()._2().get().getSerialNo() + "\n"
//                    + "Qty Out : " + f._2()._1()._2().get().getQtyOut() + "\n"
//                    + "Qty In : " + f._2()._2().get().getQtyIn() + "\n"
                    + "Create By : " + createdBy + "\n"
                    + "Update By : " + f._2()._1()._2().get().getUpdateBy() + "\n"
//                    + "Reason : " + f._2()._1()._2().get().getReason() + "\n"
//                    + "Receive By : " + f._2()._2().get().getReceiveBy() + "\n"
//                    + "Contact No : " + f._2()._1()._1().getContactNo() + "\n"
                    + "Remark : " + f._2()._1()._2().get().getRemark() + "\n"
                    + "Vendor Code : " + f._2()._1()._2().get().getVendorCode() + "\n"
                    + "Vendor Name : " + f._2()._1()._2().get().getVendorName() + "\n"
            );
        });

        /*MapColumn schema to cassandra*/
        Map<String, String> columnNameMappings = new HashMap<String, String>();
        columnNameMappings.put("company", "company");
        columnNameMappings.put("fromLocationCode", "fromlocationcode");
        columnNameMappings.put("fromLocationName", "fromlocationname");
        columnNameMappings.put("toLocationCode", "tolocationcode");
        columnNameMappings.put("toLocationName", "tolocationname");
        columnNameMappings.put("transferOutDateTime", "transferoutdatetime");
        columnNameMappings.put("transferOutNo", "transferoutno");
        columnNameMappings.put("transferInDateTime", "transferindatetime");
        columnNameMappings.put("transferInNo", "transferinno");
        columnNameMappings.put("requestNo", "requestno");
        columnNameMappings.put("forSubStock", "forsubstock");
        columnNameMappings.put("brand_key", "brand_key");
        columnNameMappings.put("model_key", "model_key");
        //columnNameMappings.put("commercialName_key" , "commercialName_key");
        columnNameMappings.put("matCode_key", "matcode_key");
        columnNameMappings.put("fromSubStock", "fromsubstock");
        columnNameMappings.put("toSubStock", "tosubstock");
        columnNameMappings.put("createBy", "createby");
        columnNameMappings.put("updateBy", "updateby");
        columnNameMappings.put("remark", "remark");
        columnNameMappings.put("vendorCode", "vendorcode");
        columnNameMappings.put("vendorName", "vendorname");




        /*insert data to cassandra*/
        JavaRDD<TransferDetailReport> orderTransferRDD = allAggregation
                .map(f -> {

                    /*Condition*/
                    String createdBy = f._2()._1()._2().isPresent() ? f._2()._1()._2().get().getCreateBy() : "null";
                    String transferInNo = f._2()._2().isPresent() ? f._2()._2().get().getTransferNo() : "null";
                    String transferInDateTime = f._2()._2().isPresent() ? f._2()._2().get().getTransferDateTime() : null ;


                    TransferDetailReport orderTransfer = new TransferDetailReport();

                    orderTransfer.setCompany(f._2()._1()._2().get().getCompany());
                    orderTransfer.setFromLocationCode(f._2()._1()._2().get().getFromLocationCode());
                    orderTransfer.setFromLocationName(f._2()._1()._2().get().getFromLocationName());
                    orderTransfer.setToLocationCode(f._2()._1()._2().get().getToLocationCode());
                    orderTransfer.setToLocationName(f._2()._1()._2().get().getToLocationName());
                    orderTransfer.setTransferOutDateTime(f._2()._1()._2().get().getTransferDateTime());
                    orderTransfer.setTransferOutNo(f._2()._1()._2().get().getTransferNo());
                    orderTransfer.setTransferInDateTime(transferInDateTime);
                    orderTransfer.setTransferInNo(transferInNo);
                    orderTransfer.setRequestNo(f._2()._1()._1().getRequestNo());
                    orderTransfer.setForSubStock(f._2()._1()._1().getForSubStock());
                    orderTransfer.setBrand_key(f._2()._1()._2().get().getBrand_key());
                    orderTransfer.setModel_key(f._2()._1()._2().get().getModel_key());
                    orderTransfer.setMatCode_key(f._2()._1()._2().get().getMatCode_key());
                    orderTransfer.setFromSubStock(f._2()._1()._2().get().getFromSubStock());
                    orderTransfer.setToSubStock(f._2()._1()._2().get().getToSubStock());
                    orderTransfer.setCreateBy(createdBy);
                    orderTransfer.setUpdateBy(f._2()._1()._2().get().getUpdateBy());
                    orderTransfer.setRemark(f._2()._1()._2().get().getRemark());
                    orderTransfer.setVendorCode(f._2()._1()._2().get().getVendorCode());
                    orderTransfer.setVendorName(f._2()._1()._2().get().getVendorName());

                    return orderTransfer;
                });

        /* show insert data to cassandra */
        System.out.println("===== insert data to cassandra =====");
        orderTransferRDD.foreach(f -> System.out.println(
                  "RequestNo: " + f.getRequestNo() + "\n"
                + "TransferOutNo : " + f.getTransferOutNo() + "\n"
                + "TransferInNo : " + f.getTransferInNo() + "\n"
                + "createBy : " + f.getCreateBy() + "\n"
                + "matCode_key : " + f.getMatCode_key()
        ));

        javaFunctions(orderTransferRDD).writerBuilder(
                DAMOCLES_KEYSPACE, TRANSFERDETAIL_TABLE,
                CassandraJavaUtil.mapToRow(TransferDetailReport.class,
                        columnNameMappings)).saveToCassandra();
    }
}
