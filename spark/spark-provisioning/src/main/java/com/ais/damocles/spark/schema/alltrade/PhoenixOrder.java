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

	public PhoenixOrder() {

	}

	public PhoenixOrder(String orderNo, String orderType, String mobileNo,
			String simSerialNo, String StatusCd, String ChargeType,
			String ErrorDestination, String LocationCode, String BaCategory,
			String BillingSystem, String PromotionCode, String ServiceCode,
			String WoItemErrorMsg, String DcpDestination,
			String DcpDestinationStatus, String BaNo, String CaNo, String SaNo) {

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

}
