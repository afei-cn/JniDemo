package com.afei.jnidemo;

public class NativeLibrary {

    static {
        System.loadLibrary("native-lib");
    }

    public static native void test();

}
