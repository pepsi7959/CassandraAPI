package com.ais.damocles.spark.util;
import kafka.serializer.Decoder;

public class ValueDecoder implements Decoder<String> {

	@Override
	public String fromBytes(byte[] bytes) {
		return bytes.toString();
	}

	public static String fromBytes(String bytes) {
		System.out.println("Decode bytes : "+bytes);
		String[] fistString = bytes.toString().split("\\|");
		return (fistString.length > 0)?fistString[0]:"";
	}
}
