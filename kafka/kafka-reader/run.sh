#java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader <filename.topname>
#java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader $1
##java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader $1 $2 $3

if [ -z $1 ]; then
	echo -e '\t' "****************************"
	echo -e '\t' "***** Enter topic name *****"
	echo -e '\t' "****************************"
elif [ -z $2 ]; then
	echo -e '\t' "*****************************"
	echo -e '\t' "****** Enter file name ******"
	echo -e '\t' "*****************************"
elif [ -z $3 ]; then
	echo -e '\t' "*****************************"
	echo -e '\t' "***** Enter IP and Port *****"
	echo -e '\t' "*****************************"
else
	echo -e '\t' "*********************************"
	echo -e '\t' "***** Information completed *****"
	echo -e '\t' "*********************************"
	java -cp ./target/dependency/*:./target/kafka-reader-0.0.1.jar com.ais.damolces.kafka.KafkaReader $1 $2 $3	
fi

