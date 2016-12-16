package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class HorizonOrder extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "orderRef")
	private String orderRef;

	public HorizonOrder() {

	}

	public HorizonOrder(String orderRef) {
		this.orderRef = orderRef;
		this.columnMapper.put("orderRef", "orderRef");

	}

	public String getOrderRef() {
		return orderRef;
	}

	public void setOrderRef(String orderRef) {
		this.orderRef = orderRef;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
