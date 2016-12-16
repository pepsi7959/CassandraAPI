package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class SaleStock extends com.ais.damocles.spark.schema.CommonSchema {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "IdentityKey")
	private String IdentityKey;

	@Column(name = "IdentityValue")
	private String IdentityValue;

	@Column(name = "dn")
	private String dn;

	public SaleStock() {

	}

	public SaleStock(String IdentityKey, String IdentityValue, String dn) {

		this.IdentityKey = IdentityKey;
		this.columnMapper.put("IdentityKey", "IdentityKey");

		this.IdentityValue = IdentityValue;
		this.columnMapper.put("IdentityValue", "IdentityValue");

		this.dn = dn;
		this.columnMapper.put("dn", "dn");

	}

	public String getIdentityKey() {
		return IdentityKey;
	}

	public void setIdentityKey(String identityKey) {
		IdentityKey = identityKey;
	}

	public String getIdentityValue() {
		return IdentityValue;
	}

	public void setIdentityValue(String identityValue) {
		IdentityValue = identityValue;
	}

	public String getDn() {
		return dn;
	}

	public void setDn(String dn) {
		this.dn = dn;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
