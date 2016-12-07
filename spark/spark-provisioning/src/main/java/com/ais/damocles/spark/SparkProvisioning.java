package com.ais.damocles.spark;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;

import kafka.serializer.StringDecoder;

import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.SparkConf;
import org.apache.spark.streaming.Durations;
import org.apache.spark.streaming.api.java.JavaPairInputDStream;
import org.apache.spark.streaming.api.java.JavaStreamingContext;
import org.apache.spark.streaming.kafka.KafkaUtils;

import com.ais.damocles.spark.util.KeyDecoder;
import com.ais.damocles.spark.util.PropertyFileReader;
import com.ais.damocles.spark.util.ValueDecoder;
import com.ais.damocles.spark.schema.Usage;
import com.datastax.spark.connector.japi.CassandraJavaUtil;
import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;



public class SparkProvisioning{
	 //private static final Logger logger = Logger.getLogger(aisDataProcessor.class);

	 public static JavaSparkContext sc;
	 public static void main(String[] args) throws Exception {

		 // read Spark and Cassandra properties and create SparkConf
		Properties prop = null;
		if( args.length == 0 ){
			prop = PropertyFileReader.readPropertyFile();
		}else if(args.length == 1){
			prop = PropertyFileReader.readPropertyFile(args[0]);
		}else{
			prop = PropertyFileReader.readPropertyFile();
		}
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
	    
 		 sc = jssc.sparkContext();
		 messages.foreachRDD((v1, v2) -> {
			v1.foreach(f -> {
				System.out.println("Data : " + f._2());
				String words[] = f._2().split("\\|");

				int usage = Integer.parseInt(words[2]);

				List<Usage> usageList = Arrays.asList(new Usage(words[0],
						words[1], usage));
				JavaRDD<Usage> rdd = sc.parallelize(usageList);

				// Map Cassandra table column
				Map<String, String> columnNameMappings = new HashMap<String, String>();
				columnNameMappings.put("user", "user");
				columnNameMappings.put("type", "type");
				columnNameMappings.put("usage", "usage");
				rdd.foreach(x->System.out.println("user: "+x.getUser()+" type: "+x.getType()+" usage: "+x.getUsage()));
				javaFunctions(rdd).writerBuilder(
						"damocles",
						"usage",
						CassandraJavaUtil.mapToRow(Usage.class,
								columnNameMappings)).saveToCassandra();

			});
			return null;
		});

	    
 		 /*
	     // Get the lines, split them into words, count the words and print
	    JavaDStream<String> lines = messages.map(new Function<Tuple2<String, String>, String>() {
		      @Override
		      public String call(Tuple2<String, String> tuple2) {
		        return tuple2._2();
		      }
		    });
	    lines.print();

	    
        JavaDStream<String> words = lines.flatMap(new FlatMapFunction<String, String>() {
	      @Override
	      public Iterable<String> call(String x) {
	        return Lists.newArrayList(SPACE.split(x));
	      }   
	    }); 

	    words.print();
		
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
		 jssc.start();            
		 System.out.println("start jssc");
		 jssc.awaitTermination();  
		 System.out.println("terminate jssc");
		 
	  }
  }

