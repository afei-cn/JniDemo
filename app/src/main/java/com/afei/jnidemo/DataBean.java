package com.afei.jnidemo;

import android.graphics.PointF;
import android.graphics.Rect;

import java.util.Arrays;

public class DataBean {

    public Rect mRect; // 其他类
    public PointF[] mPoints; // 其它类数组
    public Inner mInner; // 静态内部类

    public int mID; // 整型
    public float mScore; // 浮点型
    public byte[] mData; // 基本类型数组
    public int[][] mDoubleDimenArray; // 二维数组

    public static class Inner {
        public String mMessage; // 字符串
    }

    @Override
    public String toString() {
        return "DataBean{" +
                "mRect=" + (mRect == null ? mRect : mRect.toShortString()) +
                "\nmPoints=" + Arrays.toString(mPoints) +
                "\nmInner=" + mInner.mMessage +
                "\nmID=" + mID +
                "\nmScore=" + mScore +
                "\nmData=" + Arrays.toString(mData) +
                "\nmDoubleDimenArray=" + Arrays.toString(mDoubleDimenArray) +
                '}';
    }
}
