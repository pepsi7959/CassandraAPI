package com.ais.damocles.spark.demo;

import com.ais.damocles.spark.schema.alltrade.OrderTransfer;
import com.ais.damocles.spark.schema.alltrade.RequestGoods;
import com.ais.damocles.spark.schema.alltrade.RequestGoodsDetailReport;
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

public class TransferDetailReport {

    private static final String DAMOCLES_KEYSPACE = "damocles11";
    private static final String REQUESTGOODS_TABLE = "requestgoods";
    private static final String ORDERTRANSFER_TABLE = "ordertransfer";

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

        jssc.start();
        System.out.println("START JAVA STREAMING CONTEXT");
        jssc.awaitTermination();
        System.out.println("TERMINATE JAVA STREAMING CONTEXT");
    }

    /* Aggregate Transfer */
    private static void AggregateTransfer(JavaStreamingContext jssc) {

        JavaSparkContext sc = jssc.sparkContext();


        /* Load OrderTransferOut from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowTransferOut = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE)
                .where("transactionType=?", "TransferOut");

		/* Load OrderTransferIn from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowTransferIn = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, ORDERTRANSFER_TABLE)
                .where("transactionType=?", "TransferIn");

        /* Load RequestGoods from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowRequestGoods = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE);

        /* Load TransferOut Row*/
        JavaPairRDD<String, OrderTransfer> transferOutPairRDD = cassandraRowTransferOut
                .mapToPair(f -> {
                    OrderTransfer orderTransferOut = new OrderTransfer();

                    orderTransferOut.setCompany(f.getString(11));
                    orderTransferOut.setFromLocationCode(f.getString(17));
                    orderTransferOut.setFromLocationName(f.getString(18));
                    orderTransferOut.setToLocationCode(f.getString(32));
                    orderTransferOut.setToLocationName(f.getString(33));
                    orderTransferOut.setTransferDateTime(f.getString(39));
                    orderTransferOut.setTransferNo(f.getString(41));

                    orderTransferOut.setTransferStatus(f.getString(3));
                    orderTransferOut.setBrand_key(f.getString(5));
                    orderTransferOut.setModel_key(f.getString(6));
//                    orderTransferOut.setItem(f.getString("null"));
//                    orderTransferOut.setCommercialName(f.getString("null"));
//                    orderTransferOut.setMatCode_key(f.getString("null"));
//                    orderTransferOut.setMatDescription(f.getString("null"));
//                    orderTransferOut.setMatType(f.getString("null"));
                    orderTransferOut.setFromSubStock(f.getString(23));
                    orderTransferOut.setToSubStock(f.getString(38));
//                    orderTransferOut.setSerialNo(f.getString("null"));
//                    orderTransferOut.setQtyOut(f.getString("null"));

                    orderTransferOut.setCreateBy(f.getString(12));
                    orderTransferOut.setUpdateBy(f.getString(42));
//                    orderTransferOut.setReason(f.getString("null"));

                    orderTransferOut.setRemark(f.getString(27));
                    orderTransferOut.setVendorCode(f.getString(44));
                    orderTransferOut.setVendorName(f.getString(45));

                    return new Tuple2<>(orderTransferOut.getDocRef(), orderTransferOut);
                });

		/* Load TransferIn Row*/
        JavaPairRDD<String, OrderTransfer> transferInPairRDD = cassandraRowTransferIn
                .mapToPair(f -> {
                    OrderTransfer orderTransferIn = new OrderTransfer();

                    orderTransferIn.setTransferDateTime(f.getString(39));
                    orderTransferIn.setTransferNo(f.getString(41));

//                    orderTransferIn.setQtyIn(f.getString("null"));

//                    orderTransferIn.setReceiveBy(f.getString("null"));

                    return new Tuple2<>(orderTransferIn.getDocRef(), orderTransferIn);
                });

        /* Load RequestGoods Row*/
        JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
                .mapToPair(f -> {
                    RequestGoods requestGoods = new RequestGoods();

//                    requestGoods.setRequestDateTime(f.getString("null"));
                    requestGoods.setRequestNo(f.getString(2));
                    requestGoods.setForSubStock(f.getString(16));

//                    requestGoods.setContactNo(f.getString("null"));

                    return new Tuple2<>(requestGoods.getRequestNo(), requestGoods);
                });

        cassandraRowRequestGoods.foreach(f ->
                System.out.println("column 1 : " + f.getString(0)));

        /* show Request Goods */
        System.out.println("===== Request Goods =====");
        requestGoodsPairRDD.foreach(f ->
                System.out.println("RequestNo : " + f._1()));

		/* show TransferOut */
        System.out.println("===== TransferOut =====");
        transferOutPairRDD.foreach(f ->
                System.out.println("TransferNo : " + f._1()));

		/* show TransferIn */
        System.out.println("===== TransferIn =====");
        transferInPairRDD.foreach(f ->
                System.out.println("TransferNo : " + f._1()));

        /* join RequestGoods and TransferOut  */
        JavaPairRDD<String, Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>>
                joinTransferRequestGoods = requestGoodsPairRDD.leftOuterJoin(transferOutPairRDD);

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
        JavaPairRDD<String, Tuple2<RequestGoods, com.google.common.base.Optional<OrderTransfer>>>
                joinTransferOutRequestGoodsByDocRef = joinTransferRequestGoods
                .mapToPair(f ->
                        new Tuple2<>(f._2()._2().get().getTransferNo(), f._2()));

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

            System.out.println(
                      "key : " + f._1()
                    + "Company : " + f._2()._1()._2().get().getCompany() + "\n"
                    + "From Location Code : " + f._2()._1()._2().get().getFromLocationCode() + "\n"
                    + "From Location Name : " + f._2()._1()._2().get().getFromLocationName() + "\n"
                    + "To Location Code : " + f._2()._1()._2().get().getToLocationCode() + "\n"
                    + "To Location Name : " + f._2()._1()._2().get().getToLocationName() + "\n"
                    + "Transfer Out Date Time : " + f._2()._1()._2().get().getTransferDateTime() + "\n"
                    + "Transfer Out No : " + f._2()._1()._2().get().getTransferNo() + "\n"
                    + "Transfer In DateTime : " + transferInDateTime + "\n"
                    + "Transfer In No : " + transferInNo + "\n"
                    + "Request Date Time : " + f._2()._1()._1().getRequestDateTime() + "\n"
                    + "Request No : " + f._2()._1()._1().getRequestNo() + "\n"
                    + "For Sub Stock : " + f._2()._1()._1().getForSubStock() + "\n"
                    + "Transfer Out Status : " + f._2()._1()._2().get().getTransferOutStatus() + "\n"
                    + "Brand : " + f._2()._1()._2().get().getBrand_key() + "\n"
                    + "Model : " + f._2()._1()._2().get().getModel_key() + "\n"
                    + "Item : " + f._2()._1()._2().get().getItem() + "\n"
                    + "Commercial Name : " + f._2()._1()._2().get().getCommercialName() + "\n"
                    + "Mat Code : " + f._2()._1()._2().get().getMatCode_key() + "\n"
                    + "Mat Description : " + f._2()._1()._2().get().getMatDescription() + "\n"
                    + "Mat Type : " + f._2()._1()._2().get().getMatType() + "\n"
                    + "From Sub Stock : " + f._2()._1()._2().get().getFromSubStock() + "\n"
                    + "Serial No : " + f._2()._1()._2().get().getSerialNo() + "\n"
                    + "Qty Out : " + f._2()._1()._2().get().getQtyOut() + "\n"
                    + "Qty In : " + f._2()._2().get().getQtyIn() + "\n"
                    + "Create By : " + createdBy + "\n"
                    + "Update By : " + f._2()._1()._2().get().getUpdateBy() + "\n"
                    + "Reason : " + f._2()._1()._2().get().getReason() + "\n"
                    + "Receive By : " + f._2()._2().get().getReceiveBy() + "\n"
                    + "Contact No : " + f._2()._1()._1().getContactNo() + "\n"
                    + "Remark : " + f._2()._1()._2().get().getRemark() + "\n"
                    + "Vendor Code : " + f._2()._1()._2().get().getVendorCode() + "\n"
                    + "Vendor Name : " + f._2()._1()._2().get().getVendorName() + "\n"
            );
        });

        /*MapColumn schema to cassandra*/
        Map<String, String> columnNameMappings = new HashMap<String, String>();
        columnNameMappings.put("requestNo", "requestno");
        columnNameMappings.put("transferOutNo", "transferoutno");
        columnNameMappings.put("transferInNo", "transferinno");
        columnNameMappings.put("createBy", "createby");
        columnNameMappings.put("matCode_key", "matcode_key");

        /*insert data to cassandra*/
        JavaRDD<RequestGoodsDetailReport> requestGoodsRDD = allAggregation
                .map(f -> {

                    /*Condition*/
                    String createdBy = f._2()._1()._2().isPresent() ?
                            f._2()._1()._2().get().getCreateBy() : null;
                    String transferInNo = f._2()._2().isPresent() ?
                            f._2()._2().get().getTransferNo() : null;

                    RequestGoodsDetailReport requestGoods = new RequestGoodsDetailReport();
                    requestGoods.setRequestNo(f._2()._1()._1().getRequestNo());
                    requestGoods.setTransferOutNo(f._2()._1()._2().get().getTransferNo());
                    requestGoods.setCreateBy(createdBy);
                    requestGoods.setTransferInNo(transferInNo);
                    requestGoods.setMatCode_key(f._2()._1()._1().getMatCode_key());

                    return requestGoods;
                });

        /* show insert data to cassandra */
        System.out.println("===== insert data to cassandra =====");
        requestGoodsRDD.foreach(f -> System.out.println("RequestNo: " + f.getRequestNo()
                + "TransferOutNo : " + f.getTransferOutNo()
                + "TransferInNo : " + f.getTransferInNo()
                + "createBy : " + f.getCreateBy()
                + "matCode_key : " + f.getMatCode_key()
        ));

        javaFunctions(requestGoodsRDD).writerBuilder(
                "damocles", "requestgoods_report",
                CassandraJavaUtil.mapToRow(RequestGoodsDetailReport.class,
                        columnNameMappings)).saveToCassandra();
    }
}
