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

public class AlltradeAdjustStockReport {

    private static final String DAMOCLES_KEYSPACE = "damocles";
    private static final String STOCKADJUSTMENT_TABLE = "stockadjustment";
    private static final String ADJUSTSTOCKREPORT_TABLE = "adjustStock_report";

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

        JavaSparkContext sc = jssc.sparkContext();

        /* Load RequestGoods from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowStockAdjustment = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, STOCKADJUSTMENT_TABLE);

        JavaPairRDD<String, StockAdjustment> stockAdjustmentPairRDD = cassandraRowStockAdjustment
                .mapToPair(f -> {
                    StockAdjustment stockAdjustment = new StockAdjustment();

                    stockAdjustment.setCompany(f.getString(2));
                    stockAdjustment.setLocationCode(f.getString(16));
                    stockAdjustment.setLocationName(f.getString(17));
                    stockAdjustment.setAdjustDateTime(f.getString(3));
                    stockAdjustment.setAdjustStockNo(f.getString(1));
                    //stockAdjustment.setItemNo(f.getString(null));
                    stockAdjustment.setCommercialName_key(f.getString(7));
                    stockAdjustment.setMatCode_key(f.getString(4));
                    //stockAdjustment.setMatDescription(f.getString(null));
                    stockAdjustment.setProductType_key(f.getString(8));
                    stockAdjustment.setProductSubType_key(f.getString(9));
                    stockAdjustment.setBrand_key(f.getString(5));
                    stockAdjustment.setModel_key(f.getString(6));
                    //stockAdjustment.setSubStock(f.getString(null));
                    //stockAdjustment.setAdjustType(f.getString(null));
                    stockAdjustment.setSerial_key(f.getString(10));
                    //stockAdjustment.setAdjustQty(f.getString(null));
                    //stockAdjustment.setReason(f.getString(null));
                    stockAdjustment.setRemark(f.getString(26));

                    return new Tuple2<>(stockAdjustment.getAdjustStockNo(), stockAdjustment);
                });

        cassandraRowStockAdjustment.foreach(f ->
                System.out.println("column 1 : " + f.getString(0) + "\n"));


        /* show Request Goods */
        System.out.println("===== Stock Adjustment =====");
        stockAdjustmentPairRDD.foreach(f ->
                System.out.println("RequestNo : " + f._1() + "\n"));


        JavaPairRDD<String, StockAdjustment>
                Aggregation = stockAdjustmentPairRDD;

        System.out.println("======== Adjust Stock Report ========");
        Aggregation.foreach(f -> {


            System.out.println("key : " + f._1()
                            + "Company : " + f._2().getCompany() + "\n"
                            + "Location Code : " + f._2().getLocationCode() + "\n"
                            + "Location Name : " + f._2().getLocationName() + "\n"
                            + "Adjust Date Time : " + f._2().getAdjustDateTime() + "\n"
                            + "Adjust Stock No : " + f._2().getAdjustStockNo() + "\n"
                            //+ "Item No : " + f._2().getItemNo() + "\n"
                            + "Commercial Name : " + f._2().getCommercialName_key() + "\n"
                            + "Mat Code : " + f._2().getMatCode_key() + "\n"
                            //+ "Mat Description : " + f._2().getMatDescription() + "\n"
                            + "Product Type : " + f._2().getProductType_key() + "\n"
                            + "Product Sub Type : " + f._2().getProductSubType_key() + "\n"
                            + "Brand : " + f._2().getBrand_key() + "\n"
                            + "Model : " + f._2().getModel_key() + "\n"
                            //+ "Sub Stock : " + f._2().getSubStock() + "\n"
                            //+ "Adjust Type : " + f._2().getAdjustType() + "\n"
                            + "Serial : " + f._2().getSerial_key() + "\n"
                            //+ "Adjust Qty : " + f._2().getAdjustQty() + "\n"
                            //+ "Reason : " + f._2().getReason() + "\n"
                            + "Remark : " + f._2().getRemark() + "\n"

            );
        });

//        /*MapColumn schema to cassandra*/
//        Map<String, String> columnNameMappings = new HashMap<>();
//        columnNameMappings.put("company", "company");
//        columnNameMappings.put("locationCode", "locationcode");
//        columnNameMappings.put("locationName", "locationname");
//        columnNameMappings.put("adjustDateTime", "adjustdatetime");
//        columnNameMappings.put("adjustStockNo", "adjuststockno");
//        columnNameMappings.put("commercialName_key", "commercialname_key");
//        columnNameMappings.put("matCode_key", "matCode_key");

//        /*insert data to cassandra*/
//        JavaRDD<TransferDetailReport> orderTransferRDD = allAggregation
//                .map(f -> {
//
//
//                    TransferDetailReport orderTransfer = new TransferDetailReport();
//
//                    orderTransfer.setCompany(f._2()._1()._2().get().getCompany());
//
//                    return orderTransfer;
//                });
//
//        /* show insert data to cassandra */
//        System.out.println("===== insert data to cassandra =====");
//        orderTransferRDD.foreach(f -> System.out.println(
//                "RequestNo: " + f.getRequestNo() + "\n"
//                        + "TransferOutNo : " + f.getTransferOutNo() + "\n"
//                        + "TransferInNo : " + f.getTransferInNo() + "\n"
//                        + "createBy : " + f.getCreateBy() + "\n"
//                        + "matCode_key : " + f.getMatCode_key()
//        ));
//
//        javaFunctions(orderTransferRDD).writerBuilder(
//                DAMOCLES_KEYSPACE, ADJUSTSTOCKREPORT_TABLE,
//                CassandraJavaUtil.mapToRow(TransferDetailReport.class,
//                        columnNameMappings)).saveToCassandra();
    }
}

