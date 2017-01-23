package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class ObtainDetail extends com.ais.damocles.spark.schema.CommonSchema {

	/**
	 *
	 */
	private static final long serialVersionUID = 1L;

	@Column(name = "transactionId")
	private String transactionId;

	@Column(name = "transactionType")
	private String transactionType;

	@Column(name = "obtainNo")
	private String obtainNo;

	@Column(name = "obtainStatus")
	private String obtainStatus;

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

	@Column(name = "docRef")
	private String docRef;

	@Column(name = "obtainDateTime")
	private String obtainDateTime;

	@Column(name = "createBy")
	private String createBy;

	@Column(name = "createDateTime")
	private String createDateTime;

	@Column(name = "updateBy")
	private String updateBy;

	@Column(name = "updateDateTime")
	private String updateDateTime;

	@Column(name = "salesCode_key")
	private String salesCode_key;

	@Column(name = "matCode_key")
	private String matCode_key;

	@Column(name = "serial_key")
	private String serial_key;

	@Column(name = "obtainDetail")
	private String obtainDetail;

	public ObtainDetail() {

	}

	public ObtainDetail(String transactionId, String transactionType,
						String obtainNo, String obtainStatus, String locationCode,
						String locationName, String locationType, String locationSubType,
						String locationRegion, String locationProvince, String docRef,
						String obtainDateTime, String createBy, String createDateTime,
						String updateBy, String updateDateTime, String salesCode_key,
						String matCode_key, String serial_key, String obtainDetail) {


		this.transactionId = transactionId;
		this.columnMapper.put("transactionId", "transactionId");

		this.transactionType = transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.obtainNo = obtainNo;
		this.columnMapper.put("obtainNo", "obtainNo");

		this.obtainStatus = obtainStatus;
		this.columnMapper.put("obtainStatus", "obtainStatus");

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

		this.docRef = docRef;
		this.columnMapper.put("docRef", "docRef");

		this.obtainDateTime = obtainDateTime;
		this.columnMapper.put("obtainDateTime", "obtainDateTime");

		this.createBy = createBy;
		this.columnMapper.put("createBy", "createBy");

		this.createDateTime = createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.updateBy = updateBy;
		this.columnMapper.put("updateBy", "updateBy");

		this.updateDateTime = updateDateTime;
		this.columnMapper.put("updateDateTime", "updateDateTime");

		this.salesCode_key = salesCode_key;
		this.columnMapper.put("salesCode_key", "salesCode_key");

		this.matCode_key = matCode_key;
		this.columnMapper.put("matCode_key", "matCode_key");

		this.serial_key = serial_key;
		this.columnMapper.put("serial_key", "serial_key");

		this.obtainDetail = obtainDetail;
		this.columnMapper.put("obtainDetail", "obtainDetail");

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

	public String getObtainNo() {
		return obtainNo;
	}

	public void setObtainNo(String obtainNo) {
		this.obtainNo = obtainNo;
	}

	public String getObtainStatus() {
		return obtainStatus;
	}

	public void setObtainStatus(String obtainStatus) {
		this.obtainStatus = obtainStatus;
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

	public String getDocRef() {
		return docRef;
	}

	public void setDocRef(String docRef) {
		this.docRef = docRef;
	}

	public String getObtainDateTime() {
		return obtainDateTime;
	}

	public void setObtainDateTime(String obtainDateTime) {
		this.obtainDateTime = obtainDateTime;
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

	public String getSalesCode_key() {
		return salesCode_key;
	}

	public void setSalesCode_key(String salesCode_key) {
		this.salesCode_key = salesCode_key;
	}

	public String getMatCode_key() {
		return matCode_key;
	}

	public void setMatCode_key(String matCode_key) {
		this.matCode_key = matCode_key;
	}

	public String getSerial_key() {
		return serial_key;
	}

	public void setSerial_key(String serial_key) {
		this.serial_key = serial_key;
	}

	public String getObtainDetail() {
		return obtainDetail;
	}

	public void setObtainDetail(String obtainDetail) {
		this.obtainDetail = obtainDetail;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
