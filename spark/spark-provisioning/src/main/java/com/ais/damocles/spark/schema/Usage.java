package com.ais.damocles.spark.schema;
import java.io.Serializable;

public class Usage  implements Serializable{
    private String user;
    private String type;
    private int usage;
    public Usage() { }

    public Usage(String user, String type, int usage) {
        this.user = user;
        this.type = type;
        this.usage = usage;
    }

   	public void setUser(String user){ this.user = user;}
   	public String getUser(){return this.user;}

   	public void setType(String type){ this.type = type;}
   	public String getType(){return this.type;}

   	public void setUsage(int usage){ this.usage = usage;}
   	public int getUsage(){return this.usage;}
}