package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class SubStockDailyDetail extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "transactionId")
	private String transactionId;

	@Column(name = "transactionType")
	private String transactionType;

	@Column(name = "subStockDailyNo")
	private String subStockDailyNo;

	@Column(name = "dateForSales")
	private String dateForSales;

	@Column(name = "salesFor")
	private String salesFor;

	@Column(name = "dailyStatus")
	private String dailyStatus;

	@Column(name = "locationCode")
	private String locationCode;

	@Column(name = "locationName")
	private String locationName;

	@Column(name = "locationType")
	private String locationType;

	@Column(name = "locationSubType")
	private String locationSubType;

	@Column(name = "locationRegion")
	private String locationRegion;

	@Column(name = "locationProvince")
	private String locationProvince;

	@Column(name = "controllerName")
	private String controllerName;

	@Column(name = "devicePaymentName")
	private String devicePaymentName;

	@Column(name = "createBy")
	private String createBy;

	@Column(name = "createDateTime")
	private String createDateTime;

	@Column(name = "updateBy")
	private String updateBy;

	@Column(name = "updateDateTime")
	private String updateDateTime;

	@Column(name = "subStockDailyDetail")
	private String subStockDailyDetail;

	public SubStockDailyDetail() {

	}

	public SubStockDailyDetail(String transactionId, String transactionType,
			String subStockDailyNo, String dateForSales, String salesFor,
			String dailyStatus, String locationCode, String locationName,
			String locationType, String locationSubType, String locationRegion,
			String locationProvince, String controllerName,
			String devicePaymentName, String createBy, String createDateTime,
			String updateBy, String updateDateTime, String subStockDailyDetail) {

		this.transactionId = transactionId;
		this.columnMapper.put("transactionId", "transactionId");

		this.transactionType = transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.subStockDailyNo = subStockDailyNo;
		this.columnMapper.put("subStockDailyNo", "subStockDailyNo");

		this.dateForSales = dateForSales;
		this.columnMapper.put("dateForSales", "dateForSales");

		this.salesFor = salesFor;
		this.columnMapper.put("salesFor", "salesFor");

		this.dailyStatus = dailyStatus;
		this.columnMapper.put("dailyStatus", "dailyStatus");

		this.locationCode = locationCode;
		this.columnMapper.put("locationCode", "locationCode");

		this.locationName = locationName;
		this.columnMapper.put("locationName", "locationName");

		this.locationType = locationType;
		this.columnMapper.put("locationType", "locationType");

		this.locationSubType = locationSubType;
		this.columnMapper.put("locationSubType", "locationSubType");

		this.locationRegion = locationRegion;
		this.columnMapper.put("locationRegion", "locationRegion");

		this.locationProvince = locationProvince;
		this.columnMapper.put("locationProvince", "locationProvince");

		this.controllerName = controllerName;
		this.columnMapper.put("controllerName", "controllerName");

		this.devicePaymentName = devicePaymentName;
		this.columnMapper.put("devicePaymentName", "devicePaymentName");

		this.createBy = createBy;
		this.columnMapper.put("createBy", "createBy");

		this.createDateTime = createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.updateBy = updateBy;
		this.columnMapper.put("updateBy", "updateBy");

		this.updateDateTime = updateDateTime;
		this.columnMapper.put("updateDateTime", "updateDateTime");

		this.subStockDailyDetail = subStockDailyDetail;
		this.columnMapper.put("subStockDailyDetail", "subStockDailyDetail");
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

	public String getSubStockDailyNo() {
		return subStockDailyNo;
	}

	public void setSubStockDailyNo(String subStockDailyNo) {
		this.subStockDailyNo = subStockDailyNo;
	}

	public String getDateForSales() {
		return dateForSales;
	}

	public void setDateForSales(String dateForSales) {
		this.dateForSales = dateForSales;
	}

	public String getSalesFor() {
		return salesFor;
	}

	public void setSalesFor(String salesFor) {
		this.salesFor = salesFor;
	}

	public String getDailyStatus() {
		return dailyStatus;
	}

	public void setDailyStatus(String dailyStatus) {
		this.dailyStatus = dailyStatus;
	}

	public String getLocationCode() {
		return locationCode;
	}

	public void setLocationCode(String locationCode) {
		this.locationCode = locationCode;
	}

	public String getLocationName() {
		return locationName;
	}

	public void setLocationName(String locationName) {
		this.locationName = locationName;
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

	public String getLocationRegion() {
		return locationRegion;
	}

	public void setLocationRegion(String locationRegion) {
		this.locationRegion = locationRegion;
	}

	public String getLocationProvince() {
		return locationProvince;
	}

	public void setLocationProvince(String locationProvince) {
		this.locationProvince = locationProvince;
	}

	public String getControllerName() {
		return controllerName;
	}

	public void setControllerName(String controllerName) {
		this.controllerName = controllerName;
	}

	public String getDevicePaymentName() {
		return devicePaymentName;
	}

	public void setDevicePaymentName(String devicePaymentName) {
		this.devicePaymentName = devicePaymentName;
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

	public String getSubStockDailyDetail() {
		return subStockDailyDetail;
	}

	public void setSubStockDailyDetail(String subStockDailyDetail) {
		this.subStockDailyDetail = subStockDailyDetail;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
