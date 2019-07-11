package com.afei.jnidemo;

public class NativeLibrary {

    static {
        System.loadLibrary("native-lib");
    }

    public static native DataBean getDataFromNative();

    public static native void transferDataToNative(DataBean dataBean);
}
