package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class SubStockDetailTransfer extends
		com.ais.damocles.spark.schema.CommonSchema {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "transactionId")
	private String transactionId;

	@Column(name = "transactionType")
	private String transactionType;

	@Column(name = "transferSubStockNo")
	private String transferSubStockNo;

	@Column(name = "company")
	private String company;

	@Column(name = "transferSubStockStatus")
	private String transferSubStockStatus;

	@Column(name = "toSubStock")
	private String toSubStock;

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

	@Column(name = "transferSubStockDateTime")
	private String transferSubStockDateTime;

	@Column(name = "createBy")
	private String createBy;

	@Column(name = "createDateTime")
	private String createDateTime;

	@Column(name = "updateBy")
	private String updateBy;

	@Column(name = "updateDateTime")
	private String updateDateTime;

	@Column(name = "totalQty")
	private String totalQty;

	@Column(name = "matCode_key")
	private String matCode_key;

	@Column(name = "serial_key")
	private String serial_key;

	@Column(name = "fromSubStock_key")
	private String fromSubStock_key;

	@Column(name = "transferSubStockDetail")
	private String transferSubStockDetail;

	public SubStockDetailTransfer() {

	}

	public SubStockDetailTransfer(String transactionId, String transactionType,
			String transferSubStockNo, String company,
			String transferSubStockStatus, String toSubStock,
			String locationCode, String locationName, String locationType,
			String locationSubType, String locationRegion,
			String locationProvince, String docRef,
			String transferSubStockDateTime, String createBy,
			String createDateTime, String updateBy, String updateDateTime,
			String totalQty, String matCode_key, String serial_key,
			String fromSubStock_key, String transferSubStockDetail) {
		this.transactionId = transactionId;
		this.columnMapper.put("transactionId", "transactionId");

		this.transactionType = transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.transferSubStockNo = transferSubStockNo;
		this.columnMapper.put("transferSubStockNo", "transferSubStockNo");

		this.company = company;
		this.columnMapper.put("company", "company");

		this.transferSubStockStatus = transferSubStockStatus;
		this.columnMapper.put("transferSubStockStatus",
				"transferSubStockStatus");

		this.toSubStock = toSubStock;
		this.columnMapper.put("toSubStock", "toSubStock");

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

		this.transferSubStockDateTime = transferSubStockDateTime;
		this.columnMapper.put("transferSubStockDateTime",
				"transferSubStockDateTime");

		this.createBy = createBy;
		this.columnMapper.put("createBy", "createBy");

		this.createDateTime = createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.updateBy = updateBy;
		this.columnMapper.put("updateBy", "updateBy");

		this.updateDateTime = updateDateTime;
		this.columnMapper.put("updateDateTime", "updateDateTime");

		this.totalQty = totalQty;
		this.columnMapper.put("totalQty", "totalQty");

		this.matCode_key = matCode_key;
		this.columnMapper.put("matCode_key", "matCode_key");

		this.serial_key = serial_key;
		this.columnMapper.put("serial_key", "serial_key");

		this.fromSubStock_key = fromSubStock_key;
		this.columnMapper.put("fromSubStock_key", "fromSubStock_key");

		this.transferSubStockDetail = transferSubStockDetail;
		this.columnMapper.put("transferSubStockDetail",
				"transferSubStockDetail");

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

	public String getTransferSubStockNo() {
		return transferSubStockNo;
	}

	public void setTransferSubStockNo(String transferSubStockNo) {
		this.transferSubStockNo = transferSubStockNo;
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String company) {
		this.company = company;
	}

	public String getTransferSubStockStatus() {
		return transferSubStockStatus;
	}

	public void setTransferSubStockStatus(String transferSubStockStatus) {
		this.transferSubStockStatus = transferSubStockStatus;
	}

	public String getToSubStock() {
		return toSubStock;
	}

	public void setToSubStock(String toSubStock) {
		this.toSubStock = toSubStock;
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

	public String getTransferSubStockDateTime() {
		return transferSubStockDateTime;
	}

	public void setTransferSubStockDateTime(String transferSubStockDateTime) {
		this.transferSubStockDateTime = transferSubStockDateTime;
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

	public String getTotalQty() {
		return totalQty;
	}

	public void setTotalQty(String totalQty) {
		this.totalQty = totalQty;
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

	public String getFromSubStock_key() {
		return fromSubStock_key;
	}

	public void setFromSubStock_key(String fromSubStock_key) {
		this.fromSubStock_key = fromSubStock_key;
	}

	public String getTransferSubStockDetail() {
		return transferSubStockDetail;
	}

	public void setTransferSubStockDetail(String transferSubStockDetail) {
		this.transferSubStockDetail = transferSubStockDetail;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
