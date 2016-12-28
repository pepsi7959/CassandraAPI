package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class OrderTransfer extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 *
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "transactionId")
	private String transactionId;
	@Column(name = "transactionType")
	private String transactionType;
	@Column(name = "transferNo")
	private String transferNo;
	@Column(name = "company")
	private String company;
	@Column(name = "fromSubStock")
	private String fromSubStock;
	@Column(name = "toSubStock")
	private String toSubStock;
	@Column(name = "transferStatus")
	private String transferStatus;
	@Column(name = "fromLocationCode")
	private String fromLocationCode;
	@Column(name = "fromLocationName")
	private String fromLocationName;
	@Column(name = "fromLocationType")
	private String fromLocationType;
	@Column(name = "fromLocationSubType")
	private String fromLocationSubType;
	@Column(name = "fromLocationRegion")
	private String fromLocationRegion;
	@Column(name = "fromLocationProvince")
	private String fromLocationProvince;
	@Column(name = "toLocationCode")
	private String toLocationCode;
	@Column(name = "toLocationName")
	private String toLocationName;
	@Column(name = "toLocationType")
	private String toLocationType;
	@Column(name = "toLocationSubType")
	private String toLocationSubType;
	@Column(name = "toLocationRegion")
	private String toLocationRegion;
	@Column(name = "toLocationProvince")
	private String toLocationProvince;
	@Column(name = "docRef")
	private String docRef;
	@Column(name = "transferDateTime")
	private String transferDateTime;
	@Column(name = "mmDocYear")
	private String mmDocYear;
	@Column(name = "mmDocNo")
	private String mmDocNo;
	@Column(name = "doNo")
	private String doNo;
	@Column(name = "movementType")
	private String movementType;
	@Column(name = "distributionChannel")
	private String distributionChannel;
	@Column(name = "createBy ")
	private String createBy;
	@Column(name = "createDateTime")
	private String createDateTime;
	@Column(name = "updateBy")
	private String updateBy;
	@Column(name = "updateDateTime ")
	private String updateDateTime;
	@Column(name = "cancelBy")
	private String cancelBy;
	@Column(name = "cancelDateTime")
	private String cancelDateTime;
	@Column(name = "shipToCode")
	private String shipToCode;
	@Column(name = "shipToName")
	private String shipToName;
	@Column(name = "shipToProvince")
	private String shipToProvince;
	@Column(name = "shipToAddress")
	private String shipToAddress;
	@Column(name = "vendorCode")
	private String vendorCode;
	@Column(name = "vendorName")
	private String vendorName;
	@Column(name = "remark")
	private String remark;
	@Column(name = "matCode_key")
	private String matCode_key;
	@Column(name = "brand_key")
	private String brand_key;
	@Column(name = "model_key")
	private String model_key;
	@Column(name = "productType_key")
	private String productType_key;
	@Column(name = "productSubType_key")
	private String productSubType_key;
	@Column(name = "transferDetail")
	private String transferDetail;
	@Column(name = "transferOutStatus")
	private String transferOutStatus;
    @Column(name = "item")
    private String item;
    @Column(name = "commercialName")
    private String commercialName;
    @Column(name = "matDescription")
    private String matDescription;
    @Column(name = "matType")
    private String matType;
    @Column(name = "serialNo")
    private String serialNo;
    @Column(name = "qtyOut")
    private String qtyOut;
    @Column(name = "qtyIn")
    private String qtyIn;
    @Column(name = "reason")
    private String reason;
    @Column(name = "receiveBy")
    private String receiveBy;

    public OrderTransfer() {

	}

	public OrderTransfer(String transactionId, String transactionType,
			             String transferNo, String company, String fromSubStock,
			             String toSubStock, String transferStatus, String fromLocationCode,
                         String fromLocationName, String fromLocationType,
			             String fromLocationSubType, String fromLocationRegion,
			             String fromLocationProvince, String toLocationCode,
			             String toLocationName, String toLocationType,
			             String toLocationSubType, String toLocationRegion,
			             String toLocationProvince, String docRef, String transferDateTime,
			             String mmDocYear, String mmDocNo, String doNo, String movementType,
			             String distributionChannel, String createBy, String createDateTime,
			             String updateBy, String updateDateTime, String cancelBy,
			             String cancelDateTime, String shipToCode, String shipToName,
			             String shipToProvince, String shipToAddress, String vendorCode,
			             String vendorName, String remark, String matCode_key,
			             String brand_key, String model_key, String productType_key,
			             String productSubType_key, String transferDetail,
						 String transferOutStatus, String item, String commercialName,
                         String matDescription, String matType, String serialNo,
                         String qtyOut, String qtyIn, String reason, String receiveBy) {

		this.transactionId = transactionId;
		this.columnMapper.put("transactionId", "transactionId");

		this.transactionType = transactionType;
		this.columnMapper.put("transactionType", "transactionType");

		this.transferNo = transferNo;
		this.columnMapper.put("transferNo", "transferNo");

		this.company = company;
		this.columnMapper.put("company", "company");

		this.fromSubStock = fromSubStock;
		this.columnMapper.put("fromSubStock", "fromSubStock");

		this.toSubStock = toSubStock;
		this.columnMapper.put("toSubStock", "toSubStock");

		this.transferStatus = transferStatus;
		this.columnMapper.put("transferStatus", "transferStatus");

		this.fromLocationCode = fromLocationCode;
		this.columnMapper.put("fromLocationCode", "fromLocationCode");

		this.fromLocationName = fromLocationName;
		this.columnMapper.put("fromLocationName", "fromLocationName");

		this.fromLocationType = fromLocationType;
		this.columnMapper.put("fromLocationType", "fromLocationType");

		this.fromLocationSubType = fromLocationSubType;
		this.columnMapper.put("fromLocationSubType", "fromLocationSubType");

		this.fromLocationRegion = fromLocationRegion;
		this.columnMapper.put("fromLocationRegion", "fromLocationRegion");

		this.fromLocationProvince = fromLocationProvince;
		this.columnMapper.put("fromLocationProvince", "fromLocationProvince");

		this.toLocationCode = toLocationCode;
		this.columnMapper.put("toLocationCode", "toLocationCode");

		this.toLocationName = toLocationName;
		this.columnMapper.put("toLocationName", "toLocationName");

		this.toLocationType = toLocationType;
		this.columnMapper.put("toLocationType", "toLocationType");

		this.toLocationSubType = toLocationSubType;
		this.columnMapper.put("toLocationSubType", "toLocationSubType");

		this.toLocationRegion = toLocationRegion;
		this.columnMapper.put("toLocationRegion", "toLocationRegion");

		this.toLocationProvince = toLocationProvince;
		this.columnMapper.put("toLocationProvince", "toLocationProvince");

		this.docRef = docRef;
		this.columnMapper.put("docRef", "docRef");

		this.transferDateTime = transferDateTime;
		this.columnMapper.put("transferDateTime", "transferDateTime");

		this.mmDocYear = mmDocYear;
		this.columnMapper.put("mmDocYear", "mmDocYear");

		this.mmDocNo = mmDocNo;
		this.columnMapper.put("mmDocNo", "mmDocNo");

		this.doNo = doNo;
		this.columnMapper.put("doNo", "doNo");

		this.movementType = movementType;
		this.columnMapper.put("movementType", "movementType");

		this.distributionChannel = distributionChannel;
		this.columnMapper.put("distributionChannel", "distributionChannel");

		this.createBy = createBy;
		this.columnMapper.put("createBy ", "createBy ");

		this.createDateTime = createDateTime;
		this.columnMapper.put("createDateTime", "createDateTime");

		this.updateBy = updateBy;
		this.columnMapper.put("updateBy ", "updateBy ");

		this.updateDateTime = updateDateTime;
		this.columnMapper.put("updateDateTime ", "updateDateTime ");

		this.cancelBy = cancelBy;
		this.columnMapper.put("cancelBy", "cancelBy");

		this.cancelDateTime = cancelDateTime;
		this.columnMapper.put("cancelDateTime", "cancelDateTime");

		this.shipToCode = shipToCode;
		this.columnMapper.put("shipToCode", "shipToCode");

		this.shipToName = shipToName;
		this.columnMapper.put("shipToName", "shipToName");

		this.shipToProvince = shipToProvince;
		this.columnMapper.put("shipToProvince", "shipToProvince");

		this.shipToAddress = shipToAddress;
		this.columnMapper.put("shipToAddress", "shipToAddress");

		this.vendorCode = vendorCode;
		this.columnMapper.put("vendorCode", "vendorCode");

		this.vendorName = vendorName;
		this.columnMapper.put("vendorName", "vendorName");

		this.remark = remark;
		this.columnMapper.put("remark", "remark");

		this.matCode_key = matCode_key;
		this.columnMapper.put("matCode_key", "matCode_key");

		this.brand_key = brand_key;
		this.columnMapper.put("brand_key", "brand_key");

		this.model_key = model_key;
		this.columnMapper.put("model_key", "model_key");

		this.productType_key = productType_key;
		this.columnMapper.put("productType_key", "productType_key");

		this.productSubType_key = productSubType_key;
		this.columnMapper.put("productSubType_key", "productSubType_key");

		this.transferDetail = transferDetail;
		this.columnMapper.put("transferDetail ", "transferDetail ");

        this.transferOutStatus = transferOutStatus;
        this.columnMapper.put("transferOutStatus ", "transferOutStatus ");

        this.item = item;
        this.columnMapper.put("item ", "item ");

        this.commercialName = commercialName;
        this.columnMapper.put("commercialName ", "commercialName ");

        this.matDescription = matDescription;
        this.columnMapper.put("matDescription ", "matDescription ");

        this.matType = matType;
        this.columnMapper.put("matType ", "matType ");

        this.serialNo = serialNo;
        this.columnMapper.put("serialNo ", "serialNo ");

        this.qtyOut = qtyOut;
        this.columnMapper.put("qtyOut ", "qtyOut ");

        this.qtyIn = qtyIn;
        this.columnMapper.put("qtyIn ", "qtyIn ");

        this.reason = reason;
        this.columnMapper.put("reason ", "reason ");

        this.receiveBy = receiveBy;
        this.columnMapper.put("receiveBy ", "receiveBy ");

	}

	public String getTransactionId() {
		return transactionId;
	}

	public void setTransactionId(String transactionId) {
		this.transactionId = transactionId;
	}

	public String getTransactionType() {
		return transactionType;
	}

	public void setTransactionType(String transactionType) {
		this.transactionType = transactionType;
	}

	public String getTransferNo() {
		return transferNo;
	}

	public void setTransferNo(String transferNo) {
		this.transferNo = transferNo;
	}

	public String getCompany() {
		return company;
	}

	public void setCompany(String company) {
		this.company = company;
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

	public String getTransferStatus() {
		return transferStatus;
	}

	public void setTransferStatus(String transferStatus) {
		this.transferStatus = transferStatus;
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

	public String getFromLocationType() {
		return fromLocationType;
	}

	public void setFromLocationType(String fromLocationType) {
		this.fromLocationType = fromLocationType;
	}

	public String getFromLocationSubType() {
		return fromLocationSubType;
	}

	public void setFromLocationSubType(String fromLocationSubType) {
		this.fromLocationSubType = fromLocationSubType;
	}

	public String getFromLocationRegion() {
		return fromLocationRegion;
	}

	public void setFromLocationRegion(String fromLocationRegion) {
		this.fromLocationRegion = fromLocationRegion;
	}

	public String getFromLocationProvince() {
		return fromLocationProvince;
	}

	public void setFromLocationProvince(String fromLocationProvince) {
		this.fromLocationProvince = fromLocationProvince;
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

	public String getToLocationType() {
		return toLocationType;
	}

	public void setToLocationType(String toLocationType) {
		this.toLocationType = toLocationType;
	}

	public String getToLocationSubType() {
		return toLocationSubType;
	}

	public void setToLocationSubType(String toLocationSubType) {
		this.toLocationSubType = toLocationSubType;
	}

	public String getToLocationRegion() {
		return toLocationRegion;
	}

	public void setToLocationRegion(String toLocationRegion) {
		this.toLocationRegion = toLocationRegion;
	}

	public String getToLocationProvince() {
		return toLocationProvince;
	}

	public void setToLocationProvince(String toLocationProvince) {
		this.toLocationProvince = toLocationProvince;
	}

	public String getDocRef() {
		return docRef;
	}

	public void setDocRef(String docRef) {
		this.docRef = docRef;
	}

	public String getTransferDateTime() {
		return transferDateTime;
	}

	public void setTransferDateTime(String transferDateTime) {
		this.transferDateTime = transferDateTime;
	}

	public String getMmDocYear() {
		return mmDocYear;
	}

	public void setMmDocYear(String mmDocYear) {
		this.mmDocYear = mmDocYear;
	}

	public String getMmDocNo() {
		return mmDocNo;
	}

	public void setMmDocNo(String mmDocNo) {
		this.mmDocNo = mmDocNo;
	}

	public String getDoNo() {
		return doNo;
	}

	public void setDoNo(String doNo) {
		this.doNo = doNo;
	}

	public String getMovementType() {
		return movementType;
	}

	public void setMovementType(String movementType) {
		this.movementType = movementType;
	}

	public String getDistributionChannel() {
		return distributionChannel;
	}

	public void setDistributionChannel(String distributionChannel) {
		this.distributionChannel = distributionChannel;
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

	public String getUpdateBy() {
		return updateBy;
	}

	public void setUpdateBy(String updateBy) {
		this.updateBy = updateBy;
	}

	public String getUpdateDateTime() {
		return updateDateTime;
	}

	public void setUpdateDateTime(String updateDateTime) {
		this.updateDateTime = updateDateTime;
	}

	public String getCancelBy() {
		return cancelBy;
	}

	public void setCancelBy(String cancelBy) {
		this.cancelBy = cancelBy;
	}

	public String getCancelDateTime() {
		return cancelDateTime;
	}

	public void setCancelDateTime(String cancelDateTime) {
		this.cancelDateTime = cancelDateTime;
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

	public String getShipToProvince() {
		return shipToProvince;
	}

	public void setShipToProvince(String shipToProvince) {
		this.shipToProvince = shipToProvince;
	}

	public String getShipToAddress() {
		return shipToAddress;
	}

	public void setShipToAddress(String shipToAddress) {
		this.shipToAddress = shipToAddress;
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

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getMatCode_key() {
		return matCode_key;
	}

	public void setMatCode_key(String matCode_key) {
		this.matCode_key = matCode_key;
	}

	public String getBrand_key() {
		return brand_key;
	}

	public void setBrand_key(String brand_key) {
		this.brand_key = brand_key;
	}

	public String getModel_key() {
		return model_key;
	}

	public void setModel_key(String model_key) {
		this.model_key = model_key;
	}

	public String getProductType_key() {
		return productType_key;
	}

	public void setProductType_key(String productType_key) {
		this.productType_key = productType_key;
	}

	public String getProductSubType_key() {
		return productSubType_key;
	}

	public void setProductSubType_key(String productSubType_key) {
		this.productSubType_key = productSubType_key;
	}

	public String getTransferDetail() {
		return transferDetail;
	}

	public void setTransferDetail(String transferDetail) {
		this.transferDetail = transferDetail;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

    public String getTransferOutStatus() {
        return transferOutStatus;
    }

    public void setTransferOutStatus(String transferOutStatus) {
        this.transferOutStatus = transferOutStatus;
    }

    public String getItem() {
        return item;
    }

    public void setItem(String item) {
        this.item = item;
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

    public String getMatType() {
        return matType;
    }

    public void setMatType(String matType) {
        this.matType = matType;
    }

    public String getSerialNo() {
        return serialNo;
    }

    public void setSerialNo(String serialNo) {
        this.serialNo = serialNo;
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

    public String getReason() {
        return reason;
    }

    public void setReason(String reason) {
        this.reason = reason;
    }

    public String getReceiveBy() {
        return receiveBy;
    }

    public void setReceiveBy(String receiveBy) {
        this.receiveBy = receiveBy;
    }
}
