package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class TransferDetailReport extends com.ais.damocles.spark.schema.CommonSchema {

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

    @Column(name = "transferFromLocationCodeFrom")
    private String transferFromLocationCodeFrom;

    @Column(name = "transferToLocationCodeTo")
    private String transferToLocationCodeTo;

    @Column(name = "transferInStatus")
    private String transferInStatus;

    @Column(name = "transferOutStatus")
    private String transferOutStatus;

    @Column(name = "company")
    private String company;

    @Column(name = "fromLocationCode")
    private String fromLocationCode;

    @Column(name = "fromLocationName")
    private String fromLocationName;

    @Column(name = "toLocationCode")
    private String toLocationCode;

    @Column(name = "toLocationName")
    private String toLocationName;

    @Column(name = "transferOutDateTime")
    private String transferOutDateTime;

    @Column(name = "transferOutNo")
    private String transferOutNo;

    @Column(name = "transferInDateTime")
    private String transferInDateTime;

    @Column(name = "transferInNo")
    private String transferInNo;

    @Column(name = "requestDateTime")
    private String requestDateTime;

    @Column(name = "requestNo")
    private String requestNo;

    @Column(name = "forSubStock")
    private String forSubStock;

    @Column(name = "transferOutStatus")
    private String transferOutStatus;

    @Column(name = "brand")
    private String brand;

    @Column(name = "model")
    private String model;

    @Column(name = "matCode")
    private String matCode;

    @Column(name = "matDescription")
    private String matDescription;

    @Column(name = "matType")
    private String matType;

    @Column(name = "fromSubStock")
    private String fromSubStock;

    @Column(name = "toSubStock")
    private String toSubStock;

    @Column(name = "qtyOut")
    private String qtyOut;

    @Column(name = "qtyIn")
    private String qtyIn;

    @Column(name = "createBy")
    private String createBy;

    @Column(name = "updateBy")
    private String updateBy;

    @Column(name = "reason")
    private String reason;

    @Column(name = "shipToCode")
    private String shipToCode;

    @Column(name = "shipToName")
    private String shipToName;

    @Column(name = "matCodeNo")
    private String matCodeNo;

    @Column(name = "doNo")
    private String doNo;

    @Column(name = "pickingDateTime")
    private String pickingDateTime;

    @Column(name = "salesCode")
    private String salesCode;

    @Column(name = "salesName")
    private String salesName;

    @Column(name = "vendorCode")
    private String vendorCode;

    @Column(name = "vendorName")
    private String vendorName;

    public TransferDetailReport() {
    }

    public TransferDetailReport(String reportName, String transactionType, String createDateTimeFrom,
                                  String createDateTimeTo, String fromLocationCodeList, String fromLocationCodeFrom,
                                  String fromLocationCodeTo, String locationType, String locationSubType,
                                  String region, String transferFromLocationCodeFrom, String transferToLocationCodeTo,
                                  String transferInStatus, String transferOutStatus, String company,
                                  String fromLocationCode, String fromLocationName, String toLocationCode,
                                  String toLocationName, String transferOutDateTime, String transferOutNo,
                                  String transferInDateTime, String transferInNo, String requestDateTime,
                                  String requestNo, String forSubStock, String transferOutStatus1, String brand,
                                  String model, String matCode, String matDescription, String matType,
                                  String fromSubStock, String toSubStock, String qtyOut, String qtyIn,
                                  String createBy, String updateBy, String reason, String shipToCode,
                                  String shipToName, String matCodeNo, String doNo, String pickingDateTime,
                                  String salesCode, String salesName, String vendorCode, String vendorName) {

        this.reportName = reportName;
        this.columnMapper.put("createdBy", "createdBy");

        this.transactionType = transactionType;
        this.columnMapper.put("transactionType", "transactionType");

        this.createDateTimeFrom = createDateTimeFrom;
        this.columnMapper.put("createDateTimeFrom", "createDateTimeFrom");

        this.createDateTimeTo = createDateTimeTo;
        this.columnMapper.put("createDateTimeTo", "createDateTimeTo");

        this.fromLocationCodeList = fromLocationCodeList;
        this.columnMapper.put("fromLocationCodeList", "fromLocationCodeList");

        this.fromLocationCodeFrom = fromLocationCodeFrom;
        this.columnMapper.put("fromLocationCodeFrom", "fromLocationCodeFrom");

        this.fromLocationCodeTo = fromLocationCodeTo;
        this.columnMapper.put("fromLocationCodeTo", "fromLocationCodeTo");

        this.locationType = locationType;
        this.columnMapper.put("locationType", "locationType");

        this.locationSubType = locationSubType;
        this.columnMapper.put("locationSubType", "locationSubType");

        this.region = region;
        this.columnMapper.put("region", "region");

        this.transferFromLocationCodeFrom = transferFromLocationCodeFrom;
        this.columnMapper.put("transferFromLocationCodeFrom", "transferFromLocationCodeFrom");

        this.transferToLocationCodeTo = transferToLocationCodeTo;
        this.columnMapper.put("transferToLocationCodeTo", "transferToLocationCodeTo");

        this.transferInStatus = transferInStatus;
        this.columnMapper.put("transferInStatus", "transferInStatus");

        this.transferOutStatus = transferOutStatus;
        this.columnMapper.put("transferOutStatus", "transferOutStatus");

        this.company = company;
        this.columnMapper.put("company", "company");

        this.fromLocationCode = fromLocationCode;
        this.columnMapper.put("fromLocationCode", "fromLocationCode");

        this.fromLocationName = fromLocationName;
        this.columnMapper.put("fromLocationName", "fromLocationName");

        this.toLocationCode = toLocationCode;
        this.columnMapper.put("toLocationCode", "toLocationCode");

        this.toLocationName = toLocationName;
        this.columnMapper.put("toLocationName", "toLocationName");

        this.transferOutDateTime = transferOutDateTime;
        this.columnMapper.put("transferOutDateTime", "transferOutDateTime");

        this.transferOutNo = transferOutNo;
        this.columnMapper.put("transferOutNo", "transferOutNo");

        this.transferInDateTime = transferInDateTime;
        this.columnMapper.put("transferInDateTime", "transferInDateTime");

        this.transferInNo = transferInNo;
        this.columnMapper.put("transferInNo", "transferInNo");

        this.requestDateTime = requestDateTime;
        this.columnMapper.put("requestDateTime", "requestDateTime");

        this.requestNo = requestNo;
        this.columnMapper.put("requestNo", "requestNo");

        this.forSubStock = forSubStock;
        this.columnMapper.put("forSubStock", "forSubStock");

        this.transferOutStatus = transferOutStatus1;
        this.columnMapper.put("transferOutStatus1", "transferOutStatus1");

        this.brand = brand;
        this.columnMapper.put("brand", "brand");

        this.model = model;
        this.columnMapper.put("model", "model");

        this.matCode = matCode;
        this.columnMapper.put("matCode", "matCode");

        this.matDescription = matDescription;
        this.columnMapper.put("matDescription", "matDescription");

        this.matType = matType;
        this.columnMapper.put("matType", "matType");

        this.fromSubStock = fromSubStock;
        this.columnMapper.put("fromSubStock", "fromSubStock");

        this.toSubStock = toSubStock;
        this.columnMapper.put("toSubStock", "toSubStock");

        this.qtyOut = qtyOut;
        this.columnMapper.put("qtyOut", "qtyOut");

        this.qtyIn = qtyIn;
        this.columnMapper.put("qtyIn", "qtyIn");

        this.createBy = createBy;
        this.columnMapper.put("createBy", "createBy");

        this.updateBy = updateBy;
        this.columnMapper.put("updateBy", "updateBy");

        this.reason = reason;
        this.columnMapper.put("reason", "reason");

        this.shipToCode = shipToCode;
        this.columnMapper.put("shipToCode", "shipToCode");

        this.shipToName = shipToName;
        this.columnMapper.put("shipToName", "shipToName");

        this.matCodeNo = matCodeNo;
        this.columnMapper.put("matCodeNo", "matCodeNo");

        this.doNo = doNo;
        this.columnMapper.put("doNo", "doNo");

        this.pickingDateTime = pickingDateTime;
        this.columnMapper.put("pickingDateTime", "pickingDateTime");

        this.salesCode = salesCode;
        this.columnMapper.put("salesCode", "salesCode");

        this.salesName = salesName;
        this.columnMapper.put("salesName", "salesName");

        this.vendorCode = vendorCode;
        this.columnMapper.put("vendorCode", "vendorCode");

        this.vendorName = vendorName;
        this.columnMapper.put("vendorName", "vendorName");

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

    public String getTransferFromLocationCodeFrom() {
        return transferFromLocationCodeFrom;
    }

    public void setTransferFromLocationCodeFrom(String transferFromLocationCodeFrom) {
        this.transferFromLocationCodeFrom = transferFromLocationCodeFrom;
    }

    public String getTransferToLocationCodeTo() {
        return transferToLocationCodeTo;
    }

    public void setTransferToLocationCodeTo(String transferToLocationCodeTo) {
        this.transferToLocationCodeTo = transferToLocationCodeTo;
    }

    public String getTransferInStatus() {
        return transferInStatus;
    }

    public void setTransferInStatus(String transferInStatus) {
        this.transferInStatus = transferInStatus;
    }

    public String getTransferOutStatus() {
        return transferOutStatus;
    }

    public void setTransferOutStatus(String transferOutStatus) {
        this.transferOutStatus = transferOutStatus;
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

    public String getMatType() {
        return matType;
    }

    public void setMatType(String matType) {
        this.matType = matType;
    }

    public String getFromSubStock() {
        return fromSubStock;
    }

    public void setFromSubStock(String fromSubStock) {
        this.fromSubStock = fromSubStock;
    }

    public String getToSubStock() {
        return toSubStock;
    }

    public void setToSubStock(String toSubStock) {
        this.toSubStock = toSubStock;
    }

    public String getQtyOut() {
        return qtyOut;
    }

    public void setQtyOut(String qtyOut) {
        this.qtyOut = qtyOut;
    }

    public String getQtyIn() {
        return qtyIn;
    }

    public void setQtyIn(String qtyIn) {
        this.qtyIn = qtyIn;
    }

    public String getCreateBy() {
        return createBy;
    }

    public void setCreateBy(String createBy) {
        this.createBy = createBy;
    }

    public String getUpdateBy() {
        return updateBy;
    }

    public void setUpdateBy(String updateBy) {
        this.updateBy = updateBy;
    }

    public String getReason() {
        return reason;
    }

    public void setReason(String reason) {
        this.reason = reason;
    }

    public String getShipToCode() {
        return shipToCode;
    }

    public void setShipToCode(String shipToCode) {
        this.shipToCode = shipToCode;
    }

    public String getShipToName() {
        return shipToName;
    }

    public void setShipToName(String shipToName) {
        this.shipToName = shipToName;
    }

    public String getMatCodeNo() {
        return matCodeNo;
    }

    public void setMatCodeNo(String matCodeNo) {
        this.matCodeNo = matCodeNo;
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

    public String getSalesCode() {
        return salesCode;
    }

    public void setSalesCode(String salesCode) {
        this.salesCode = salesCode;
    }

    public String getSalesName() {
        return salesName;
    }

    public void setSalesName(String salesName) {
        this.salesName = salesName;
    }

    public String getVendorCode() {
        return vendorCode;
    }

    public void setVendorCode(String vendorCode) {
        this.vendorCode = vendorCode;
    }

    public String getVendorName() {
        return vendorName;
    }

    public void setVendorName(String vendorName) {
        this.vendorName = vendorName;
    }

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public String getFromLocationCode() {
        return fromLocationCode;
    }

    public void setFromLocationCode(String fromLocationCode) {
        this.fromLocationCode = fromLocationCode;
    }

    public String getFromLocationName() {
        return fromLocationName;
    }

    public void setFromLocationName(String fromLocationName) {
        this.fromLocationName = fromLocationName;
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

    public String getTransferOutDateTime() {
        return transferOutDateTime;
    }

    public void setTransferOutDateTime(String transferOutDateTime) {
        this.transferOutDateTime = transferOutDateTime;
    }

    public String getTransferOutNo() {
        return transferOutNo;
    }

    public void setTransferOutNo(String transferOutNo) {
        this.transferOutNo = transferOutNo;
    }

    public String getTransferInDateTime() {
        return transferInDateTime;
    }

    public void setTransferInDateTime(String transferInDateTime) {
        this.transferInDateTime = transferInDateTime;
    }

    public String getTransferInNo() {
        return transferInNo;
    }

    public void setTransferInNo(String transferInNo) {
        this.transferInNo = transferInNo;
    }

    public String getRequestDateTime() {
        return requestDateTime;
    }

    public void setRequestDateTime(String requestDateTime) {
        this.requestDateTime = requestDateTime;
    }

    public String getRequestNo() {
        return requestNo;
    }

    public void setRequestNo(String requestNo) {
        this.requestNo = requestNo;
    }

    public String getForSubStock() {
        return forSubStock;
    }

    public void setForSubStock(String forSubStock) {
        this.forSubStock = forSubStock;
    }
}