package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

import java.util.List;

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

    @Column(name = "commercialName_key")
    private List<Object> commercialName_key;

    @Column(name = "matCode_key ")
    private List<Object> matCode_key;

    @Column(name = "matDescription")
    private String matDescription;

    @Column(name = "productType")
    private List<Object> productType;

    @Column(name = "productSubType")
    private List<Object> productSubType;

    @Column(name = "brand_key")
    private List<Object> brand_key;

    @Column(name = "model_key")
    private List<Object> model_key;

    @Column(name = "subStock")
    private String subStock;

    @Column(name = "serial ")
    private List<Object> serial;

    @Column(name = "adjustQty")
    private String adjustQty;

    @Column(name = "reason")
    private String reason;

    @Column(name = "remark")
    private String remark;

    @Column(name = "region")
    private String region;

    @Column(name = "adjustType")
    private String adjustType;

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
                             String adjustStockNo, String itemNo, List<Object> commercialName_key,
                             List<Object> matCode_key, String matDescription, List<Object> productType,
                             List<Object> productSubType, List<Object> brand_key, List<Object> model_key, String subStock,
                             String adjustType, List<Object> serial, String adjustQty, String reason, String remark
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

        this.commercialName_key = commercialName_key;
        this.columnMapper.put("commercialName_key", "commercialName_key");

        this.matCode_key = matCode_key;
        this.columnMapper.put("matCode_key ", "matCode_key ");

        this.matDescription = matDescription;
        this.columnMapper.put("matDescription", "matDescription");

        this.productType = productType;
        this.columnMapper.put("productType", "productType");

        this.productSubType = productSubType;
        this.columnMapper.put("productSubType", "productSubType");

        this.brand_key = brand_key;
        this.columnMapper.put("brand_key", "brand_key");

        this.model_key = model_key;
        this.columnMapper.put("model_key", "model_key");

        this.subStock = subStock;
        this.columnMapper.put("subStock", "subStock");

        this.adjustType = adjustType;
        this.columnMapper.put("adjustType", "adjustType");

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

    public List<Object> getCommercialName_key() {
        return commercialName_key;
    }

    public void setCommercialName_key(List<Object> commercialName_key) {
        this.commercialName_key = commercialName_key;
    }

    public List<Object> getMatCode_key() {
        return matCode_key;
    }

    public void setMatCode_key(List<Object> matCode_key) {
        this.matCode_key = matCode_key;
    }

    public String getMatDescription() {
        return matDescription;
    }

    public void setMatDescription(String matDescription) {
        this.matDescription = matDescription;
    }

    public List<Object> getProductType() {
        return productType;
    }

    public void setProductType(List<Object> productType) {
        this.productType = productType;
    }

    public List<Object> getProductSubType() {
        return productSubType;
    }

    public void setProductSubType(List<Object> productSubType) {
        this.productSubType = productSubType;
    }

    public List<Object> getBrand_key() {
        return brand_key;
    }

    public void setBrand_key(List<Object> brand_key) {
        this.brand_key = brand_key;
    }

    public List<Object> getModel_key() {
        return model_key;
    }

    public void setModel_key(List<Object> model_key) {
        this.model_key = model_key;
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

    public List<Object> getSerial() {
        return serial;
    }

    public void setSerial(List<Object> serial) {
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


