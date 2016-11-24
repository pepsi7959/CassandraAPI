package com.ais.damocles.spark;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Pattern;
import java.util.Iterator;
import java.util.Arrays;

import org.apache.log4j.Logger;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.*;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.*;
import org.apache.spark.api.java.function.Function3;
import org.apache.spark.broadcast.Broadcast;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.State;
import org.apache.spark.streaming.StateSpec;
import org.apache.spark.streaming.api.java.JavaDStream;
import org.apache.spark.streaming.api.java.JavaMapWithStateDStream;
import org.apache.spark.streaming.api.java.JavaPairDStream;
import org.apache.spark.streaming.api.java.JavaPairInputDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;
import org.apache.spark.streaming.kafka.KafkaUtils;

//import com.google.common.base.Optional;
//import com.ais.damocles.spark.util.aisDataDecoder;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.ais.damocles.spark.Usage;
//import com.ais.damocles.spark.vo.aisData;
//import com.ais.damocles.spark.vo.POIData;

import static com.datastax.spark.connector.japi.CassandraStreamingJavaUtil.javaFunctions;
import static com.datastax.spark.connector.japi.CassandraJavaUtil.*;
import com.datastax.spark.connector.japi.CassandraJavaUtil;

import kafka.serializer.StringDecoder;
import scala.Tuple2;
import scala.Tuple3;


public class SparkProvisioning{
	 //private static final Logger logger = Logger.getLogger(aisDataProcessor.class);
	 private static final Pattern SPACE = Pattern.compile(" ");
	 private final String usage_schema[] = {"user", "type", "usage"};
	 public static void main(String[] args) throws Exception {

		 //read Spark and Cassandra properties and create SparkConf
		 Properties prop = PropertyFileReader.readPropertyFile();		
		 SparkConf conf = new SparkConf()
				 .setAppName(prop.getProperty("com.ais.damocles.spark.damocles.name"))
				 .setMaster(prop.getProperty("com.ais.damocles.spark.master"))
				 .set("spark.cassandra.connection.host", prop.getProperty("com.ais.damocles.cassandra.host"))
				 .set("spark.cassandra.connection.port", prop.getProperty("com.ais.damocles.cassandra.port"))
				 .set("spark.cassandra.connection.keep_alive_ms", prop.getProperty("com.ais.damocles.cassandra.keep_alive"));		 
		 //batch interval of 5 seconds for incoming stream		 
		 JavaStreamingContext 	jssc = new JavaStreamingContext(conf, Durations.seconds(5));	
		 //add check point directory
		 jssc.checkpoint(prop.getProperty("com.ais.damocles.spark.checkpoint.dir"));
		 
		 //read and set Kafka properties
		 Map<String, String> kafkaParams = new HashMap<String, String>();
		 kafkaParams.put("zookeeper.connect", prop.getProperty("com.ais.damocles.kafka.zookeeper"));
		 kafkaParams.put("metadata.broker.list", prop.getProperty("com.ais.damocles.kafka.brokerlist"));
		 String topic = prop.getProperty("com.ais.damocles.kafka.topic");
		 Set<String> topicsSet = new HashSet<String>();
		 topicsSet.add(topic);


	     JavaPairInputDStream<String, String> messages = KafkaUtils.createDirectStream(
	        	jssc,
	        	String.class,
	        	String.class,
	        	StringDecoder.class,
	        	StringDecoder.class,
	        	kafkaParams,
	       	 	topicsSet
			);
 		 //logger.info("Starting Stream Processing");
 		 messages.print();
	    
	     // Get the lines, split them into words, count the words and print
	    JavaDStream<String> lines = messages.map(new Function<Tuple2<String, String>, String>() {
		      @Override
		      public String call(Tuple2<String, String> tuple2) {
		        return tuple2._2();
		      }
		    });

	    /*
	    JavaDStream<String> words = lines.flatMap(new FlatMapFunction<String, String>() {
		      @Override
		      public Iterator<String> call(String x) {
		        return Arrays.asList(SPACE.split(x)).iterator();
		      }
		    });
	    JavaPairDStream<String, Integer> wordCounts = words.mapToPair(
		      new PairFunction<String, String, Integer>() {
		        @Override
		        public Tuple2<String, Integer> call(String s) {
		          return new Tuple2<>(s, 1);
		        }
		      }).reduceByKey(
		        new Function2<Integer, Integer, Integer>() {
		        @Override
		        public Integer call(Integer i1, Integer i2) {
		          return i1 + i2;
		        }
		      });
			wordCounts.print();
		*/
		// Map Cassandra table column
		Map<String, String> columnNameMappings = new HashMap<String, String>();
		columnNameMappings.put("user", "user");
		columnNameMappings.put("type", "type");
		columnNameMappings.put("usage", "usage");
		Usage myUsage = new Usage("pepsi", "mobile", "10");
		// call CassandraStreamingJavaUtil function to save in DB
		//javaFunctions(messages).writerBuilder("simpledb", "usage",
		//		CassandraJavaUtil.mapToRow(Usage.class, columnNameMappings)).saveToCassandra();
		System.out.println("Write to total_traffic");
		 jssc.start();            
		 System.out.println("start jssc");
		 jssc.awaitTermination();  
		 System.out.println("terminate jssc");
		 
	  }
  }

