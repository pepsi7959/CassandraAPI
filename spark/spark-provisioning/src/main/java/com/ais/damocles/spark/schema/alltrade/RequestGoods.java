package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class RequestGoods extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "requestNo")
	private String requestNo;

	public RequestGoods() {
	
	}

	public RequestGoods(String requestNo) {
		this.requestNo = requestNo;
		this.columnMapper.put("requestNo", "requestNo");
	}

	public String getRequestNo() {
		return this.requestNo;
	}

	public void setRequestNo(String requestNo) {
		this.requestNo = requestNo;
	}
}
