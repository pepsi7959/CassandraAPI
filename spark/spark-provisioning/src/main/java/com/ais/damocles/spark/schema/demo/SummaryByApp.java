package com.ais.damocles.spark.schema.demo;
import java.io.Serializable;
import java.text.Format;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;



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


    public String format_date(String date)
    {
     try {
    SimpleDateFormat in_formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ssZ");
    Date date2 = new Date();
    date2 = in_formatter.parse(date);

    Format out_formatter = (Format)new SimpleDateFormat("yyyy-MM-dd");
    String date3 = out_formatter.format(date2);
    //System.out.println(s);

    return date3;
     } catch (Exception e) {

       e.printStackTrace();
      //System.out.println(e.printStackTrace();
      return null;
    }

   }
}


