package com.ais.damocles.spark.demo;

import com.ais.damocles.spark.schema.alltrade.*;
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
    private static final String ADJUSTSTOCKREPORT_TABLE = "adjuststock_report";

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
                    stockAdjustment.setLocationCode(f.getString(17));
                    stockAdjustment.setLocationName(f.getString(18));
                    stockAdjustment.setAdjustDateTime(f.getString(3));
                    stockAdjustment.setAdjustStockNo(f.getString(1));
                    //stockAdjustment.setItemNo(f.getString(null));
                    stockAdjustment.setCommercialName_key(f.getList(7));
                    stockAdjustment.setMatCode_key(f.getList(4));
                    //stockAdjustment.setMatDescription(f.getString(null));
                    //stockAdjustment.setProductType_key(f.getList(8));
                    //stockAdjustment.setProductSubType_key(f.getList(9));
                    stockAdjustment.setBrand_key(f.getList(5));
                    stockAdjustment.setModel_key(f.getList(6));
                    //stockAdjustment.setSubStock(f.getString(null));
                    stockAdjustment.setAdjustType(f.getString(13));
                    stockAdjustment.setSerial_key(f.getList(10));
                    //stockAdjustment.setAdjustQty(f.getString(null));
                    //stockAdjustment.setReason(f.getString(null));
                    stockAdjustment.setRemark(f.getString(27));

                    return new Tuple2<>(stockAdjustment.getAdjustStockNo(), stockAdjustment);
                });

        cassandraRowStockAdjustment.foreach(f ->
                System.out.println("column 1 : " + f.getString(0) + "\n"));


        /* show Request Goods */
        System.out.println("===== Stock Adjustment =====");
        stockAdjustmentPairRDD.foreach(f ->
                System.out.println("Adjust Stock No : " + f._1() + "\n"));


        JavaPairRDD<String, StockAdjustment>
                Aggregation = stockAdjustmentPairRDD;

        System.out.println("======== Adjust Stock Report ========");
        Aggregation.foreach(f -> System.out.println("key : " + f._1()
                        + "Company : " + f._2().getCompany() + "\n"
                        + "Location Code : " + f._2().getLocationCode() + "\n"
                        + "Location Name : " + f._2().getLocationName() + "\n"
                        + "Adjust Date Time : " + f._2().getAdjustDateTime() + "\n"
                        + "Adjust Stock No : " + f._2().getAdjustStockNo() + "\n"
                        //+ "Item No : " + f._2().getItemNo() + "\n"
                        + "Commercial Name : " + f._2().getCommercialName_key() + "\n"
                        + "Mat Code : " + f._2().getMatCode_key() + "\n"
                        //+ "Mat Description : " + f._2().getMatDescription() + "\n"
                        //+ "Product Type : " + f._2().getProductType_key() + "\n"
                        //+ "Product Sub Type : " + f._2().getProductSubType_key() + "\n"
                        + "Brand : " + f._2().getBrand_key() + "\n"
                        + "Model : " + f._2().getModel_key() + "\n"
                        //+ "Sub Stock : " + f._2().getSubStock() + "\n"
                        + "Adjust Type : " + f._2().getAdjustType() + "\n"
                        + "Serial : " + f._2().getSerial_key() + "\n"
                        //+ "Adjust Qty : " + f._2().getAdjustQty() + "\n"
                        //+ "Reason : " + f._2().getReason() + "\n"
                        + "Remark : " + f._2().getRemark() + "\n"

        ));

        /*MapColumn schema to cassandra*/
        Map<String, String> columnNameMappings = new HashMap<>();
        columnNameMappings.put("company", "company");
        columnNameMappings.put("locationCode", "locationcode");
        columnNameMappings.put("locationName", "locationname");
        columnNameMappings.put("adjustDateTime", "adjustdatetime");
        columnNameMappings.put("adjustStockNo", "adjuststockno");
        columnNameMappings.put("commercialName_key", "commercialname_key");
        columnNameMappings.put("matCode_key", "matcode_key");
        //columnNameMappings.put("productType_key", "producttype");
        //columnNameMappings.put("productSubType_key", "productsubtype");
        columnNameMappings.put("brand_key", "brand_key");
        columnNameMappings.put("model_key", "model_key");
        //columnNameMappings.put("subStock", "substock");
        columnNameMappings.put("adjustType", "adjusttype");
        columnNameMappings.put("serial", "serial");
        //columnNameMappings.put("adjustQty", "adjustqty");
        //columnNameMappings.put("reason", "reason");
        columnNameMappings.put("remark", "remark");

        /*insert data to cassandra*/
        JavaRDD<AdjustStockReport> adjustStockRDD = Aggregation
                .map((Tuple2<String, StockAdjustment> f) -> {

                    AdjustStockReport adjustStock = new AdjustStockReport();

                    adjustStock.setCompany(f._2().getCompany());
                    adjustStock.setLocationCode(f._2().getLocationCode());
                    adjustStock.setLocationName(f._2().getLocationName());
                    adjustStock.setAdjustDateTime(f._2().getAdjustDateTime());
                    adjustStock.setAdjustStockNo(f._2().getAdjustStockNo());
                    //adjustStock.setItemNo(f._2().getString(null));
                    adjustStock.setCommercialName_key(f._2().getCommercialName_key());
                    adjustStock.setMatCode_key(f._2().getMatCode_key());
                    //adjustStock.setMatDescription(f._2().getString(null));
                    //adjustStock.setProductType(f._2().getProductType_key());
                    //adjustStock.setProductSubType(f._2().getProductSubType_key());
                    adjustStock.setBrand_key(f._2().getBrand_key());
                    adjustStock.setModel_key(f._2().getModel_key());
                    //adjustStock.setSubStock(f._2().getString(null));
                    adjustStock.setAdjustType(f._2().getAdjustType());
                    adjustStock.setSerial(f._2().getSerial_key());
                    //adjustStock.setAdjustQty(f._2().getAdjustQty("null"));
                    //adjustStock.setReason(f._2().getString(null));
                    adjustStock.setRemark(f._2().getRemark());

                    return adjustStock;
                });

        /* show insert data to cassandra */
        System.out.println("===== insert data to cassandra =====");
        adjustStockRDD.foreach(f -> System.out.println(
                "Adjust Stock No: " + f.getAdjustStockNo() + "\n"
                        + "Adjust Date Time : " + f.getAdjustDateTime() + "\n"
                        + "Mat Code key : " + f.getMatCode_key()
        ));

        javaFunctions(adjustStockRDD).writerBuilder(
                DAMOCLES_KEYSPACE, ADJUSTSTOCKREPORT_TABLE,
                CassandraJavaUtil.mapToRow(AdjustStockReport.class,
                        columnNameMappings)).saveToCassandra();
    }
}

