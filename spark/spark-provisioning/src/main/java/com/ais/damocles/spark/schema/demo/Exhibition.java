package com.ais.damocles.spark.schema.demo;

import com.datastax.driver.mapping.annotations.Column;

public class Exhibition extends com.ais.damocles.spark.schema.CommonSchema{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	//@Column(name="app_name")
	private String appName;
	//@Column(name="usages")
        private int usage;
	/* TODO: @Peaw add another field here!
	 * 
	 */
	public Exhibition(){
		
	}
	
	public Exhibition(String appName, String timeEvent, String eventType, int usage){
		this.appName = appName;
		setTimeEvent(timeEvent);
		setEventType(eventType);
		this.usage = usage;
			
	}
	
	/* TODO: @peaw add getter and setter here!
	 * 
	 */
	public void setAppName(String appName){this.appName = appName;}
	public String getAppName(){return this.appName;}

	public void setUsage(int usage){this.usage = usage;}
	public int getUsage(){return this.usage;}


}




