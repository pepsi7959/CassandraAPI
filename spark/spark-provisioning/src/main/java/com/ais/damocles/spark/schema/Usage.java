package com.ais.damocles.spark.schema;
import java.io.Serializable;

public class Usage  implements Serializable{
    private String user;
    private String type;
    private String usage;
    public Usage() { }

    public Usage(String user, String type, String usage) {
        this.user = user;
        this.type = type;
        this.usage = usage;
    }

   	public void setUser(String user){ this.user = user;}
   	public String getUser(){return this.user;}

   	public void setType(String type){ this.type = type;}
   	public String getType(){return this.type;}

   	public void setUsage(String usage){ this.usage = usage;}
   	public String getUsage(){return this.usage;}
}