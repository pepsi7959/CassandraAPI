package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class RequestGoodsSummaryReport extends com.ais.damocles.spark.schema.CommonSchema {

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

    @Column(name ="locationCodeList")
    private String locationCodeList;

    @Column(name = "fromLocationCode")
    private String fromLocationCode;

    @Column(name = "toLocationCode")
    private String toLocationCode;

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

    @Column(name = "toLocationName")
    private String toLocationName;

    @Column(name = "forSubStock")
    private String forSubStock;

    @Column(name = "createBy")
    private String createBy;

    @Column(name = "createDateTime")
    private String createDateTime;

    @Column(name = "requestNo")
    private String requestNo;

    @Column(name = "requestStatus")
    private String requestStatus;

    @Column(name = "OrderNo")
    private String OrderNo;

    @Column(name = "reservedNo")
    private String reservedNo;

    @Column(name = "MMDocNo")
    private String MMDocNo;

    @Column(name = "doNo")
    private String doNo;

    @Column(name = "pickingDate")
    private String pickingDate;

    @Column(name = "pickingTime")
    private String pickingTime;

    @Column(name = "TransferOutDate")
    private String TransferOutDate;

    @Column(name = "TransferOutTime")
    private String TransferOutTime;

    @Column(name = "TransferInBy")
    private String TransferInBy;

    @Column(name = "TransferInDate")
    private String TransferInDate;

    @Column(name = "TransferInTime")
    private String TransferInTime;

    @Column(name = "DeviceHandsetQty")
    private String DeviceHandsetQty;

    @Column(name = "DeviceHandsetBundleQty")
    private String DeviceHandsetBundleQty;

    @Column(name = "SIMPrepaidQty")
    private String SIMPrepaidQty;

    @Column(name = "SIMPostpaidQty")
    private String SIMPostpaidQty;

    @Column(name = "PremiumQty")
    private String PremiumQty;

    @Column(name = "OtherQty")
    private String OtherQty;

    @Column(name = "receiveBy")
    private String receiveBy;

    @Column(name = "mobileNo")
    private String mobileNo;

    @Column(name = "remark")
    private String remark;

    public RequestGoodsSummaryReport(){

    }

    public RequestGoodsSummaryReport(String reportName, String transactionType, String createDateTimeFrom,
                                     String createDateTimeTo, String locationCodeList, String fromLocationCode,
                                     String toLocationCode, String locationType, String locationSubType,
                                     String region, String requestStatusList, String companyList ,
                                     String company, String toLocationName,
                                     String forSubStock, String createBy, String createDateTime,
                                     String requestNo, String requestStatus, String OrderNo,
                                     String reservedNo, String MMDocNo, String doNo,
                                     String pickingDate, String pickingTime, String TransferOutDate,
                                     String TransferOutTime, String TransferInBy, String TransferInDate,
                                     String TransferInTime, String DeviceHandsetQty,String DeviceHandsetBundleQty,
                                     String SIMPrepaidQty,String SIMPostpaidQty,String PremiumQty,
                                     String OtherQty, String receiveBy, String mobileNo, String remark){

        this.reportName =reportName;
        this.columnMapper.put("reportName", "reportName");

        this.transactionType =transactionType;
        this.columnMapper.put("transactionType", "transactionType");

        this.createDateTimeFrom =createDateTimeFrom;
        this.columnMapper.put("createDateTimeFrom", "createDateTimeFrom");

        this.createDateTimeTo =createDateTimeTo;
        this.columnMapper.put("createDateTimeTo", "createDateTimeTo");

        this.locationCodeList =locationCodeList;
        this.columnMapper.put("locationCodeList", "locationCodeList");

        this.fromLocationCode =fromLocationCode;
        this.columnMapper.put("fromLocationCode", "fromLocationCode");

        this.toLocationCode =toLocationCode;
        this.columnMapper.put("toLocationCode", "toLocationCode");

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

        this.createBy =createBy;
        this.columnMapper.put("createBy", "createBy");

        this.createDateTime =createDateTime;
        this.columnMapper.put("createDateTime", "createDateTime");

        this.createDateTime =createDateTime;
        this.columnMapper.put("createDateTime", "createDateTime");

        this.requestStatus =requestStatus;
        this.columnMapper.put("requestStatus", "requestStatus");

        this.OrderNo =OrderNo;
        this.columnMapper.put("OrderNo", "OrderNo");

        this.reservedNo =reservedNo;
        this.columnMapper.put("reservedNo", "reservedNo");

        this.MMDocNo =MMDocNo;
        this.columnMapper.put("MMDocNo", "MMDocNo");

        this.doNo =doNo;
        this.columnMapper.put("doNo", "doNo");

        this.pickingDate =pickingDate;
        this.columnMapper.put("pickingDate", "pickingDate");

        this.pickingTime =pickingTime;
        this.columnMapper.put("pickingTime", "pickingTime");

        this.TransferOutDate =TransferOutDate;
        this.columnMapper.put("TransferOutDate", "TransferOutDate");

        this.TransferInBy =TransferInBy;
        this.columnMapper.put("TransferInBy", "TransferInBy");

        this.TransferInDate =TransferInDate;
        this.columnMapper.put("TransferInDate", "TransferInDate");

        this.TransferInTime =TransferInTime;
        this.columnMapper.put("TransferInTime", "TransferInTime");

        this.DeviceHandsetQty =DeviceHandsetQty;
        this.columnMapper.put("DeviceHandsetQty", "DeviceHandsetQty");

        this.DeviceHandsetBundleQty =DeviceHandsetBundleQty;
        this.columnMapper.put("DeviceHandsetBundleQty", "DeviceHandsetBundleQty");

        this.SIMPrepaidQty =SIMPrepaidQty;
        this.columnMapper.put("SIMPrepaidQty", "SIMPrepaidQty");

        this.SIMPostpaidQty =SIMPostpaidQty;
        this.columnMapper.put("SIMPostpaidQty", "SIMPostpaidQty");

        this.PremiumQty =PremiumQty;
        this.columnMapper.put("PremiumQty", "PremiumQty");

        this.OtherQty =OtherQty;
        this.columnMapper.put("OtherQty", "OtherQty");

        this.receiveBy =receiveBy;
        this.columnMapper.put("receiveBy", "receiveBy");

        this.mobileNo =mobileNo;
        this.columnMapper.put("mobileNo", "mobileNo");

        this.remark =remark;
        this.columnMapper.put("remark", "remark");

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

    public String getFromLocationCode() {
        return fromLocationCode;
    }

    public void setFromLocationCode(String fromLocationCode) {
        this.fromLocationCode = fromLocationCode;
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

    public String getOrderNo() {
        return OrderNo;
    }

    public void setOrderNo(String orderNo) {
        OrderNo = orderNo;
    }

    public String getReservedNo() {
        return reservedNo;
    }

    public void setReservedNo(String reservedNo) {
        this.reservedNo = reservedNo;
    }

    public String getMMDocNo() {
        return MMDocNo;
    }

    public void setMMDocNo(String MMDocNo) {
        this.MMDocNo = MMDocNo;
    }

    public String getDoNo() {
        return doNo;
    }

    public void setDoNo(String doNo) {
        this.doNo = doNo;
    }

    public String getPickingDate() {
        return pickingDate;
    }

    public void setPickingDate(String pickingDate) {
        this.pickingDate = pickingDate;
    }

    public String getPickingTime() {
        return pickingTime;
    }

    public void setPickingTime(String pickingTime) {
        this.pickingTime = pickingTime;
    }

    public String getTransferOutDate() {
        return TransferOutDate;
    }

    public void setTransferOutDate(String transferOutDate) {
        TransferOutDate = transferOutDate;
    }

    public String getTransferOutTime() {
        return TransferOutTime;
    }

    public void setTransferOutTime(String transferOutTime) {
        TransferOutTime = transferOutTime;
    }

    public String getTransferInBy() {
        return TransferInBy;
    }

    public void setTransferInBy(String transferInBy) {
        TransferInBy = transferInBy;
    }

    public String getTransferInDate() {
        return TransferInDate;
    }

    public void setTransferInDate(String transferInDate) {
        TransferInDate = transferInDate;
    }

    public String getTransferInTime() {
        return TransferInTime;
    }

    public void setTransferInTime(String transferInTime) {
        TransferInTime = transferInTime;
    }

    public String getDeviceHandsetQty() {
        return DeviceHandsetQty;
    }

    public void setDeviceHandsetQty(String deviceHandsetQty) {
        DeviceHandsetQty = deviceHandsetQty;
    }

    public String getDeviceHandsetBundleQty() {
        return DeviceHandsetBundleQty;
    }

    public void setDeviceHandsetBundleQty(String deviceHandsetBundleQty) {
        DeviceHandsetBundleQty = deviceHandsetBundleQty;
    }

    public String getSIMPrepaidQty() {
        return SIMPrepaidQty;
    }

    public void setSIMPrepaidQty(String SIMPrepaidQty) {
        this.SIMPrepaidQty = SIMPrepaidQty;
    }

    public String getSIMPostpaidQty() {
        return SIMPostpaidQty;
    }

    public void setSIMPostpaidQty(String SIMPostpaidQty) {
        this.SIMPostpaidQty = SIMPostpaidQty;
    }

    public String getPremiumQty() {
        return PremiumQty;
    }

    public void setPremiumQty(String premiumQty) {
        PremiumQty = premiumQty;
    }

    public String getOtherQty() {
        return OtherQty;
    }

    public void setOtherQty(String otherQty) {
        OtherQty = otherQty;
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

}