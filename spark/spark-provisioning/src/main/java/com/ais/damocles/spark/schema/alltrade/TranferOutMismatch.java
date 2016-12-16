package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class TranferOutMismatch extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "transactionId")
	private String transactionId;

	@Column(name = "transactionType")
	private String transactionType;

	@Column(name = "requestNo")
	private String requestNo;

	@Column(name = "company")
	private String company;

	@Column(name = "dataErrorDateTime")
	private String dataErrorDateTime;

	@Column(name = "createBy")
	private String createBy;

	@Column(name = "createDateTime")
	private String createDateTime;

	@Column(name = "updateBy")
	private String updateBy;

	@Column(name = "updateDateTime")
	private String updateDateTime;

	@Column(name = "misMatchDetail")
	private String misMatchDetail;

	public TranferOutMismatch() {

	}

	public TranferOutMismatch(String transactionId, String transactionType,
			String requestNo, String company, String dataErrorDateTime,
			String createBy, String createDateTime, String updateBy,
			String updateDateTime, String misMatchDetail) {
		
		this.transactionId = transactionId;
		this.columnMapper.put("transactionId", "transactionId");

		this.transactionType = transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.requestNo = requestNo;
		this.columnMapper.put("requestNo", "requestNo");

		this.company = company;
		this.columnMapper.put("company", "company");

		this.dataErrorDateTime = dataErrorDateTime;
		this.columnMapper.put("dataErrorDateTime", "dataErrorDateTime");

		this.createBy = createBy;
		this.columnMapper.put("createBy", "createBy");

		this.createDateTime = createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.updateBy = updateBy;
		this.columnMapper.put("updateBy", "updateBy");

		this.updateDateTime = updateDateTime;
		this.columnMapper.put("updateDateTime", "updateDateTime");

		this.misMatchDetail = misMatchDetail;
		this.columnMapper.put("misMatchDetail", "misMatchDetail");

	}

	public String getTransactionId() {
		return transactionId;
	}

	public void setTransactionId(String transactionId) {
		this.transactionId = transactionId;
	}

	public String getTransactionType() {
		return transactionType;
	}

	public void setTransactionType(String transactionType) {
		this.transactionType = transactionType;
	}

	public String getRequestNo() {
		return requestNo;
	}

	public void setRequestNo(String requestNo) {
		this.requestNo = requestNo;
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String company) {
		this.company = company;
	}

	public String getDataErrorDateTime() {
		return dataErrorDateTime;
	}

	public void setDataErrorDateTime(String dataErrorDateTime) {
		this.dataErrorDateTime = dataErrorDateTime;
	}

	public String getCreateBy() {
		return createBy;
	}

	public void setCreateBy(String createBy) {
		this.createBy = createBy;
	}

	public String getCreateDateTime() {
		return createDateTime;
	}

	public void setCreateDateTime(String createDateTime) {
		this.createDateTime = createDateTime;
	}

	public String getUpdateBy() {
		return updateBy;
	}

	public void setUpdateBy(String updateBy) {
		this.updateBy = updateBy;
	}

	public String getUpdateDateTime() {
		return updateDateTime;
	}

	public void setUpdateDateTime(String updateDateTime) {
		this.updateDateTime = updateDateTime;
	}

	public String getMisMatchDetail() {
		return misMatchDetail;
	}

	public void setMisMatchDetail(String misMatchDetail) {
		this.misMatchDetail = misMatchDetail;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
