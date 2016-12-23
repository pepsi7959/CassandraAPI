package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class AdjustStockReport extends com.ais.damocles.spark.schema.CommonSchema {

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

    @Column(name = "matCodeList")
    private String matCodeList;

    @Column(name = "serialList")
    private String serialList;

    @Column(name = "adjustStatusList")
    private String adjustStatusList;

    @Column(name = "company")
    private String company;

    @Column(name = "locationCode")
    private String locationCode;

    @Column(name = "locationName")
    private String locationName;

    @Column(name = "adjustDateTime")
    private String adjustDateTime;

    @Column(name = "adjustStockNo")
    private String adjustStockNo;

    @Column(name = "itemNo")
    private String itemNo;

    @Column(name = "commercialName")
    private String commercialName;

    @Column(name = "matCode ")
    private String matCode;

    @Column(name = "matDescription")
    private String matDescription;

    @Column(name = "productType")
    private String productType;

    @Column(name = "productSubType")
    private String productSubType;

    @Column(name = "brand")
    private String brand;

    @Column(name = "model")
    private String model;

    @Column(name = "subStock")
    private String subStock;

    @Column(name = "adjustType")
    private String adjustType;

    @Column(name = "serial ")
    private String serial;

    @Column(name = "adjustQty")
    private String adjustQty;

    @Column(name = "reason")
    private String reason;

    @Column(name = "remark")
    private String remark;

    @Column(name = "region")
    private String region;

    @Column(name = "adjustTyped")
    private String adjustTyped;

    /*
     * TODO: add other attribute from schema
     */
    public AdjustStockReport() {

    }

    public AdjustStockReport(String reportName, String transactionType,
                             String createDateTimeFrom, String createDateTimeTo,
                             String fromLocationCodeList, String fromLocationCodeFrom,
                             String fromLocationCodeTo, String locationType,
                             String locationSubType, String region, String matCodeList,
                             String serialList, String adjustStatusList, String company,
                             String locationCode, String locationName, String adjustDateTime,
                             String adjustStockNo, String itemNo, String commercialName,
                             String matCode, String matDescription, String productType,
                             String productSubType, String brand, String model, String subStock,
                             String adjustType, String serial, String adjustQty, String reason, String remark
    ) {
        this.reportName = reportName;
        this.columnMapper.put("reportName", "reportName");

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

        this.matCodeList = matCodeList;
        this.columnMapper.put("matCodeList", "matCodeList");

        this.serialList = serialList;
        this.columnMapper.put("serialList", "serialList");

        this.adjustStatusList = adjustStatusList;
        this.columnMapper.put("adjustStatusList", "adjustStatusList");

        this.company = company;
        this.columnMapper.put("company", "company");

        this.locationCode = locationCode;
        this.columnMapper.put("locationCode", "locationCode");

        this.locationName = locationName;
        this.columnMapper.put("locationName", "locationName");

        this.adjustDateTime = adjustDateTime;
        this.columnMapper.put("adjustDateTime", "adjustDateTime");

        this.adjustStockNo = adjustStockNo;
        this.columnMapper.put("adjustStockNo", "adjustStockNo");

        this.itemNo = itemNo;
        this.columnMapper.put("itemNo", "itemNo");

        this.commercialName = commercialName;
        this.columnMapper.put("commercialName", "commercialName");

        this.matCode = matCode;
        this.columnMapper.put("matCode ", "matCode ");

        this.matDescription = matDescription;
        this.columnMapper.put("matDescription", "matDescription");

        this.productType = productType;
        this.columnMapper.put("productType", "productType");

        this.productSubType = productSubType;
        this.columnMapper.put("productSubType", "productSubType");

        this.brand = brand;
        this.columnMapper.put("brand", "brand");

        this.model = model;
        this.columnMapper.put("model", "model");

        this.subStock = subStock;
        this.columnMapper.put("subStock", "subStock");

        this.adjustTyped = adjustTyped;
        this.columnMapper.put("adjustTyped", "adjustTyped");

        this.serial = serial;
        this.columnMapper.put("serial ", "serial ");

        this.adjustQty = adjustQty;
        this.columnMapper.put("adjustQty", "adjustQty");

        this.reason = reason;
        this.columnMapper.put("reason", "reason");

        this.remark = remark;
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

    public String getMatCodeList() {
        return matCodeList;
    }

    public void setMatCodeList(String matCodeList) {
        this.matCodeList = matCodeList;
    }

    public String getSerialList() {
        return serialList;
    }

    public void setSerialList(String serialList) {
        this.serialList = serialList;
    }

    public String getAdjustStatusList() {
        return adjustStatusList;
    }

    public void setAdjustStatusList(String adjustStatusList) {
        this.adjustStatusList = adjustStatusList;
    }

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
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

    public String getAdjustDateTime() {
        return adjustDateTime;
    }

    public void setAdjustDateTime(String adjustDateTime) {
        this.adjustDateTime = adjustDateTime;
    }

    public String getAdjustStockNo() {
        return adjustStockNo;
    }

    public void setAdjustStockNo(String adjustStockNo) {
        this.adjustStockNo = adjustStockNo;
    }

    public String getItemNo() {
        return itemNo;
    }

    public void setItemNo(String itemNo) {
        this.itemNo = itemNo;
    }

    public String getCommercialName() {
        return commercialName;
    }

    public void setCommercialName(String commercialName) {
        this.commercialName = commercialName;
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

    public String getProductType() {
        return productType;
    }

    public void setProductType(String productType) {
        this.productType = productType;
    }

    public String getProductSubType() {
        return productSubType;
    }

    public void setProductSubType(String productSubType) {
        this.productSubType = productSubType;
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

    public String getSubStock() {
        return subStock;
    }

    public void setSubStock(String subStock) {
        this.subStock = subStock;
    }

    public String getAdjustType() {
        return adjustType;
    }

    public void setAdjustType(String adjustType) {
        this.adjustType = adjustType;
    }

    public String getSerial() {
        return serial;
    }

    public void setSerial(String serial) {
        this.serial = serial;
    }

    public String getAdjustQty() {
        return adjustQty;
    }

    public void setAdjustQty(String adjustQty) {
        this.adjustQty = adjustQty;
    }

    public String getReason() {
        return reason;
    }

    public void setReason(String reason) {
        this.reason = reason;
    }

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

}


