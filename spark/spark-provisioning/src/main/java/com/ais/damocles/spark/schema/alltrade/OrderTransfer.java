package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class OrderTransfer extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "docRef")
	private String docRef;
	@Column(name = "transferNo")
	private String transferNo;

	/*
	 * TODO: add other attribute from schema
	 */

	public OrderTransfer() {

	}

	public OrderTransfer(String docRef) {
		this.docRef = docRef;
		this.columnMapper.put("docRef", "docRef");
	}

	public String getDocRef() {
		return this.docRef;
	}

	public void setDocRef(String docRef) {
		this.docRef = docRef;
	}

	public String getTransferNo() {
		return this.transferNo;
	}

	public void setTransferNo(String transferNo) {
		this.transferNo = transferNo;
	}
}
