package com.ais.damocles.spark.schema.alltrade;

import com.datastax.driver.mapping.annotations.Column;

public class ProductMaster extends com.ais.damocles.spark.schema.CommonSchema {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name = "key")
	private String key;
	@Column(name = "value")
	private String value;

	public ProductMaster() {

	}

	public ProductMaster(String key, String value) {

		this.key = key;
		this.columnMapper.put("key", "key");
		this.value = value;
		this.columnMapper.put("key", "key");

	}

	public String getKey() {
		return key;
	}

	public void setKey(String key) {
		this.key = key;
	}

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}

}
