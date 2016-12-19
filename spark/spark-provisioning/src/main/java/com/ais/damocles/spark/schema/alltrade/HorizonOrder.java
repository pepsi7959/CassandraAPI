package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class HorizonOrder extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "orderRef")
	private String orderRef;
	@Column(name = "requestDt")
	private String requestDt;
	@Column(name = "serviceReqType")
	private String serviceReqType;
	@Column(name = "provisioningFlag")
	private String provisioningFlag;
	@Column(name = "parallelFlag")
	private String parallelFlag;
	@Column(name = "resendFlag")
	private String resendFlag;
	@Column(name = "woHeader")
	private String woHeader;

	public HorizonOrder() {

	}

	public HorizonOrder(String orderRef, String requestDt,
			String serviceReqType, String provisioningFlag,
			String parallelFlag, String resendFlag, String woHeader) {
		this.orderRef = orderRef;
		this.columnMapper.put("orderRef", "orderRef");

		this.serviceReqType = serviceReqType;
		this.columnMapper.put("serviceReqType", "serviceReqType");

		this.provisioningFlag = provisioningFlag;
		this.columnMapper.put("provisioningFlag", "provisioningFlag");

		this.parallelFlag = parallelFlag;
		this.columnMapper.put("parallelFlag", "parallelFlag");

		this.resendFlag = resendFlag;
		this.columnMapper.put("resendFlag", "resendFlag");

		this.woHeader = woHeader;
		this.columnMapper.put("woHeader", "woHeader");

	}

	public String getOrderRef() {
		return orderRef;
	}

	public void setOrderRef(String orderRef) {
		this.orderRef = orderRef;
	}

	public String getRequestDt() {
		return requestDt;
	}

	public void setRequestDt(String requestDt) {
		this.requestDt = requestDt;
	}

	public String getServiceReqType() {
		return serviceReqType;
	}

	public void setServiceReqType(String serviceReqType) {
		this.serviceReqType = serviceReqType;
	}

	public String getProvisioningFlag() {
		return provisioningFlag;
	}

	public void setProvisioningFlag(String provisioningFlag) {
		this.provisioningFlag = provisioningFlag;
	}

	public String getParallelFlag() {
		return parallelFlag;
	}

	public void setParallelFlag(String parallelFlag) {
		this.parallelFlag = parallelFlag;
	}

	public String getResendFlag() {
		return resendFlag;
	}

	public void setResendFlag(String resendFlag) {
		this.resendFlag = resendFlag;
	}

	public String getWoHeader() {
		return woHeader;
	}

	public void setWoHeader(String woHeader) {
		this.woHeader = woHeader;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
