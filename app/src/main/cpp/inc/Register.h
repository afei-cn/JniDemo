#ifndef JNIDEMO_REGISTER_H
#define JNIDEMO_REGISTER_H

#include <jni.h>
#include "DataBean.h"

// 对应 android.graphics.Rect 类
typedef struct rect_block_t {
    jclass clazz;
    jfieldID left;
    jfieldID top;
    jfieldID right;
    jfieldID bottom;
    jmethodID constructor;
} rect_block;

// 对应 android.graphics.PointF 类
typedef struct point_block_t {
    jclass clazz;
    jfieldID x;
    jfieldID y;
    jmethodID constructor;
} point_block;

// 对应 com.afei.jnidemo.DataBean$Inner 类
typedef struct inner_block_t {
    jclass clazz;
    jfieldID message;
    jmethodID constructor;
} inner_block;

// 对应 com.afei.jnidemo.DataBean 类
typedef struct data_bean_block_t {
    jclass clazz;
    jfieldID rect;
    jfieldID points;
    jfieldID inner;

    jfieldID id;
    jfieldID score;
    jfieldID data;
    jfieldID double_dimen_array;

    jmethodID constructor;
} data_bean_block;

// 注册
void register_classes(JNIEnv *env);

// C结构体转Java类
jobject data_bean_c_to_java(JNIEnv *env, jni_data_bean *data_bean);

// Java类转C结构体
void data_bean_java_to_c(JNIEnv *env, jobject data_bean_in, jni_data_bean *data_bean_out);

#endif //JNIDEMO_REGISTER_H
