package com.ais.damocles.spark.demo;

import com.ais.damocles.spark.schema.alltrade.ObtainDetail;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.datastax.spark.connector.japi.CassandraRow;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaStreamingContext;
import scala.Tuple2;

import java.util.Properties;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;

/**
 * Created by User on 1/16/2017.
 */
public class AlltradeDirectsaleDetailReport {

    private static final String DAMOCLES_KEYSPACE = "damocles";
    private static final String OBTAINDETAIL_TABLE = "obtaindetail";
    private static final String DIRECTSALEDETAILREPORT_TABLE = "directsaledetail_report";

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

        AggregateDirectSaleDetail(jssc);

        //jssc.start();
        //System.out.println("START JAVA STREAMING CONTEXT");
        //jssc.awaitTermination();
        //System.out.println("TERMINATE JAVA STREAMING CONTEXT");
    }

    private static void AggregateDirectSaleDetail(JavaStreamingContext jssc) {

        JavaSparkContext sc = jssc.sparkContext();

        /* Load RequestGoods from the Cassandra */
        JavaRDD<CassandraRow> cassandraRowObtainDetail = javaFunctions(sc)
                .cassandraTable(DAMOCLES_KEYSPACE, OBTAINDETAIL_TABLE);

        JavaPairRDD<String, ObtainDetail> obtainDetailPairRDD = cassandraRowObtainDetail
                .mapToPair(f -> {
                    ObtainDetail obtainDetail = new ObtainDetail();

                    obtainDetail.setLocationCode(f.getString(8));
                    obtainDetail.setLocationName(f.getString(9));
                    obtainDetail.setObtainNo(f.getString(0));
                    obtainDetail.setObtainDateTime(f.getString(1));
                    obtainDetail.setCreateBy(f.getString(5));

                    return new Tuple2<>(obtainDetail.getObtainNo(), obtainDetail);
                });


        System.out.println("===== Direct Sale Detail =====");
        obtainDetailPairRDD.foreach(f ->
                System.out.println("Obtain No : " + f._1() + "\n"));

        JavaPairRDD<String, ObtainDetail>
                Aggregation = obtainDetailPairRDD;

        System.out.println("======== Direct Sale Detail Report ========");
        Aggregation.foreach(f -> System.out.println("key : " + f._1() + "\n"
                + "Location Code : " + f._2().getLocationCode() + "\n"
                + "Location Name : " + f._2().getLocationName() + "\n"
                + "Obtain No : " + f._2().getObtainNo() + "\n"
                + "Obtain Date Time : " + f._2().getObtainDateTime() + "\n"
                + "Created By : " + f._2().getCreateBy() + "\n"

        ));

    }

}
