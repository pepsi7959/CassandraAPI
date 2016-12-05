package com.ais.damocles.spark.schema;
import java.io.Serializable;

import com.datastax.driver.mapping.annotations.Column;

public class SummaryByUser  implements Serializable{
    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name="user")
	private String user;
	@Column(name="all_usages")
    private int usage;
    public SummaryByUser() { }

    public SummaryByUser(String user, int usage) {
        this.user = user;
        this.usage = usage;
    }

   	public void setUser(String user){ this.user = user;}
   	public String getUser(){return this.user;}

   	public void setUsage(int usage){ this.usage = usage;}
   	public int getUsage(){return this.usage;}
}