package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class CheckStockReport extends com.ais.damocles.spark.schema.CommonSchema {

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

    @Column(name = "company")
    private String company;

    @Column(name = "subStock")
    private String subStock;

    @Column(name = "locationType")
    private String locationType;

    @Column(name = "locationSubType")
    private String locationSubType;

    @Column(name = "region")
    private String region;

    @Column(name = "locationCode")
    private String locationCode;

    @Column(name = "locationName")
    private String locationName;

    @Column(name = "confirmUser")
    private String confirmUser;

    @Column(name = "confirmDateTime")
    private String confirmDateTime;

    @Column(name = "confirmNo")
    private String confirmNo;

    @Column(name = "checkStockOnHand")
    private String checkStockOnHand;

    @Column(name = "company")
    private String company;

    @Column(name = "matCode")
    private String matCode;

    @Column(name = "matType")
    private String matType;

    @Column(name = "brand")
    private String brand;

    @Column(name = "model")
    private String model;

    @Column(name = "color")
    private String color;

    @Column(name = "commercialName")
    private String commercialName;

    @Column(name = "matDescription")
    private String matDescription;

    @Column(name = "productType")
    private String productType;

    @Column(name = "productSubType")
    private String productSubType;

    @Column(name = "totalScanQty")
    private String totalScanQty;

    @Column(name = "diffQty")
    private String diffQty;

    @Column(name = "matchQty")
    private String matchQty;

    @Column(name = "notFoundQty")
    private String notFoundQty;

    @Column(name = "notScanQty")
    private String notScanQty;

    @Column(name = "serial")
    private String serial;

    @Column(name = "substock")
    private String substock;

    public AdjustStockReport() {

    }

    public AdjustStockReport(String reportName, String transactionType, String createDateTimeFrom,
                             String createDateTimeTo, String fromLocationCodeList,
                             String fromLocationCodeFrom, String fromLocationCodeTo,
                             String company, String subStock, String locationType,
                             String locationSubType, String regionlocationCode ,
                             String locationName , String confirmUser ,
                             String confirmDateTime , String confirmNo ,
                             String checkStockOnHand , String company ,
                             String matCode , String matType , String brand ,
                             String model , String color , String commercialName ,
                             String matDescription , String productType ,
                             String productSubType , String totalScanQty ,
                             String diffQty , String matchQty , String notFoundQty ,
                             String notScanQty , String serial , String substock){


        this.reportName =reportName;
        this.columnMapper.put("reportName", "reportName");

        this.transactionType =transactionType;
        this.columnMapper.put("transactionType", "transactionType");

        this.createDateTimeFrom =createDateTimeFrom;
        this.columnMapper.put("createDateTimeFrom", "createDateTimeFrom");

        this.fromLocationCodeFrom =fromLocationCodeFrom;
        this.columnMapper.put("fromLocationCodeFrom", "fromLocationCodeFrom");

        this.fromLocationCodeTo =fromLocationCodeTo;
        this.columnMapper.put("fromLocationCodeTo", "fromLocationCodeTo");

        this.company =company;
        this.columnMapper.put("company", "company");

        this.subStock =subStock;
        this.columnMapper.put("subStock", "subStock");

        this.locationType =locationType;
        this.columnMapper.put("locationType", "locationType");

        this.locationSubType =locationSubType;
        this.columnMapper.put("locationSubType", "locationSubType");

        this.region =region;
        this.columnMapper.put("region", "region");

        this.locationCode =locationCode;
        this.columnMapper.put("locationCode", "locationCode");

        this.locationName =locationName;
        this.columnMapper.put("locationName", "locationName");

        this.confirmUser =confirmUser;
        this.columnMapper.put("confirmUser", "confirmUser");

        this.confirmDateTime =confirmDateTime;
        this.columnMapper.put("confirmDateTime", "confirmDateTime");

        this.confirmNo =confirmNo;
        this.columnMapper.put("confirmNo", "confirmNo");

        this.checkStockOnHand =checkStockOnHand;
        this.columnMapper.put("checkStockOnHand", "checkStockOnHand");

        this.company =company;
        this.columnMapper.put("company", "company");

        this.matCode =matCode;
        this.columnMapper.put("matCode", "matCode");

        this.matType =matType;
        this.columnMapper.put("matType", "matType");

        this.brand =brand;
        this.columnMapper.put("brand", "brand");

        this.model =model;
        this.columnMapper.put("model", "model");

        this.color =color;
        this.columnMapper.put("color", "color");

        this.commercialName =commercialName;
        this.columnMapper.put("commercialName", "commercialName");

        this.matDescription =matDescription;
        this.columnMapper.put("matDescription", "matDescription");

        this.productType =productType;
        this.columnMapper.put("productType", "productType");

        this.productSubType =productSubType;
        this.columnMapper.put("productSubType", "productSubType");

        this.totalScanQty =totalScanQty;
        this.columnMapper.put("totalScanQty", "totalScanQty");

        this.diffQty =diffQty;
        this.columnMapper.put("diffQty", "diffQty");

        this.matchQty =matchQty;
        this.columnMapper.put("matchQty", "matchQty");

        this.notFoundQty =notFoundQty;
        this.columnMapper.put("notFoundQty", "notFoundQty");

        this.notScanQty =notScanQty;
        this.columnMapper.put("notScanQty", "notScanQty");

        this.serial =serial;
        this.columnMapper.put("serial", "serial");

        this.substock =substock;
        this.columnMapper.put("substock", "substock");

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

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public String getMatCode() {
        return matCode;
    }

    public void setMatCode(String matCode) {
        this.matCode = matCode;
    }

    public String getMatType() {
        return matType;
    }

    public void setMatType(String matType) {
        this.matType = matType;
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

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public String getCommercialName() {
        return commercialName;
    }

    public void setCommercialName(String commercialName) {
        this.commercialName = commercialName;
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

    public String getTotalScanQty() {
        return totalScanQty;
    }

    public void setTotalScanQty(String totalScanQty) {
        this.totalScanQty = totalScanQty;
    }

    public String getDiffQty() {
        return diffQty;
    }

    public void setDiffQty(String diffQty) {
        this.diffQty = diffQty;
    }

    public String getMatchQty() {
        return matchQty;
    }

    public void setMatchQty(String matchQty) {
        this.matchQty = matchQty;
    }

    public String getNotFoundQty() {
        return notFoundQty;
    }

    public void setNotFoundQty(String notFoundQty) {
        this.notFoundQty = notFoundQty;
    }

    public String getNotScanQty() {
        return notScanQty;
    }

    public void setNotScanQty(String notScanQty) {
        this.notScanQty = notScanQty;
    }

    public String getSerial() {
        return serial;
    }

    public void setSerial(String serial) {
        this.serial = serial;
    }

    public String getSubstock() {
        return substock;
    }

    public void setSubstock(String substock) {
        this.substock = substock;
    }

    public String getSubStock() {
        return subStock;
    }

    public void setSubStock(String subStock) {
        this.subStock = subStock;
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

    public String getConfirmUser() {
        return confirmUser;
    }

    public void setConfirmUser(String confirmUser) {
        this.confirmUser = confirmUser;
    }

    public String getConfirmDateTime() {
        return confirmDateTime;
    }

    public void setConfirmDateTime(String confirmDateTime) {
        this.confirmDateTime = confirmDateTime;
    }

    public String getConfirmNo() {
        return confirmNo;
    }

    public void setConfirmNo(String confirmNo) {
        this.confirmNo = confirmNo;
    }

    public String getCheckStockOnHand() {
        return checkStockOnHand;
    }

    public void setCheckStockOnHand(String checkStockOnHand) {
        this.checkStockOnHand = checkStockOnHand;
    }

}
