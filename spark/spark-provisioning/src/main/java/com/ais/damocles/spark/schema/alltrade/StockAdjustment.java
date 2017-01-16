package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

import java.util.List;

public class StockAdjustment extends com.ais.damocles.spark.schema.CommonSchema {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "transactionId")
	private String transactionId;

	@Column(name = "transactionType ")
	private String transactionType;

	@Column(name = "adjustStockNo")
	private String adjustStockNo;

	@Column(name = "company")
	private String company;

	@Column(name = "memoNo")
	private String memoNo;

	@Column(name = "remark")
	private String remark;

	@Column(name = "adjustDateTime")
	private String adjustDateTime;

	@Column(name = "mmDocYear")
	private String mmDocYear;

	@Column(name = "mmDocNo")
	private String mmDocNo;

	@Column(name = "docRef")
	private String docRef;

	@Column(name = "moveMentType")
	private String moveMentType;

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

	@Column(name = "adjustStatus")
	private String adjustStatus;

	@Column(name = "createBy")
	private String createBy;

	@Column(name = "createDateTime")
	private String createDateTime;

	@Column(name = "updateBy")
	private String updateBy;

	@Column(name = "updateDateTime")
	private String updateDateTime;

	@Column(name = "matCode_key")
	private List<Object> matCode_key;

	@Column(name = "brand_key")
	private List<Object> brand_key;

	@Column(name = "model_key")
	private List<Object> model_key;

	@Column(name = "commercialName_key")
	private List<Object> commercialName_key;

	@Column(name = "productType_key")
	private List<Object> productType_key;

	@Column(name = "productSubType_key")
	private List<Object> productSubType_key;

	@Column(name = "serial_key")
	private List<Object> serial_key;

	@Column(name = "adjustDetail")
	private String adjustDetail;

	@Column(name = "adjustQty")
	private String adjustQty;

	@Column(name = "adjustType")
	private String adjustType;

	public StockAdjustment() {

	}

	public StockAdjustment(String transactionId, String transactionType,
						   String adjustStockNo, String company, String memoNo, String remark,
						   String adjustDateTime, String mmDocYear, String mmDocNo,
						   String docRef, String moveMentType, String locationCode,
						   String locationName, String locationType, String locationSubType,
						   String locationRegion, String locationProvince,
						   String adjustStatus, String createBy, String createDateTime,
						   String updateBy, String updateDateTime, List<Object> matCode_key,
						   List<Object> brand_key, List<Object> model_key, List<Object> commercialName_key,
						   List<Object> productType_key, List<Object> productSubType_key,
						   List<Object> serial_key, String adjustDetail, String adjustType
	, String adjustQty) {

		this.adjustType= adjustType;
		this.columnMapper.put("adjustType","adjustType");

		this.adjustQty = adjustQty;
		this.columnMapper.put("adjustQty","adjustQty");

		this.transactionId = transactionId;
		this.columnMapper.put("transactionId", "transactionId");

		this.transactionType = transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.adjustStockNo = adjustStockNo;
		this.columnMapper.put("adjustStockNo", "adjustStockNo");

		this.company = company;
		this.columnMapper.put("company", "company");

		this.memoNo = memoNo;
		this.columnMapper.put("memoNo", "memoNo");

		this.remark = remark;
		this.columnMapper.put("remark", "remark");

		this.adjustDateTime = adjustDateTime;
		this.columnMapper.put("adjustDateTime", "adjustDateTime");

		this.mmDocYear = mmDocYear;
		this.columnMapper.put("mmDocYear", "mmDocYear");

		this.mmDocNo = mmDocNo;
		this.columnMapper.put("mmDocNo", "mmDocNo");

		this.docRef = docRef;
		this.columnMapper.put("docRef", "docRef");

		this.moveMentType = moveMentType;
		this.columnMapper.put("moveMentType", "moveMentType");

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

		this.adjustStatus = adjustStatus;
		this.columnMapper.put("adjustStatus", "adjustStatus");

		this.createBy = createBy;
		this.columnMapper.put("createBy", "createBy");

		this.createDateTime = createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.updateBy = updateBy;
		this.columnMapper.put("updateBy", "updateBy");

		this.updateDateTime = updateDateTime;
		this.columnMapper.put("updateDateTime", "updateDateTime");

		this.matCode_key = matCode_key;
		this.columnMapper.put("matCode_key", "matCode_key");

		this.brand_key = brand_key;
		this.columnMapper.put("brand_key", "brand_key");

		this.model_key = model_key;
		this.columnMapper.put("model_key", "model_key");

		this.commercialName_key = commercialName_key;
		this.columnMapper.put("commercialName_key", "commercialName_key");

		this.productType_key = productType_key;
		this.columnMapper.put("productType_key", "productType_key");

		this.productSubType_key = productSubType_key;
		this.columnMapper.put("productSubType_key", "productSubType_key");

		this.serial_key = serial_key;
		this.columnMapper.put("serial_key", "serial_key");

		this.adjustDetail = adjustDetail;
		this.columnMapper.put("adjustDetail", "adjustDetail");

	}

	public String getAdjustQty() {
		return adjustQty;
	}

	public void setAdjustQty(String adjustQty) {
		this.adjustQty = adjustQty;
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

	public String getAdjustStockNo() {
		return adjustStockNo;
	}

	public void setAdjustStockNo(String adjustStockNo) {
		this.adjustStockNo = adjustStockNo;
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String company) {
		this.company = company;
	}

	public String getMemoNo() {
		return memoNo;
	}

	public void setMemoNo(String memoNo) {
		this.memoNo = memoNo;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getAdjustDateTime() {
		return adjustDateTime;
	}

	public void setAdjustDateTime(String adjustDateTime) {
		this.adjustDateTime = adjustDateTime;
	}

	public String getMmDocYear() {
		return mmDocYear;
	}

	public void setMmDocYear(String mmDocYear) {
		this.mmDocYear = mmDocYear;
	}

	public String getMmDocNo() {
		return mmDocNo;
	}

	public void setMmDocNo(String mmDocNo) {
		this.mmDocNo = mmDocNo;
	}

	public String getDocRef() {
		return docRef;
	}

	public void setDocRef(String docRef) {
		this.docRef = docRef;
	}

	public String getMoveMentType() {
		return moveMentType;
	}

	public void setMoveMentType(String moveMentType) {
		this.moveMentType = moveMentType;
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

	public String getAdjustStatus() {
		return adjustStatus;
	}

	public void setAdjustStatus(String adjustStatus) {
		this.adjustStatus = adjustStatus;
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

	public List<Object> getMatCode_key() {
		return matCode_key;
	}

	public void setMatCode_key(List<Object> matCode_key) {
		this.matCode_key = matCode_key;
	}

	public List<Object> getBrand_key() {
		return brand_key;
	}

	public void setBrand_key(List<Object> brand_key) {
		this.brand_key = brand_key;
	}

	public List<Object> getModel_key() {
		return model_key;
	}

	public void setModel_key(List<Object> model_key) {
		this.model_key = model_key;
	}

	public List<Object> getCommercialName_key() {
		return commercialName_key;
	}

	public void setCommercialName_key(List<Object> commercialName_key) {
		this.commercialName_key = commercialName_key;
	}

	public List<Object> getProductType_key() {
		return productType_key;
	}

	public void setProductType_key(List<Object> productType_key) {
		this.productType_key = productType_key;
	}

	public List<Object> getProductSubType_key() {
		return productSubType_key;
	}

	public void setProductSubType_key(List<Object> productSubType_key) {
		this.productSubType_key = productSubType_key;
	}

	public List<Object> getSerial_key() {
		return serial_key;
	}

	public void setSerial_key(List<Object> serial_key) {
		this.serial_key = serial_key;
	}

	public String getAdjustDetail() {
		return adjustDetail;
	}

	public void setAdjustDetail(String adjustDetail) {
		this.adjustDetail = adjustDetail;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

	public void setAdjustType(String adjustType) {
		this.adjustType = adjustType;
	}

	public String getAdjustType() {
		return adjustType;
	}
}
