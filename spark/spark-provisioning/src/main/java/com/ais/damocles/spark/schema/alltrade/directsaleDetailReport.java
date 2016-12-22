package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class directsaleDetailReport extends com.ais.damocles.spark.schema.CommonSchema {

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

    @Column(name = "pickupNo")
    private String pickupNo;

    @Column(name = "pickupDate")
    private String pickupDate;

    @Column(name = "salesCode")
    private String salesCode;

    @Column(name = "salesName")
    private String salesName;

    @Column(name = "returnNo")
    private String returnNo;

    @Column(name = "matCode")
    private String matCode;

    @Column(name = "returnDate")
    private String returnDate;

    @Column(name = "status")
    private String status;

    @Column(name = "fromSubStock")
    private String fromSubStock;

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

    @Column(name = "matType")
    private String matType;

    @Column(name = "serial")
    private String serial;

    @Column(name = "qtyPickUp")
    private String qtyPickUp;

    @Column(name = "qtyReturn")
    private String qtyReturn;

    @Column(name = "remark")
    private String remark;

    @Column(name = "createdBy")
    private String createdBy;

    @Column(name = "refDoc")
    private String refDoc;

    public directsaleDetailReport() {

    }

    public directsaleDetailReport( String reportName , String transactionType ,
                                   String createDateTimeFrom ,  String createDateTimeTo ,
                                   String fromLocationCodeList ,  String fromLocationCodeFrom ,
                                   String fromLocationCodeTo ,  String salesCode ,  String status ,
                                   String locationType ,  String locationSubType ,  String region,
                                   String company ,  String locationCode ,  String locationName ,
                                   String pickupNo ,  String pickupDate ,  String salesCode ,
                                   String salesName ,  String returnNo ,  String returnDate ,
                                   String status ,  String fromSubStock ,  String brand ,
                                   String Smodel ,  String color ,  String commercialName ,
                                   String matCode ,  String matDescription ,  String matType ,
                                   String serial ,  String qtyPickUp ,  String qtyReturn ,
                                   String remark ,  String createdBy ,  String refDoc){


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

        this.pickupNo =pickupNo;
        this.columnMapper.put("pickupNo", "pickupNo");

        this.pickupDate =pickupDate;
        this.columnMapper.put("pickupDate", "pickupDate");

        this.salesCode =salesCode;
        this.columnMapper.put("salesCode", "salesCode");

        this.salesName =salesName;
        this.columnMapper.put("salesName", "salesName");

        this.returnNo =returnNo;
        this.columnMapper.put("returnNo", "returnNo");

        this.returnDate =returnDate;
        this.columnMapper.put("returnDate", "returnDate");

        this.status =status;
        this.columnMapper.put("status", "status");

        this.fromSubStock =fromSubStock;
        this.columnMapper.put("fromSubStock", "fromSubStock");

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

        this.matType =matType;
        this.columnMapper.put("matType", "matType");

        this.serial =serial;
        this.columnMapper.put("serial", "serial");

        this.qtyPickUp =qtyPickUp;
        this.columnMapper.put("qtyPickUp", "qtyPickUp");

        this.qtyReturn =qtyReturn;
        this.columnMapper.put("qtyReturn", "qtyReturn");

        this.remark =remark;
        this.columnMapper.put("remark", "remark");

        this.createdBy =createdBy;
        this.columnMapper.put("createdBy", "createdBy");

        this.refDoc =refDoc;
        this.columnMapper.put("refDoc", "refDoc");

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

    public String getReturnNo() {
        return returnNo;
    }

    public void setReturnNo(String returnNo) {
        this.returnNo = returnNo;
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

    public String getSerial() {
        return serial;
    }

    public void setSerial(String serial) {
        this.serial = serial;
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

    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    public String getCreatedBy() {
        return createdBy;
    }

    public void setCreatedBy(String createdBy) {
        this.createdBy = createdBy;
    }

    public String getRefDoc() {
        return refDoc;
    }

    public void setRefDoc(String refDoc) {
        this.refDoc = refDoc;
    }

    public String getReturnDate() {
        return returnDate;
    }

    public void setReturnDate(String returnDate) {
        this.returnDate = returnDate;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getFromSubStock() {
        return fromSubStock;
    }

    public void setFromSubStock(String fromSubStock) {
        this.fromSubStock = fromSubStock;
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

    public String getPickupNo() {
        return pickupNo;
    }

    public void setPickupNo(String pickupNo) {
        this.pickupNo = pickupNo;
    }

    public String getPickupDate() {
        return pickupDate;
    }

    public void setPickupDate(String pickupDate) {
        this.pickupDate = pickupDate;
    }
}
