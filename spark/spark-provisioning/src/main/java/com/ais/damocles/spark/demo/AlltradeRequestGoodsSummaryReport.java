package com.ais.damocles.spark.demo;

import com.ais.damocles.spark.schema.alltrade.OrderTransfer;
import com.ais.damocles.spark.schema.alltrade.RequestGoods;
import com.ais.damocles.spark.schema.alltrade.RequestGoodsSummaryReport;
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

/**
 * Created by User on 1/17/2017.
 */
public class AlltradeRequestGoodsSummaryReport {

    private static final String DAMOCLES_KEYSPACE = "damocles";
    private static final String ORDERTRANSFER_TABLE = "ordertransfer";
    private static final String REQUESTGOODS_TABLE = "requestgoods";
    private static final String REQUESTGOODSSUMMARY_REPORT = "requestgoodssummary_report";

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

        AggregateRequestGoodsSummary(jssc);

        //jssc.start();
        //System.out.println("START JAVA STREAMING CONTEXT");
        //jssc.awaitTermination();
        //System.out.println("TERMINATE JAVA STREAMING CONTEXT");
    }

    private static void AggregateRequestGoodsSummary(JavaStreamingContext jssc) {

            JavaSparkContext sc = jssc.sparkContext();

		/* Load RequestGoods from the Cassandra */
            JavaRDD<CassandraRow> cassandraRowRequestGoods = javaFunctions(sc)
                    .cassandraTable(DAMOCLES_KEYSPACE, REQUESTGOODS_TABLE);

            JavaPairRDD<String, RequestGoods> requestGoodsPairRDD = cassandraRowRequestGoods
                    .mapToPair(f -> {

                        RequestGoods requestGoods = new RequestGoods();
                        requestGoods.setCompany(f.getString(11));
                        requestGoods.setToLocationCode(f.getString(44));
                        requestGoods.setToLocationName(f.getString(45));
                        requestGoods.setForSubStock(f.getString(18));
                        requestGoods.setCreateBy(f.getString(13));
                        requestGoods.setCreateDateTime(f.getString(14));
                        requestGoods.setRequestNo(f.getString(1));
                        requestGoods.setRequestStatus(f.getString(2));
                        requestGoods.setReservedNo(f.getString(34));
                        requestGoods.setDoNo(f.getString(17));
                        requestGoods.setPickingDateTime(f.getString(28));
                        requestGoods.setDeviceHandsetQty(f.getString(15));
                        requestGoods.setDeviceHandsetBundleQty(f.getString(16));
                        requestGoods.setSimPrepaidQty(f.getString(42));
                        requestGoods.setSimPostpaidQty(f.getString(41));
                        requestGoods.setPremiumQty(f.getString(29));
                        requestGoods.setOtherQty(f.getString(27));
                        requestGoods.setReceiveBy(f.getString(31));
                        requestGoods.setContactNo(f.getString(12));
                        requestGoods.setRemark(f.getString(32));

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
                        orderTransferIn.setCreateDateTime(f.getString(12));
                        orderTransferIn.setCreateBy(f.getString(11));
                        orderTransferIn.setTransferNo(f.getString(40));

                        return new Tuple2<>(orderTransferIn.getDocRef(), orderTransferIn);
                    });

		/* Load TransferOut */
            JavaPairRDD<String, OrderTransfer> transferOutPairRDD = cassandraRowTransferOut
                    .mapToPair(f -> {
                        OrderTransfer orderTransferOut = new OrderTransfer();
                        orderTransferOut.setCreateDateTime(f.getString(12));
                        orderTransferOut.setTransferNo(f.getString(40));

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
                        + "Company : " + f._2()._1()._1().getCompany() + "\n"
                        + "ToLocationCode : " + f._2()._1()._1().getToLocationCode() + "\n"
                        + "ToLocationName : " + f._2()._1()._1().getToLocationName() + "\n"
                        + "ForSubStock : " + f._2()._1()._1().getForSubStock() + "\n"
                        + "CreateDateTime : " + f._2()._1()._1().getCreateDateTime() + "\n"
                        + "RequestStatus : " + f._2()._1()._1().getRequestStatus() + "\n"
                        + "ReservedNo : " + f._2()._1()._1().getReservedNo() + "\n"
                        + "DoNo : " + f._2()._1()._1().getDoNo() + "\n"
                        + "PickingDateTime : " + f._2()._1()._1().getPickingDateTime() + "\n"
                        + "TransferOutDateTime : " + f._2()._1()._2().get().getTransferDateTime() + "\n"
                        + "CreateBy : " + createdBy + "\n"
                        + "TransferInDateTime : " + transferindatetime + "\n"
                        + "DeviceHandsetQty : " + f._2()._1()._1().getDeviceHandsetQty()
                        + "DeviceHandsetBundelQty : " + f._2()._1()._1().getDeviceHandsetBundleQty()
                        + "SimPrepaidQty : " + f._2()._1()._1().getSimPrepaidQty()
                        + "SimPostpaidQty : " + f._2()._1()._1().getSimPostpaidQty()
                        + "PremiumQty : " + f._2()._1()._1().getPremiumQty()
                        + "OtherQty : " + f._2()._1()._1().getOtherQty()
                        + "ReceiveBy : " + f._2()._1()._1().getReceiveBy()
                        + "ContactNo : " + f._2()._1()._1().getContactNo()
                        + "Remark : " + f._2()._1()._1().getRemark() + "\n");
            });

        /*MapColumn schema to cassandra*/
            Map<String, String> columnNameMappings = new HashMap<>();

            columnNameMappings.put("company", "company");
            columnNameMappings.put("toLocationCode", "tolocationcode");
            columnNameMappings.put("toLocationName", "tolocationname");
            columnNameMappings.put("forSubStock", "forsubstock");
            columnNameMappings.put("createDateTime", "createdatetime");
            columnNameMappings.put("requestStatus", "requeststatus");
            columnNameMappings.put("reservedNo", "reservedno");
            columnNameMappings.put("doNo", "dono");
            columnNameMappings.put("pickingDateTime", "pickingdatetime");
            columnNameMappings.put("transferOutDateTime", "transferoutdatetime");
            columnNameMappings.put("createBy", "createby");
            columnNameMappings.put("deviceHandsetQty", "devicehandsetqty");
            columnNameMappings.put("deviceHandsetBundleQty", "devicehandsetbundleqty");
            columnNameMappings.put("simPrepaidQty", "simprepaidqty");
            columnNameMappings.put("simPostpaidQty", "simpostpaidqty");
            columnNameMappings.put("premiumQty", "premiumqty");
            columnNameMappings.put("otherQty", "otherqty");
            columnNameMappings.put("receiveBy", "receiveby");
            columnNameMappings.put("contactNo", "contactno");
            columnNameMappings.put("remark", "remark");

        /*insert data to cassandra*/
            JavaRDD<RequestGoodsSummaryReport> requestGoodsRDD = allAggregation
                    .map(f -> {

                    /*Condition*/
                        String createdBy = f._2()._1()._2().isPresent() ? f._2()._1()._2().get().getCreateBy() : null;
                        String transferindatetime = f._2()._2().isPresent() ? f._2()._2().get().getTransferDateTime() : null;

                        RequestGoodsSummaryReport requestGoods = new RequestGoodsSummaryReport();

                        requestGoods.setCompany(f._2()._1()._1().getCompany());
                        requestGoods.setToLocationCode(f._2()._1()._1().getToLocationCode());
                        requestGoods.setToLocationName(f._2()._1()._1().getToLocationName());
                        requestGoods.setForSubStock(f._2()._1()._1().getForSubStock());
                        requestGoods.setCreateDateTime( f._2()._1()._1().getCreateDateTime());
                        requestGoods.setRequestStatus(f._2()._1()._1().getRequestStatus());
                        requestGoods.setReservedNo(f._2()._1()._1().getReservedNo());
                        requestGoods.setDoNo(f._2()._1()._1().getDoNo());
                        requestGoods.setPickingDate(f._2()._1()._1().getPickingDateTime());
                        requestGoods.setTransferOutDate(f._2()._1()._2().get().getTransferDateTime());
                        requestGoods.setCreateBy(createdBy);
                        requestGoods.setTransferInDate(transferindatetime);
                        requestGoods.setDeviceHandsetQty(f._2()._1()._1().getDeviceHandsetQty());
                        requestGoods.setDeviceHandsetBundleQty(f._2()._1()._1().getDeviceHandsetBundleQty());
                        requestGoods.setSimPrepaidQty(f._2()._1()._1().getSimPrepaidQty());
                        requestGoods.setSimPostpaidQty(f._2()._1()._1().getSimPostpaidQty());
                        requestGoods.setPremiumQty(f._2()._1()._1().getPremiumQty());
                        requestGoods.setOtherQty(f._2()._1()._1().getOtherQty());
                        requestGoods.setContactNo(f._2()._1()._1().getRemark());
                        requestGoods.setRemark(f._2()._1()._1().getRemark());

                        return requestGoods;
                    });

        /* show insert data to cassandra */
            System.out.println("===== insert data to cassandra =====");
            requestGoodsRDD.foreach(f -> System.out.println("Company : " + f.getCompany()
                            + "Transfer Out Date : " + f.getTransferOutDate()
                            + "Transfer In Date : " + f.getTransferInDate()
            ));

            javaFunctions(requestGoodsRDD).writerBuilder(
                    DAMOCLES_KEYSPACE,
                    REQUESTGOODSSUMMARY_REPORT,
                    CassandraJavaUtil.mapToRow(RequestGoodsSummaryReport.class,
                            columnNameMappings)).saveToCassandra();
        }

    }
