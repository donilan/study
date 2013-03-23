package com.ii2d;

import com.sun.jna.Library;
import com.sun.jna.Native;

public class Hello {
    public interface CTest extends Library {
     	public void hello();
    }

    public static void main(String argv[]) {
	CTest ctest = (CTest) Native.loadLibrary("ctest", CTest.class);
	ctest.hello();
    }
}