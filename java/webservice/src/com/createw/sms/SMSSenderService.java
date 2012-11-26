package com.createw.sms;


public class SMSSenderService {
	
	public void send(String phoneNo, String msg) {
		System.out.printf("phoneNo is %s, msg: %s\n", phoneNo, msg);
	}
}
