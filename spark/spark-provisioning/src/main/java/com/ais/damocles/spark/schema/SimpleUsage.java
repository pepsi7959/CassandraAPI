package com.ais.damocles.spark.schema;

public class SimpleUsage extends CommonSchema {
	private int usage = 0;

	public SimpleUsage() {
		super();
	}

	public SimpleUsage(int usage) {
		this.usage = usage;
	}

	public SimpleUsage(String timeEvent, String eventType, String identity,
			String cycle, String group, String promotion, String service,
			String status, String version, int usage) {
		this.usage = usage;
		setTimeEvent(timeEvent);
		setEventType(eventType);
		setIdentity(identity);
		setCycle(cycle);
		setGroup(group);
		setPromotion(promotion);
		setService(service);
		setStatus(status);
		setVersion(version);
	}

	public int getUsage() {
		return this.usage;
	}

	public void setUsage(int usage) {
		this.usage = usage;
	}
}
