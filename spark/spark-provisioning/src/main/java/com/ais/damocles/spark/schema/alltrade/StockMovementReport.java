package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class StockMovementReport extends com.ais.damocles.spark.schema.CommonSchema {

    private static final long serialVersionUID = 1L;

    @Column(name = "reportName")
    private String reportName;

    @Column(name = "transactionType")
    private String transactionType;

    @Column(name = "createDateTimeFrom")
    private String createDateTimeFrom;

    @Column(name = "createDateTimeTo")
    private String createDateTimeTo;

    @Column(name = "locationCodeList")
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

    @Column(name = "refDoc")
    private String refDoc;

    @Column(name = "matCodeList")
    private String matCodeList;

    @Column(name = "serialList")
    private String serialList;

    @Column(name = "locationNameAfter")
    private String locationNameAfter;

    @Column(name = "company")
    private String company;

    @Column(name = "subStockNameBefore")
    private String subStockNameBefore;

    @Column(name = "subStockNameAfter")
    private String subStockNameAfter;

    @Column(name = "matCode")
    private String matCode;

    @Column(name = "matDescription")
    private String matDescription;

    @Column(name = "commercialName")
    private String commercialName;

    @Column(name = "productType")
    private String productType;

    @Column(name = "productSubType")
    private String productSubType;

    @Column(name = "brand")
    private String brand;

    @Column(name = "model")
    private String model;

    @Column(name = "unit")
    private String unit;

    @Column(name = "serial")
    private String serial;

    @Column(name = "movementQuantity")
    private String movementQuantity;

    @Column(name = "type")
    private String type;

    @Column(name = "locationBefore")
    private String locationBefore;

    @Column(name = "locationAfter")
    private String locationAfter;

    @Column(name = "stockBeforeQuantity")
    private String stockBeforeQuantity;

    @Column(name = "stockAfterQuantity")
    private String stockAfterQuantity;

    @Column(name = "refDocType")
    private String refDocType;

    @Column(name = "movementType")
    private String movementType;

    @Column(name = "createDateTime")
    private String createDateTime;

    public StockMovementReport (){

    }

    public StockMovementReport (String reportName, String transactionType, String createDateTimeFrom,
                                String createDateTimeTo, String locationCodeList, String fromLocationCode,
                                String toLocationCode, String locationType, String locationSubType,
                                String region, String refDoc, String matCodeList, String serialList,
                                String locationNameAfter, String company, String subStockNameBefore,
                                String subStockNameAfter, String matCode, String matDescription,
                                String commercialName, String productType, String productSubType,
                                String brand, String model, String unit, String serial,
                                String movementQuantity, String type, String locationBefore,
                                String locationAfter, String stockBeforeQuantity, String stockAfterQuantity,
                                String refDocType, String movementType, String createDateTime){

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

     this.refDoc =refDoc;
     this.columnMapper.put("refDoc", "refDoc");

     this.matCodeList =matCodeList;
     this.columnMapper.put("matCodeList", "matCodeList");

     this.serialList =serialList;
     this.columnMapper.put("serialList", "serialList");

     this.locationNameAfter =locationNameAfter;
     this.columnMapper.put("locationNameAfter", "locationNameAfter");

     this.company =company;
     this.columnMapper.put("company", "company");

     this.subStockNameBefore =subStockNameBefore;
     this.columnMapper.put("subStockNameBefore", "subStockNameBefore");

     this.subStockNameAfter =subStockNameAfter;
     this.columnMapper.put("subStockNameAfter", "subStockNameAfter");

     this.matCode =matCode;
     this.columnMapper.put("matCode", "matCode");

     this.matDescription =matDescription;
     this.columnMapper.put("matDescription", "matDescription");

     this.commercialName =commercialName;
     this.columnMapper.put("commercialName", "commercialName");

     this.productType =productType;
     this.columnMapper.put("productType", "productType");

     this.productSubType =productSubType;
     this.columnMapper.put("productSubType", "productSubType");

     this.brand =brand;
     this.columnMapper.put("brand", "brand");

     this.model =model;
     this.columnMapper.put("model", "model");

     this.unit =unit;
     this.columnMapper.put("unit", "unit");

     this.serial =serial;
     this.columnMapper.put("serial", "serial");

     this.movementQuantity =movementQuantity;
     this.columnMapper.put("movementQuantity", "movementQuantity");

     this.type =type;
     this.columnMapper.put("type", "type");

     this.locationBefore =locationBefore;
     this.columnMapper.put("locationBefore", "locationBefore");

     this.locationAfter =locationAfter;
     this.columnMapper.put("locationAfter", "locationAfter");

     this.stockBeforeQuantity =stockBeforeQuantity;
     this.columnMapper.put("stockBeforeQuantity", "stockBeforeQuantity");

     this.stockAfterQuantity =stockAfterQuantity;
     this.columnMapper.put("stockAfterQuantity", "stockAfterQuantity");

     this.refDoc =refDoc;
     this.columnMapper.put("refDoc", "refDoc");

     this.refDocType =refDocType;
     this.columnMapper.put("refDocType", "refDocType");

     this.movementType =movementType;
     this.columnMapper.put("movementType", "movementType");

     this.createDateTime =createDateTime;
     this.columnMapper.put("createDateTime", "createDateTime");

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

    public String getLocationCodeList() {
        return locationCodeList;
    }

    public void setLocationCodeList(String locationCodeList) {
        this.locationCodeList = locationCodeList;
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

    public String getRefDoc() {
        return refDoc;
    }

    public void setRefDoc(String refDoc) {
        this.refDoc = refDoc;
    }

    public String getRefDocType() {
        return refDocType;
    }

    public void setRefDocType(String refDocType) {
        this.refDocType = refDocType;
    }

    public String getMovementType() {
        return movementType;
    }

    public void setMovementType(String movementType) {
        this.movementType = movementType;
    }

    public String getCreateDateTime() {
        return createDateTime;
    }

    public void setCreateDateTime(String createDateTime) {
        this.createDateTime = createDateTime;
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

    public String getLocationNameAfter() {
        return locationNameAfter;
    }

    public void setLocationNameAfter(String locationNameAfter) {
        this.locationNameAfter = locationNameAfter;
    }

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public String getSubStockNameBefore() {
        return subStockNameBefore;
    }

    public void setSubStockNameBefore(String subStockNameBefore) {
        this.subStockNameBefore = subStockNameBefore;
    }

    public String getSubStockNameAfter() {
        return subStockNameAfter;
    }

    public void setSubStockNameAfter(String subStockNameAfter) {
        this.subStockNameAfter = subStockNameAfter;
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

    public String getCommercialName() {
        return commercialName;
    }

    public void setCommercialName(String commercialName) {
        this.commercialName = commercialName;
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

    public String getUnit() {
        return unit;
    }

    public void setUnit(String unit) {
        this.unit = unit;
    }

    public String getSerial() {
        return serial;
    }

    public void setSerial(String serial) {
        this.serial = serial;
    }

    public String getMovementQuantity() {
        return movementQuantity;
    }

    public void setMovementQuantity(String movementQuantity) {
        this.movementQuantity = movementQuantity;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getLocationBefore() {
        return locationBefore;
    }

    public void setLocationBefore(String locationBefore) {
        this.locationBefore = locationBefore;
    }

    public String getLocationAfter() {
        return locationAfter;
    }

    public void setLocationAfter(String locationAfter) {
        this.locationAfter = locationAfter;
    }

    public String getStockBeforeQuantity() {
        return stockBeforeQuantity;
    }

    public void setStockBeforeQuantity(String stockBeforeQuantity) {
        this.stockBeforeQuantity = stockBeforeQuantity;
    }

    public String getStockAfterQuantity() {
        return stockAfterQuantity;
    }

    public void setStockAfterQuantity(String stockAfterQuantity) {
        this.stockAfterQuantity = stockAfterQuantity;
    }

}