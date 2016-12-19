package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class PhoenixOrder extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "orderNo")
	private String orderNo;

	@Column(name = "orderType")
	private String orderType;

	@Column(name = "mobileNo")
	private String mobileNo;

	@Column(name = "simSerialNo")
	private String simSerialNo;

	@Column(name = "StatusCd")
	private String StatusCd;

	@Column(name = "ChargeType")
	private String ChargeType;

	@Column(name = "ErrorDestination")
	private String ErrorDestination;

	@Column(name = "LocationCode")
	private String LocationCode;

	@Column(name = "BaCategory")
	private String BaCategory;

	@Column(name = "BaSegment")
	private String BaSegment;

	@Column(name = "BillingSystem")
	private String BillingSystem;

	@Column(name = "PromotionCode")
	private String PromotionCode;

	@Column(name = "ServiceCode")
	private String ServiceCode;

	@Column(name = "WoItemErrorMsg")
	private String WoItemErrorMsg;

	@Column(name = "DcpDestination")
	private String DcpDestination;

	@Column(name = "DcpDestinationStatus")
	private String DcpDestinationStatus;

	@Column(name = "BaNo")
	private String BaNo;

	@Column(name = "CaNo")
	private String CaNo;

	@Column(name = "SaNo")
	private String SaNo;
	@Column(name = "rowId")
	private String rowId;

	@Column(name = "created")
	private String created;

	@Column(name = "createdBy")
	private String createdBy;

	@Column(name = "lastUpd")
	private String lastUpd;

	@Column(name = "lastUpdBy")
	private String lastUpdBy;

	@Column(name = "accntId")
	private String accntId;

	@Column(name = "orderDt")
	private String orderDt;

	@Column(name = "orderChannel")
	private String orderChannel;

	@Column(name = "submitedDt")
	private String submitedDt;

	@Column(name = "completedDt")
	private String completedDt;

	@Column(name = "locationCd")
	private String locationCd;

	@Column(name = "statusCd")
	private String statusCd;

	@Column(name = "statusDt")
	private String statusDt;

	@Column(name = "priorityCd")
	private String priorityCd;

	@Column(name = "modificationNum")
	private String modificationNum;

	@Column(name = "sffOrderFees")
	private String sffOrderFees;

	@Column(name = "sffOrderDocumentTrackings")
	private String sffOrderDocumentTrackings;

	@Column(name = "sffOrderServiceInstances")
	private String sffOrderServiceInstances;

	@Column(name = "sffOrderAccountPromotions")
	private String sffOrderAccountPromotions;

	@Column(name = "locationId")
	private String locationId;

	@Column(name = "smartCardFlg")
	private String smartCardFlg;

	public PhoenixOrder() {

	}

	public PhoenixOrder(String orderNo, String orderType, String mobileNo,
			String simSerialNo, String StatusCd, String ChargeType,
			String ErrorDestination, String LocationCode, String BaCategory,
			String BaSegment, String BillingSystem, String PromotionCode,
			String ServiceCode, String WoItemErrorMsg, String DcpDestination,
			String DcpDestinationStatus, String BaNo, String CaNo, String SaNo,
			String rowId, String created, String createdBy, String lastUpd,
			String lastUpdBy, String accntId, String orderDt,
			String orderChannel, String submitedDt, String completedDt,
			String locationCd, String statusCd, String statusDt,
			String priorityCd, String modificationNum, String sffOrderFees,
			String sffOrderDocumentTrackings, String sffOrderServiceInstances,
			String sffOrderAccountPromotions, String locationId,
			String smartCardFlg) {

		this.orderNo = orderNo;
		this.columnMapper.put("orderNo", "orderNo");

		this.orderType = orderType;
		this.columnMapper.put("orderType", "orderType");

		this.mobileNo = mobileNo;
		this.columnMapper.put("mobileNo", "mobileNo");

		this.simSerialNo = simSerialNo;
		this.columnMapper.put("simSerialNo", "simSerialNo");

		this.StatusCd = StatusCd;
		this.columnMapper.put("StatusCd", "StatusCd");

		this.ChargeType = ChargeType;
		this.columnMapper.put("ChargeType", "ChargeType");

		this.ErrorDestination = ErrorDestination;
		this.columnMapper.put("ErrorDestination", "ErrorDestination");

		this.LocationCode = LocationCode;
		this.columnMapper.put("LocationCode", "LocationCode");

		this.BaCategory = BaCategory;
		this.columnMapper.put("BaCategory", "BaCategory");

		this.BaSegment = BaSegment;
		this.columnMapper.put("BaSegment", "BaSegment");

		this.BillingSystem = BillingSystem;
		this.columnMapper.put("BillingSystem", "BillingSystem");

		this.PromotionCode = PromotionCode;
		this.columnMapper.put("PromotionCode", "PromotionCode");

		this.ServiceCode = ServiceCode;
		this.columnMapper.put("ServiceCode", "ServiceCode");

		this.WoItemErrorMsg = WoItemErrorMsg;
		this.columnMapper.put("WoItemErrorMsg", "WoItemErrorMsg");

		this.DcpDestination = DcpDestination;
		this.columnMapper.put("DcpDestination", "DcpDestination");

		this.DcpDestinationStatus = DcpDestinationStatus;
		this.columnMapper.put("DcpDestinationStatus", "DcpDestinationStatus");

		this.BaNo = BaNo;
		this.columnMapper.put("BaNo", "BaNo");

		this.CaNo = CaNo;
		this.columnMapper.put("CaNo", "CaNo");

		this.SaNo = SaNo;
		this.columnMapper.put("SaNo", "SaNo");

		this.rowId = rowId;
		this.columnMapper.put("rowId", "rowId");

		this.created = created;
		this.columnMapper.put("created", "created");

		this.createdBy = createdBy;
		this.columnMapper.put("createdBy", "createdBy");

		this.lastUpd = lastUpd;
		this.columnMapper.put("lastUpd", "lastUpd");

		this.lastUpdBy = lastUpdBy;
		this.columnMapper.put("lastUpdBy", "lastUpdBy");

		this.accntId = accntId;
		this.columnMapper.put("accntId", "accntId");

		this.orderDt = orderDt;
		this.columnMapper.put("orderDt", "orderDt");

		this.orderChannel = orderChannel;
		this.columnMapper.put("orderChannel", "orderChannel");

		this.submitedDt = submitedDt;
		this.columnMapper.put("submitedDt", "submitedDt");

		this.completedDt = completedDt;
		this.columnMapper.put("completedDt", "completedDt");

		this.locationCd = locationCd;
		this.columnMapper.put("locationCd", "locationCd");

		this.statusCd = statusCd;
		this.columnMapper.put("statusCd", "statusCd");

		this.statusDt = statusDt;
		this.columnMapper.put("statusDt", "statusDt");

		this.priorityCd = priorityCd;
		this.columnMapper.put("priorityCd", "priorityCd");

		this.modificationNum = modificationNum;
		this.columnMapper.put("modificationNum", "modificationNum");

		this.sffOrderFees = sffOrderFees;
		this.columnMapper.put("sffOrderFees", "sffOrderFees");

		this.sffOrderDocumentTrackings = sffOrderDocumentTrackings;
		this.columnMapper.put("sffOrderDocumentTrackings",
				"sffOrderDocumentTrackings");

		this.sffOrderServiceInstances = sffOrderServiceInstances;
		this.columnMapper.put("sffOrderServiceInstances",
				"sffOrderServiceInstances");

		this.sffOrderAccountPromotions = sffOrderAccountPromotions;
		this.columnMapper.put("sffOrderAccountPromotions",
				"sffOrderAccountPromotions");

		this.locationId = locationId;
		this.columnMapper.put("locationId", "locationId");

		this.smartCardFlg = smartCardFlg;
		this.columnMapper.put("smartCardFlg", "smartCardFlg");
	}

	public String getOrderNo() {
		return orderNo;
	}

	public void setOrderNo(String orderNo) {
		this.orderNo = orderNo;
	}

	public String getOrderType() {
		return orderType;
	}

	public void setOrderType(String orderType) {
		this.orderType = orderType;
	}

	public String getMobileNo() {
		return mobileNo;
	}

	public void setMobileNo(String mobileNo) {
		this.mobileNo = mobileNo;
	}

	public String getSimSerialNo() {
		return simSerialNo;
	}

	public void setSimSerialNo(String simSerialNo) {
		this.simSerialNo = simSerialNo;
	}

	public String getStatusCd() {
		return StatusCd;
	}

	public void setStatusCd(String statusCd) {
		StatusCd = statusCd;
	}

	public String getChargeType() {
		return ChargeType;
	}

	public void setChargeType(String chargeType) {
		ChargeType = chargeType;
	}

	public String getErrorDestination() {
		return ErrorDestination;
	}

	public void setErrorDestination(String errorDestination) {
		ErrorDestination = errorDestination;
	}

	public String getLocationCode() {
		return LocationCode;
	}

	public void setLocationCode(String locationCode) {
		LocationCode = locationCode;
	}

	public String getBaCategory() {
		return BaCategory;
	}

	public void setBaCategory(String baCategory) {
		BaCategory = baCategory;
	}

	public String getBaSegment() {
		return BaSegment;
	}

	public void setBaSegment(String baSegment) {
		BaSegment = baSegment;
	}

	public String getBillingSystem() {
		return BillingSystem;
	}

	public void setBillingSystem(String billingSystem) {
		BillingSystem = billingSystem;
	}

	public String getPromotionCode() {
		return PromotionCode;
	}

	public void setPromotionCode(String promotionCode) {
		PromotionCode = promotionCode;
	}

	public String getServiceCode() {
		return ServiceCode;
	}

	public void setServiceCode(String serviceCode) {
		ServiceCode = serviceCode;
	}

	public String getWoItemErrorMsg() {
		return WoItemErrorMsg;
	}

	public void setWoItemErrorMsg(String woItemErrorMsg) {
		WoItemErrorMsg = woItemErrorMsg;
	}

	public String getDcpDestination() {
		return DcpDestination;
	}

	public void setDcpDestination(String dcpDestination) {
		DcpDestination = dcpDestination;
	}

	public String getDcpDestinationStatus() {
		return DcpDestinationStatus;
	}

	public void setDcpDestinationStatus(String dcpDestinationStatus) {
		DcpDestinationStatus = dcpDestinationStatus;
	}

	public String getBaNo() {
		return BaNo;
	}

	public void setBaNo(String baNo) {
		BaNo = baNo;
	}

	public String getCaNo() {
		return CaNo;
	}

	public void setCaNo(String caNo) {
		CaNo = caNo;
	}

	public String getSaNo() {
		return SaNo;
	}

	public void setSaNo(String saNo) {
		SaNo = saNo;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

	public String getRowId() {
		return rowId;
	}

	public void setRowId(String rowId) {
		this.rowId = rowId;
	}

	public String getCreated() {
		return created;
	}

	public void setCreated(String created) {
		this.created = created;
	}

	public String getCreatedBy() {
		return createdBy;
	}

	public void setCreatedBy(String createdBy) {
		this.createdBy = createdBy;
	}

	public String getLastUpd() {
		return lastUpd;
	}

	public void setLastUpd(String lastUpd) {
		this.lastUpd = lastUpd;
	}

	public String getLastUpdBy() {
		return lastUpdBy;
	}

	public void setLastUpdBy(String lastUpdBy) {
		this.lastUpdBy = lastUpdBy;
	}

	public String getAccntId() {
		return accntId;
	}

	public void setAccntId(String accntId) {
		this.accntId = accntId;
	}

	public String getOrderDt() {
		return orderDt;
	}

	public void setOrderDt(String orderDt) {
		this.orderDt = orderDt;
	}

	public String getOrderChannel() {
		return orderChannel;
	}

	public void setOrderChannel(String orderChannel) {
		this.orderChannel = orderChannel;
	}

	public String getSubmitedDt() {
		return submitedDt;
	}

	public void setSubmitedDt(String submitedDt) {
		this.submitedDt = submitedDt;
	}

	public String getCompletedDt() {
		return completedDt;
	}

	public void setCompletedDt(String completedDt) {
		this.completedDt = completedDt;
	}

	public String getLocationCd() {
		return locationCd;
	}

	public void setLocationCd(String locationCd) {
		this.locationCd = locationCd;
	}

	public String getStatusDt() {
		return statusDt;
	}

	public void setStatusDt(String statusDt) {
		this.statusDt = statusDt;
	}

	public String getPriorityCd() {
		return priorityCd;
	}

	public void setPriorityCd(String priorityCd) {
		this.priorityCd = priorityCd;
	}

	public String getModificationNum() {
		return modificationNum;
	}

	public void setModificationNum(String modificationNum) {
		this.modificationNum = modificationNum;
	}

	public String getSffOrderFees() {
		return sffOrderFees;
	}

	public void setSffOrderFees(String sffOrderFees) {
		this.sffOrderFees = sffOrderFees;
	}

	public String getSffOrderDocumentTrackings() {
		return sffOrderDocumentTrackings;
	}

	public void setSffOrderDocumentTrackings(String sffOrderDocumentTrackings) {
		this.sffOrderDocumentTrackings = sffOrderDocumentTrackings;
	}

	public String getSffOrderServiceInstances() {
		return sffOrderServiceInstances;
	}

	public void setSffOrderServiceInstances(String sffOrderServiceInstances) {
		this.sffOrderServiceInstances = sffOrderServiceInstances;
	}

	public String getSffOrderAccountPromotions() {
		return sffOrderAccountPromotions;
	}

	public void setSffOrderAccountPromotions(String sffOrderAccountPromotions) {
		this.sffOrderAccountPromotions = sffOrderAccountPromotions;
	}

	public String getLocationId() {
		return locationId;
	}

	public void setLocationId(String locationId) {
		this.locationId = locationId;
	}

	public String getSmartCardFlg() {
		return smartCardFlg;
	}

	public void setSmartCardFlg(String smartCardFlg) {
		this.smartCardFlg = smartCardFlg;
	}

}
