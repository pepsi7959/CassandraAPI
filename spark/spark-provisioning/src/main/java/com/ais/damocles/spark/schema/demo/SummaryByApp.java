package com.ais.damocles.spark.schema.demo;
import java.io.Serializable;

import com.datastax.driver.mapping.annotations.Column;

public class SummaryByApp  implements Serializable{
     /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	@Column(name="user")
	private String user;
	@Column(name="date")
	private String date;
	@Column(name="all_usages")
   	 private int usage;
    public SummaryByApp() { }

    public SummaryByApp(String user,String date, int usage) {
        this.user = user;
	this.date = date;
        this.usage = usage;
    }

   	public void setUser(String user){ this.user = user;}
   	public String getUser(){return this.user;}

	public void setDate(String date){ this.date = date;}
   	public String getDate(){return this.date;}

   	public void setUsage(int usage){ this.usage = usage;}
   	public int getUsage(){return this.usage;}
}
