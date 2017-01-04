package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class DirectsaleSummaryReport extends com.ais.damocles.spark.schema.CommonSchema {

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

    @Column(name = "salesCode")
    private String salesCode;

    @Column(name = "status")
    private String status;

    @Column(name = "locationType")
    private String locationType;

    @Column(name = "locationSubType")
    private String locationSubType;

    @Column(name = "region")
    private String region;

    @Column(name = "company")
    private String company;

    @Column(name = "locationCode")
    private String locationCode;

    @Column(name = "locationName")
    private String locationName;

    @Column(name = "salesName")
    private String salesName;

    @Column(name = "productType")
    private String productType;

    @Column(name = "productSubType")
    private String productSubType;

    @Column(name = "brand")
    private String brand;

    @Column(name = "model")
    private String model;

    @Column(name = "color")
    private String color;

    @Column(name = "commercialName")
    private String commercialName;

    @Column(name = "matCode")
    private String matCode;

    @Column(name = "matDescription")
    private String matDescription;

    @Column(name = "qtyPickUp")
    private String qtyPickUp;

    @Column(name = "qtyReturn")
    private String qtyReturn;

    public DirectsaleSummaryReport() {

    }

    public DirectsaleSummaryReport( String reportName , String transactionType ,
                                    String createDateTimeFrom ,  String createDateTimeTo ,
                                    String fromLocationCodeList ,  String fromLocationCodeFrom ,
                                    String fromLocationCodeTo ,  String salesCode ,  String status ,
                                    String locationType ,  String locationSubType ,  String region,
                                    String company ,  String locationCode ,  String locationName ,
                                    String salesName ,  String productType ,
                                    String productSubType ,  String brand ,  String model ,
                                    String color ,  String commercialName ,  String matCode ,
                                    String matDescription ,  String qtyPickUp ,  String qtyReturn){


        this.reportName =reportName;
        this.columnMapper.put("reportName", "reportName");

        this.transactionType =transactionType;
        this.columnMapper.put("transactionType", "transactionType");

        this.createDateTimeTo =createDateTimeTo;
        this.columnMapper.put("createDateTimeTo", "createDateTimeTo");

        this.fromLocationCodeList =fromLocationCodeList;
        this.columnMapper.put("fromLocationCodeList", "fromLocationCodeList");

        this.fromLocationCodeFrom =fromLocationCodeFrom;
        this.columnMapper.put("fromLocationCodeFrom", "fromLocationCodeFrom");

        this.fromLocationCodeTo =fromLocationCodeTo;
        this.columnMapper.put("fromLocationCodeTo", "fromLocationCodeTo");

        this.salesCode =salesCode;
        this.columnMapper.put("salesCode", "salesCode");

        this.status =status;
        this.columnMapper.put("status", "status");

        this.locationType =locationType;
        this.columnMapper.put("locationType", "locationType");

        this.locationSubType =locationSubType;
        this.columnMapper.put("locationSubType", "locationSubType");

        this.region =region;
        this.columnMapper.put("region", "region");

        this.company =company;
        this.columnMapper.put("company", "company");

        this.locationCode =locationCode;
        this.columnMapper.put("locationCode", "locationCode");

        this.locationName =locationName;
        this.columnMapper.put("locationName", "locationName");

        this.salesCode =salesCode;
        this.columnMapper.put("salesCode", "salesCode");

        this.salesName =salesName;
        this.columnMapper.put("salesName", "salesName");

        this.productType =productType;
        this.columnMapper.put("productType", "productType");

        this.productSubType =productSubType;
        this.columnMapper.put("productSubType", "productSubType");

        this.brand =brand;
        this.columnMapper.put("brand", "brand");

        this.model =model;
        this.columnMapper.put("model", "model");

        this.color =color;
        this.columnMapper.put("color", "color");

        this.commercialName =commercialName;
        this.columnMapper.put("commercialName", "commercialName");

        this.matCode =matCode;
        this.columnMapper.put("matCode", "matCode");

        this.matDescription =matDescription;
        this.columnMapper.put("matDescription", "matDescription");

        this.qtyPickUp =qtyPickUp;
        this.columnMapper.put("qtyPickUp", "qtyPickUp");

        this.qtyReturn =qtyReturn;
        this.columnMapper.put("qtyReturn", "qtyReturn");


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

    public String getQtyPickUp() {
        return qtyPickUp;
    }

    public void setQtyPickUp(String qtyPickUp) {
        this.qtyPickUp = qtyPickUp;
    }

    public String getQtyReturn() {
        return qtyReturn;
    }

    public void setQtyReturn(String qtyReturn) {
        this.qtyReturn = qtyReturn;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
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

}
