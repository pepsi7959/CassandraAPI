package com.ais.damocles.spark.util;
import kafka.serializer.Decoder;

public class KeyDecoder implements Decoder<String> {

	@Override
	public String fromBytes(byte[] bytes) {
		String[] fistString = bytes.toString().split("\\|");
		System.out.println("First name : "+fistString[0]);
		return (fistString.length > 0)?fistString[0]:"";
	}

	public static String fromBytes(String bytes) {
		System.out.println("Decode bytes : "+bytes);
		String[] fistString = bytes.toString().split("\\|");
		return (fistString.length > 0)?fistString[0]:"";
	}
}
