package com.ais.damocles.spark.schema;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

import com.datastax.driver.mapping.annotations.Column;

public class CommonSchema implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "timeevent")
	protected String timeEvent;
	@Column(name = "eventeype")
	protected String eventType;
	@Column(name = "identity")
	protected String identity;
	@Column(name = "cycle")
	protected String cycle;
	@Column(name = "group")
	protected String group;
	@Column(name = "promotion")
	protected String promotion;
	@Column(name = "service")
	protected String service;
	@Column(name = "status")
	protected String status;
	@Column(name = "version")
	protected String version;

	protected Map<String, String> columnMapper = new HashMap<String, String>();

	public CommonSchema() {

	}

	public CommonSchema(String timeEvent, String eventType, String identity,
			String cycle, String group, String promotion, String service,
			String status, String version) {
		this.timeEvent = timeEvent;
		this.eventType = eventType;
		this.identity = identity;
		this.cycle = cycle;
		this.group = group;
		this.promotion = promotion;
		this.service = service;
		this.status = status;
		this.version = version;
		
	}

	public void setTimeEvent(String timeEvent) {
		this.timeEvent = timeEvent;
	}

	public String getTimeEvent() {
		return timeEvent;
	}

	public void setEventType(String eventType) {
		this.eventType = eventType;
	}

	public String getEventType() {
		return this.eventType;
	}

	public void setIdentity(String identity) {
		this.identity = identity;
	}

	public String getIdentity() {
		return identity;
	}

	public void setCycle(String cycle) {
		this.cycle = cycle;
	}

	public String getCycle() {
		return cycle;
	}

	public void setGroup(String group) {
		this.group = group;
	}

	public String getGroup() {
		return this.group;
	}

	public void setPromotion(String promotion) {
		this.promotion = promotion;
	}

	public String getPromotion() {
		return this.promotion;
	}

	public void setService(String service) {
		this.service = service;
	}

	public String getService() {
		return service;
	}

	public void setStatus(String status) {
		this.status = status;
	}

	public String getStatus() {
		return status;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getVersion() {
		return this.version;
	}
	
	public  Map<String,String> getColumnMapper(){
		return columnMapper;
	}
}
