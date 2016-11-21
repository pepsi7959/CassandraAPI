#java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader <filename.topname>
#java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader $1
java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader $1 $2 $3
