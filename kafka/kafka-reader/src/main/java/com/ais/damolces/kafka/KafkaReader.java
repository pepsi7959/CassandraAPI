
//*****************************************************************************//
//************ run.sh testreadtextfile tempfile.txt localhost:9092 ************//
//*****************************************************************************//


package com.ais.damolces.kafka;


import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;


//import util.properties packages
import java.util.Properties;

//import simple producer packages
import org.apache.kafka.clients.producer.Producer;

//import KafkaProducer packages
import org.apache.kafka.clients.producer.KafkaProducer;

//import ProducerRecord packages
import org.apache.kafka.clients.producer.ProducerRecord;

//Create java class named "KafkaReader"
public class KafkaReader {
   
   	public static void main(String[] args) throws Exception{
      		BufferedReader br = null;
		int i=0;
      		// Check arguments length value
      		if(args.length == 0){
         		System.out.println("Enter topic name, file name and IP:Port");
			System.out.println("run.sh topic_name file_name localhost:Port");
         		return;
      		}
      
      		//Assign topicName to string variable
      		String topicName = args[0].toString();

		//Input file name
		String filename = args[1].toString();
		//System.out.println(filename);

		//Input IP and Port
		String IPPort = args[2].toString();
		//System.out.println(IPPort);

      		// create instance for properties to access producer configs   
      		Properties props = new Properties();
      
      		//Assign localhost id
      		props.put("bootstrap.servers", IPPort);
      
      		//Set acknowledgements for producer requests.      
      		props.put("acks", "all");
      
      		//If the request fails, the producer can automatically retry,
      		props.put("retries", 0);
      
      		//Specify buffer size in config
      		props.put("batch.size", 16384);
      
      		//Reduce the no of requests less than 0   
      		props.put("linger.ms", 1);
      
      		//The buffer.memory controls the total amount of memory available to the producer for buffering.   
      		props.put("buffer.memory", 33554432);
      
      		props.put("key.serializer","org.apache.kafka.common.serialization.StringSerializer");
         
      		props.put("value.serializer","org.apache.kafka.common.serialization.StringSerializer");
      
      		Producer<String, String> producer = new KafkaProducer<String, String>(props);
            
		try {
		    //br = new BufferedReader(new FileReader("tempfile.txt"));
		    br = new BufferedReader(new FileReader(filename));
		    String line;
		    while ((line = br.readLine()) != null) {
			//i++;
			//System.out.println("loop :"+i);
			producer.send(new ProducerRecord<String, String>(topicName,line,line));
		        //System.out.println(line);
		    }
		} catch (IOException e) {
		    e.printStackTrace();
		} finally {
		    try {
		        if (br != null) {
		            br.close();
		        }
		    } catch (IOException ex) {
		        ex.printStackTrace();
		    }
		}
		System.out.println("\t*************************************");
		System.out.println("\t***** Message sent successfully *****");
		System.out.println("\t*************************************");
   		producer.close();
   	}
}
