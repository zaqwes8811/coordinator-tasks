package com.hmkcode.vo;

import java.io.Serializable;

public class Person implements Serializable {
 
  private static final long serialVersionUID = 1L;
  private int id;
  private String name;
 
//getters and setters...
  public void setId(int v) {
	  id = v;
  }
  public int getId() {
	  return id;
  }
  public void setName(String v) {
	  name = v;
  }
  public String getName() {
	  return name;
  }
 
}