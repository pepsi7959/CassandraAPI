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

    @Column(name = "transferOutDate")
    private String transferOutDate;

    @Column(name = "transferOutTime")
    private String transferOutTime;

    @Column(name = "transferInBy")
    private String transferInBy;

    @Column(name = "transferInDate")
    private String transferInDate;

    @Column(name = "transferInTime")
    private String transferInTime;

    @Column(name = "deviceHandsetQty")
    private String deviceHandsetQty;

    @Column(name = "deviceHandsetBundleQty")
    private String deviceHandsetBundleQty;

    @Column(name = "simPrepaidQty")
    private String simPrepaidQty;

    @Column(name = "simPostpaidQty")
    private String simPostpaidQty;

    @Column(name = "premiumQty")
    private String premiumQty;

    @Column(name = "otherQty")
    private String otherQty;

    @Column(name = "receiveBy")
    private String receiveBy;

    @Column(name = "mobileNo")
    private String mobileNo;

    @Column(name = "remark")
    private String remark;

    @Column(name = "contactNo")
    private String contactNo;

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
                                     String OtherQty, String receiveBy, String mobileNo, String remark,
                                     String contactNo){

        this.contactNo =contactNo;
        this.columnMapper.put("contactNo", "contactNo");

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

        this.transferOutDate = transferOutDate;
        this.columnMapper.put("transferOutDate", "transferOutDate");

        this.transferInBy = transferInBy;
        this.columnMapper.put("transferInBy", "transferInBy");

        this.transferInDate = transferInDate;
        this.columnMapper.put("transferInDate", "transferInDate");

        this.transferInTime = TransferInTime;
        this.columnMapper.put("transferInTime", "transferInTime");

        this.deviceHandsetQty = deviceHandsetQty;
        this.columnMapper.put("deviceHandsetQty", "deviceHandsetQty");

        this.deviceHandsetBundleQty = deviceHandsetBundleQty;
        this.columnMapper.put("deviceHandsetBundleQty", "deviceHandsetBundleQty");

        this.simPrepaidQty = simPrepaidQty;
        this.columnMapper.put("simPrepaidQty", "simPrepaidQty");

        this.simPostpaidQty = simPostpaidQty;
        this.columnMapper.put("simPostpaidQty", "simPostpaidQty");

        this.premiumQty = premiumQty;
        this.columnMapper.put("premiumQty", "premiumQty");

        this.otherQty = otherQty;
        this.columnMapper.put("otherQty", "otherQty");

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

    public String getLocationCodeList() {
        return locationCodeList;
    }

    public void setLocationCodeList(String locationCodeList) {
        this.locationCodeList = locationCodeList;
    }

    public String getTransferOutDate() {
        return transferOutDate;
    }

    public void setTransferOutDate(String transferOutDate) {
        this.transferOutDate = transferOutDate;
    }

    public String getTransferOutTime() {
        return transferOutTime;
    }

    public void setTransferOutTime(String transferOutTime) {
        this.transferOutTime = transferOutTime;
    }

    public String getTransferInBy() {
        return transferInBy;
    }

    public void setTransferInBy(String transferInBy) {
        this.transferInBy = transferInBy;
    }

    public String getTransferInDate() {
        return transferInDate;
    }

    public void setTransferInDate(String transferInDate) {
        this.transferInDate = transferInDate;
    }

    public String getTransferInTime() {
        return transferInTime;
    }

    public void setTransferInTime(String transferInTime) {
        this.transferInTime = transferInTime;
    }

    public String getDeviceHandsetQty() {
        return deviceHandsetQty;
    }

    public void setDeviceHandsetQty(String deviceHandsetQty) {
        this.deviceHandsetQty = deviceHandsetQty;
    }

    public String getDeviceHandsetBundleQty() {
        return deviceHandsetBundleQty;
    }

    public void setDeviceHandsetBundleQty(String deviceHandsetBundleQty) {
        this.deviceHandsetBundleQty = deviceHandsetBundleQty;
    }

    public String getSimPrepaidQty() {
        return simPrepaidQty;
    }

    public void setSimPrepaidQty(String simPrepaidQty) {
        this.simPrepaidQty = simPrepaidQty;
    }

    public String getSimPostpaidQty() {
        return simPostpaidQty;
    }

    public void setSimPostpaidQty(String simPostpaidQty) {
        this.simPostpaidQty = simPostpaidQty;
    }

    public String getPremiumQty() {
        return premiumQty;
    }

    public void setPremiumQty(String premiumQty) {
        this.premiumQty = premiumQty;
    }

    public String getOtherQty() {
        return otherQty;
    }

    public void setOtherQty(String otherQty) {
        this.otherQty = otherQty;
    }

    public String getContactNo() {
        return contactNo;
    }

    public void setContactNo(String contactNo) {
        this.contactNo = contactNo;
    }
}