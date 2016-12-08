package com.ais.damocles.spark.schema.demo;

import com.datastax.driver.mapping.annotations.Column;

public class Exhibition extends com.ais.damocles.spark.schema.CommonSchema{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	@Column(name="app_name")
	private String appName;
	/* TODO: @Peaw add another field here!
	 * 
	 */
	public Exhibition(){
		
	}
	
	public Exhibition(String appName, String eventTime, String eventType, String Usage){
		
	}
	
	/* TODO: @peaw add getter and setter here!
	 * 
	 */
}
