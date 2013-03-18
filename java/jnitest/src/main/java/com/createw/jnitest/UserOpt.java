package com.createw.jnitest;

import java.io.IOException;

public class UserOpt {
	public native User getUser(String username);
	public native void saveUser(User u);
	
	static {
		try {
			NativeUtils.loadLibraryFromJar("/libUserOpt.so");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
