package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class RequestGoodsDetailReport extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "reportName")
	private String reportName;

	@Column(name = "transactionType")
	private String transactionType;

	@Column(name = "createDateTimeFrom")
	private String createDateTimeFrom;

	@Column(name = "createDateTimeTo")
	private String createDateTimeTo;

	@Column(name = "fromLocationCodeList")
	private String fromLocationCodeList;

	@Column(name = "fromLocationCodeFrom")
	private String fromLocationCodeFrom;

	@Column(name = "fromLocationCodeTo")
	private String fromLocationCodeTo;

	@Column(name = "locationType")
	private String locationType;

	@Column(name = "locationSubType")
	private String locationSubType;

	@Column(name = "region")
	private String region;

	@Column(name = "requestStatusList")
	private String requestStatusList;

	@Column(name = "companyList")
	private String companyList;

	@Column(name = "company")
	private String company;

	@Column(name = "toLocationCode")
	private String toLocationCode;

	@Column(name = "toLocationName")
	private String toLocationName;

	@Column(name = "forSubStock")
	private String forSubStock;

	@Column(name = "createDateTime")
	private String createDateTime;

	@Column(name = "requestNo")
	private String requestNo;

	@Column(name = "requestStatus")
	private String requestStatus;

	@Column(name = "reservedNo ")
	private String reservedNo;

	@Column(name = "mmDocNo")
	private String mmDocNo;

	@Column(name = "doNo")
	private String doNo;

	@Column(name = "pickingDateTime")
	private String pickingDateTime;

	@Column(name = "transferOutNo")
	private String transferOutNo;

	@Column(name = "transferOutDateTime")
	private String transferOutDateTime;

	@Column(name = "createBy")
	private String createBy;

	@Column(name = "transferInNo")
	private String transferInNo;

	@Column(name = "transferInDateTime ")
	private String transferInDateTime;

	@Column(name = "quotaFlag")
	private String quotaFlag;

	@Column(name = "itemNo")
	private String itemNo;

	@Column(name = "brand")
	private String brand;

	@Column(name = "model")
	private String model;

	@Column(name = "matCode")
	private String matCode;

	@Column(name = "matDescription")
	private String matDescription;

	@Column(name = "commercialName")
	private String commercialName;

	@Column(name = "qtyReq,")
	private String qtyReq;

	@Column(name = "receiveBy")
	private String receiveBy;

	@Column(name = "mobileNo")
	private String mobileNo;

	@Column(name = "remark")
	private String remark;

	@Column(name = "shipToCode")
	private String shipToCode;

	@Column(name = "shipToProvince")
	private String shipToProvince;

	@Column(name = "locationCode")
	private String locationCode;

	/*
	 * TODO: add other attribute from schema
	 */
	public RequestGoodsDetailReport() {
		
	}

	public RequestGoodsDetailReport(String reportName, String transactionType,
			String createDateTimeFrom, String createDateTimeTo,
			String fromLocationCodeList, String fromLocationCodeFrom,
			String fromLocationCodeTo, String locationType,
			String locationSubType, String region, String requestStatusList,
			String companyList, String company, String toLocationCode,
			String toLocationName, String forSubStock, String createDateTime,
			String requestNo, String requestStatus, String reservedNo,
			String mmDocNo, String doNo, String pickingDateTime,
			String transferOutNo, String transferOutDateTime, String createBy,
			String transferInNo, String transferInDateTime, String quotaFlag,
			String itemNo, String brand, String model, String matCode,
			String matDescription, String commercialName, String qtyReq,
			String receiveBy, String mobileNo, String remark,
			String shipToCode, String shipToProvince, String locationCode) {
		this.reportName =reportName;
		this.columnMapper.put("reportName", "reportName");

		this.transactionType =transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.createDateTimeFrom =createDateTimeFrom;
		this.columnMapper.put("createDateTimeFrom", "createDateTimeFrom");

		this.createDateTimeTo =createDateTimeTo;
		this.columnMapper.put("createDateTimeTo", "createDateTimeTo");

		this.fromLocationCodeList =fromLocationCodeList;
		this.columnMapper.put("fromLocationCodeList", "fromLocationCodeList");

		this.fromLocationCodeFrom =fromLocationCodeFrom;
		this.columnMapper.put("fromLocationCodeFrom", "fromLocationCodeFrom");

		this.fromLocationCodeTo =fromLocationCodeTo;
		this.columnMapper.put("fromLocationCodeTo", "fromLocationCodeTo");

		this.locationType =locationType;
		this.columnMapper.put("locationType", "locationType");

		this.locationSubType =locationSubType;
		this.columnMapper.put("locationSubType", "locationSubType");

		this.region =region;
		this.columnMapper.put("region", "region");

		this.requestStatusList =requestStatusList;
		this.columnMapper.put("requestStatusList", "requestStatusList");

		this.companyList =companyList;
		this.columnMapper.put("companyList", "companyList");

		this.company =company;
		this.columnMapper.put("company", "company");

		this.toLocationCode =toLocationCode;
		this.columnMapper.put("toLocationCode", "toLocationCode");

		this.toLocationName =toLocationName;
		this.columnMapper.put("toLocationName", "toLocationName");

		this.forSubStock =forSubStock;
		this.columnMapper.put("forSubStock", "forSubStock");

		this.createDateTime =createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.requestNo =requestNo;
		this.columnMapper.put("requestNo", "requestNo");

		this.requestStatus =requestStatus;
		this.columnMapper.put("requestStatus", "requestStatus");

		this.reservedNo  =reservedNo ;
		this.columnMapper.put("reservedNo ", "reservedNo ");

		this.mmDocNo =mmDocNo;
		this.columnMapper.put("mmDocNo", "mmDocNo");

		this.doNo =doNo;
		this.columnMapper.put("doNo", "doNo");

		this.pickingDateTime =pickingDateTime;
		this.columnMapper.put("pickingDateTime", "pickingDateTime");

		this.transferOutNo =transferOutNo;
		this.columnMapper.put("transferOutNo", "transferOutNo");

		this.transferOutDateTime =transferOutDateTime;
		this.columnMapper.put("transferOutDateTime", "transferOutDateTime");

		this.createBy =createBy;
		this.columnMapper.put("createBy", "createBy");

		this.transferInNo =transferInNo;
		this.columnMapper.put("transferInNo", "transferInNo");

		this.transferInDateTime  =transferInDateTime ;
		this.columnMapper.put("transferInDateTime ", "transferInDateTime ");

		this.quotaFlag =quotaFlag;
		this.columnMapper.put("quotaFlag", "quotaFlag");

		this.itemNo =itemNo;
		this.columnMapper.put("itemNo", "itemNo");

		this.brand =brand;
		this.columnMapper.put("brand", "brand");

		this.model =model;
		this.columnMapper.put("model", "model");

		this.matCode =matCode;
		this.columnMapper.put("matCode", "matCode");

		this.matDescription =matDescription;
		this.columnMapper.put("matDescription", "matDescription");

		this.commercialName =commercialName;
		this.columnMapper.put("commercialName", "commercialName");

		this.qtyReq =qtyReq;
		this.columnMapper.put("qtyReq", "qtyReq");

		this.receiveBy =receiveBy;
		this.columnMapper.put("receiveBy", "receiveBy");

		this.mobileNo =mobileNo;
		this.columnMapper.put("mobileNo", "mobileNo");

		this.remark =remark;
		this.columnMapper.put("remark", "remark");

		this.shipToCode =shipToCode;
		this.columnMapper.put("shipToCode", "shipToCode");

		this.shipToProvince =shipToProvince;
		this.columnMapper.put("shipToProvince", "shipToProvince");

		this.locationCode =locationCode;
		this.columnMapper.put("locationCode", "locationCode");

	}

	public String getReportName() {
		return reportName;
	}

	public void setReportName(String reportName) {
		this.reportName = reportName;
	}

	public String getTransactionType() {
		return transactionType;
	}

	public void setTransactionType(String transactionType) {
		this.transactionType = transactionType;
	}

	public String getCreateDateTimeFrom() {
		return createDateTimeFrom;
	}

	public void setCreateDateTimeFrom(String createDateTimeFrom) {
		this.createDateTimeFrom = createDateTimeFrom;
	}

	public String getCreateDateTimeTo() {
		return createDateTimeTo;
	}

	public void setCreateDateTimeTo(String createDateTimeTo) {
		this.createDateTimeTo = createDateTimeTo;
	}

	public String getFromLocationCodeList() {
		return fromLocationCodeList;
	}

	public void setFromLocationCodeList(String fromLocationCodeList) {
		this.fromLocationCodeList = fromLocationCodeList;
	}

	public String getFromLocationCodeFrom() {
		return fromLocationCodeFrom;
	}

	public void setFromLocationCodeFrom(String fromLocationCodeFrom) {
		this.fromLocationCodeFrom = fromLocationCodeFrom;
	}

	public String getFromLocationCodeTo() {
		return fromLocationCodeTo;
	}

	public void setFromLocationCodeTo(String fromLocationCodeTo) {
		this.fromLocationCodeTo = fromLocationCodeTo;
	}

	public String getLocationType() {
		return locationType;
	}

	public void setLocationType(String locationType) {
		this.locationType = locationType;
	}

	public String getLocationSubType() {
		return locationSubType;
	}

	public void setLocationSubType(String locationSubType) {
		this.locationSubType = locationSubType;
	}

	public String getRegion() {
		return region;
	}

	public void setRegion(String region) {
		this.region = region;
	}

	public String getRequestStatusList() {
		return requestStatusList;
	}

	public void setRequestStatusList(String requestStatusList) {
		this.requestStatusList = requestStatusList;
	}

	public String getCompanyList() {
		return companyList;
	}

	public void setCompanyList(String companyList) {
		this.companyList = companyList;
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String company) {
		this.company = company;
	}

	public String getToLocationCode() {
		return toLocationCode;
	}

	public void setToLocationCode(String toLocationCode) {
		this.toLocationCode = toLocationCode;
	}

	public String getToLocationName() {
		return toLocationName;
	}

	public void setToLocationName(String toLocationName) {
		this.toLocationName = toLocationName;
	}

	public String getForSubStock() {
		return forSubStock;
	}

	public void setForSubStock(String forSubStock) {
		this.forSubStock = forSubStock;
	}

	public String getCreateDateTime() {
		return createDateTime;
	}

	public void setCreateDateTime(String createDateTime) {
		this.createDateTime = createDateTime;
	}

	public String getRequestNo() {
		return requestNo;
	}

	public void setRequestNo(String requestNo) {
		this.requestNo = requestNo;
	}

	public String getRequestStatus() {
		return requestStatus;
	}

	public void setRequestStatus(String requestStatus) {
		this.requestStatus = requestStatus;
	}

	public String getReservedNo() {
		return reservedNo;
	}

	public void setReservedNo(String reservedNo) {
		this.reservedNo = reservedNo;
	}

	public String getMmDocNo() {
		return mmDocNo;
	}

	public void setMmDocNo(String mmDocNo) {
		this.mmDocNo = mmDocNo;
	}

	public String getDoNo() {
		return doNo;
	}

	public void setDoNo(String doNo) {
		this.doNo = doNo;
	}

	public String getPickingDateTime() {
		return pickingDateTime;
	}

	public void setPickingDateTime(String pickingDateTime) {
		this.pickingDateTime = pickingDateTime;
	}

	public String getTransferOutNo() {
		return transferOutNo;
	}

	public void setTransferOutNo(String transferOutNo) {
		this.transferOutNo = transferOutNo;
	}

	public String getTransferOutDateTime() {
		return transferOutDateTime;
	}

	public void setTransferOutDateTime(String transferOutDateTime) {
		this.transferOutDateTime = transferOutDateTime;
	}

	public String getCreateBy() {
		return createBy;
	}

	public void setCreateBy(String createBy) {
		this.createBy = createBy;
	}

	public String getTransferInNo() {
		return transferInNo;
	}

	public void setTransferInNo(String transferInNo) {
		this.transferInNo = transferInNo;
	}

	public String getTransferInDateTime() {
		return transferInDateTime;
	}

	public void setTransferInDateTime(String transferInDateTime) {
		this.transferInDateTime = transferInDateTime;
	}

	public String getQuotaFlag() {
		return quotaFlag;
	}

	public void setQuotaFlag(String quotaFlag) {
		this.quotaFlag = quotaFlag;
	}

	public String getItemNo() {
		return itemNo;
	}

	public void setItemNo(String itemNo) {
		this.itemNo = itemNo;
	}

	public String getBrand() {
		return brand;
	}

	public void setBrand(String brand) {
		this.brand = brand;
	}

	public String getModel() {
		return model;
	}

	public void setModel(String model) {
		this.model = model;
	}

	public String getMatCode() {
		return matCode;
	}

	public void setMatCode(String matCode) {
		this.matCode = matCode;
	}

	public String getMatDescription() {
		return matDescription;
	}

	public void setMatDescription(String matDescription) {
		this.matDescription = matDescription;
	}

	public String getCommercialName() {
		return commercialName;
	}

	public void setCommercialName(String commercialName) {
		this.commercialName = commercialName;
	}

	public String getQtyReq() {
		return qtyReq;
	}

	public void setQtyReq(String qtyReq) {
		this.qtyReq = qtyReq;
	}

	public String getReceiveBy() {
		return receiveBy;
	}

	public void setReceiveBy(String receiveBy) {
		this.receiveBy = receiveBy;
	}

	public String getMobileNo() {
		return mobileNo;
	}

	public void setMobileNo(String mobileNo) {
		this.mobileNo = mobileNo;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getShipToCode() {
		return shipToCode;
	}

	public void setShipToCode(String shipToCode) {
		this.shipToCode = shipToCode;
	}

	public String getShipToProvince() {
		return shipToProvince;
	}

	public void setShipToProvince(String shipToProvince) {
		this.shipToProvince = shipToProvince;
	}

	public String getLocationCode() {
		return locationCode;
	}

	public void setLocationCode(String locationCode) {
		this.locationCode = locationCode;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}	

}
