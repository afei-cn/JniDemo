# JniDemo

## 一、简介
JNI 开发中，常常会存在对应的 Java 类和 C 结构体需要互相转换。通过本实例学习和了解这个过程。

**预备知识：**
1. JNI数据类型和类型描述符介绍：[https://blog.csdn.net/afei__/article/details/80899758](https://blog.csdn.net/afei__/article/details/80899758)
2. JNI 方法及使用示例：[https://blog.csdn.net/afei__/article/details/81016413](https://blog.csdn.net/afei__/article/details/81016413)

## 二、目标
我们的目标就是通过 JNI 来完成下面两个类/结构体的互相转化。
### 1. Java 类
以下 `DataBean ` 类基本包含了可能用得着的大部分属性（类、类数组、内部类、基本数据类型、基本数据类型数组、二维数组）了。
```java
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
}
```

### 2. C 结构体
对应上面的 Java 类。
```cpp
typedef struct jni_rect_t {
    int left;
    int top;
    int right;
    int bottom;
} jni_rect;

typedef struct jni_point_t {
    float x;
    float y;
} jni_point;

typedef struct jni_data_bean_t {
    jni_rect rect; // Rect
    jni_point points[4]; // PointF[]
    const char *message; // String
    int id; // int
    float score; // float
    signed char data[4]; // byte[]
    int double_dimen_array[2][2]; // int[][]
} jni_data_bean;
```

## 三、开发

> 备注：本例开发环境为 Android Studio，NDK 编译方式为 Cmake。

### 1. NativeLibrary
首先新建一个类，来负责调用 native 方法。
```java
public class NativeLibrary {
    static {
        System.loadLibrary("native-lib");
    }
    // 将C结构体转为Java类
    public static native DataBean getDataFromNative();
    // 将Java类转为C结构体
    public static native void transferDataToNative(DataBean dataBean);
}
```

### 2. com_afei_jnidemo_NativeLibrary.cpp
对应 NativeLibrary 类中的 native 方法，这里使用的是 **“静态注册”** 的方式。

下面代码中，比较重要的地方，一个是在 `JNI_OnLoad` 方法中，我们调用了 `register_classes` 方法去注册类，这是因为我们要在 JNI 中使用 Java 的类、成员、方法，必须将他们先关联起来。

其次就是转换的两个方法的实现，也都放在了 `Register.cpp` 里了。
```cpp
#include "com_afei_jnidemo_NativeLibrary.h"
#include "DataBean.h"
#include "Register.h"
#include "LogUtil.h"

void print(jni_data_bean *data_bean);

/**
 * JNI 加载动态库的时候就会自动调用 JNI_OnLoad 方法
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = JNI_ERR;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    register_classes(env); // 注册所有的类
    return JNI_VERSION_1_6;
}

JNIEXPORT jobject JNICALL
Java_com_afei_jnidemo_NativeLibrary_getDataFromNative(JNIEnv *env, jclass type) {
    jni_data_bean data_bean = {
            .rect = {0, 0, 640, 480},
            .points = {
                    {0.0, 1.0},
                    {1.0, 2.0},
                    {2.0, 3.0},
                    {3.0, 4.0}
            },
            .message = "data from native",
            .id = 0,
            .score = 1.0,
            .data = {0, 1, 2, 3},
            .double_dimen_array = {
                    {0, 1},
                    {2, 3}
            }
    };
    print(&data_bean);
    jobject obj = data_bean_c_to_java(env, &data_bean);
    return obj;
}

JNIEXPORT void JNICALL
Java_com_afei_jnidemo_NativeLibrary_transferDataToNative(JNIEnv *env, jclass type, jobject dataBean) {
    jni_data_bean data_bean;
    data_bean_java_to_c(env, dataBean, &data_bean);
    print(&data_bean);
}
```

### 3. Register.h
这里我们创建了一些新的结构体，方便存储各个类的相关信息。
```cpp
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
```

### 4. Register.cpp
这里内容比较多，分几步来说：

#### a. 注册类信息
```cpp
rect_block m_rect_block;
point_block m_point_block;
inner_block m_inner_block;
data_bean_block m_data_bean_block;

int find_class(JNIEnv *env, const char *name, jclass *clazz_out) {
    jclass clazz = env->FindClass(name);
    if (clazz == nullptr) {
        LOGE("Can't find %s", name);
        return -1;
    }
    *clazz_out = (jclass) env->NewGlobalRef(clazz); // 这里必须新建一个全局的引用
    return 0;
}

int get_field(JNIEnv *env, jclass *clazz, const char *name, const char *sig, jfieldID *field_out) {
    jfieldID filed = env->GetFieldID(*clazz, name, sig);
    if (filed == nullptr) {
        LOGE("Can't find. filed name: %s, sig: %s", name, sig);
        return -1;
    }
    *field_out = filed;
    return 0;
}

void register_rect_class(JNIEnv *env) {
    int ret = find_class(env, "android/graphics/Rect", &m_rect_block.clazz);
    if (ret != 0) {
        LOGE("register_rect_class failed");
        return;
    }
    jclass clazz = m_rect_block.clazz;
    // 构造方法
    m_rect_block.constructor = env->GetMethodID(clazz, "<init>", "()V");
    // 成员
    get_field(env, &clazz, "left", "I", &m_rect_block.left);
    get_field(env, &clazz, "top", "I", &m_rect_block.top);
    get_field(env, &clazz, "right", "I", &m_rect_block.right);
    get_field(env, &clazz, "bottom", "I", &m_rect_block.bottom);
}

void register_point_class(JNIEnv *env) {
    int ret = find_class(env, "android/graphics/PointF", &m_point_block.clazz);
    if (ret != 0) {
        LOGE("register_point_class failed");
        return;
    }
    jclass clazz = m_point_block.clazz;
    // 构造方法
    m_point_block.constructor = env->GetMethodID(clazz, "<init>", "()V");
    // 成员
    get_field(env, &clazz, "x", "F", &m_point_block.x);
    get_field(env, &clazz, "y", "F", &m_point_block.y);
}

void register_inner_class(JNIEnv *env) {
    int ret = find_class(env, "com/afei/jnidemo/DataBean$Inner", &m_inner_block.clazz);
    if (ret != 0) {
        LOGE("register_inner_class failed");
        return;
    }
    jclass clazz = m_inner_block.clazz;
    // 构造方法
    m_inner_block.constructor = env->GetMethodID(clazz, "<init>", "()V");
    // 成员
    get_field(env, &clazz, "mMessage", "Ljava/lang/String;", &m_inner_block.message);
}

void register_data_bean_class(JNIEnv *env) {
    int ret = find_class(env, "com/afei/jnidemo/DataBean", &m_data_bean_block.clazz);
    if (ret != 0) {
        LOGE("register_data_bean_class failed");
        return;
    }
    jclass clazz = m_data_bean_block.clazz;
    // 构造方法
    m_data_bean_block.constructor = env->GetMethodID(clazz, "<init>", "()V");
    // 成员
    get_field(env, &clazz, "mRect", "Landroid/graphics/Rect;", &m_data_bean_block.rect);
    get_field(env, &clazz, "mPoints", "[Landroid/graphics/PointF;", &m_data_bean_block.points);
    get_field(env, &clazz, "mInner", "Lcom/afei/jnidemo/DataBean$Inner;", &m_data_bean_block.inner);
    get_field(env, &clazz, "mID", "I", &m_data_bean_block.id);
    get_field(env, &clazz, "mScore", "F", &m_data_bean_block.score);
    get_field(env, &clazz, "mData", "[B", &m_data_bean_block.data);
    get_field(env, &clazz, "mDoubleDimenArray", "[[I", &m_data_bean_block.double_dimen_array);
}

void register_classes(JNIEnv *env) {
    register_rect_class(env);
    register_point_class(env);
    register_inner_class(env);
    register_data_bean_class(env);
}
```
实现头文件中的 `register_classes` 方法，完成四个相关 Java 类的注册逻辑。关于具体的 JNI 类型描述符和方法介绍，还请参考最上面 **预备知识** 提供的两个链接。

#### b. C转Java
```cpp
jobject data_bean_c_to_java(JNIEnv *env, jni_data_bean *c_data_bean) {
    if (c_data_bean == nullptr) {
        LOGW("input data is null!");
        return nullptr;
    }
    LOGD("start data_bean_c_to_java");

    // 1. create rect
    jobject rect = env->NewObject(m_rect_block.clazz, m_rect_block.constructor);
    env->SetIntField(rect, m_rect_block.left, c_data_bean->rect.left);
    env->SetIntField(rect, m_rect_block.top, c_data_bean->rect.top);
    env->SetIntField(rect, m_rect_block.right, c_data_bean->rect.right);
    env->SetIntField(rect, m_rect_block.bottom, c_data_bean->rect.bottom);

    // 2. point array
    jsize len = NELEM(c_data_bean->points);
    LOGD("point array len: %d", len);
    jobjectArray point_array = env->NewObjectArray(len, m_point_block.clazz, NULL);
    for (int i = 0; i < len; i++) {
        jobject point = env->NewObject(m_point_block.clazz, m_point_block.constructor);
        env->SetFloatField(point, m_point_block.x, c_data_bean->points[i].x);
        env->SetFloatField(point, m_point_block.y, c_data_bean->points[i].y);
        env->SetObjectArrayElement(point_array, i, point);
    }

    // 3. inner class
    jobject inner = env->NewObject(m_inner_block.clazz, m_inner_block.constructor);
    jstring message = env->NewStringUTF(c_data_bean->message);
    env->SetObjectField(inner, m_inner_block.message, message);

    // 4. DataBean class
    jobject java_data_bean = env->NewObject(m_data_bean_block.clazz, m_data_bean_block.constructor);
    env->SetObjectField(java_data_bean, m_data_bean_block.rect, rect);
    env->SetObjectField(java_data_bean, m_data_bean_block.points, point_array);
    env->SetObjectField(java_data_bean, m_data_bean_block.inner, inner);
    env->SetIntField(java_data_bean, m_data_bean_block.id, c_data_bean->id);
    env->SetFloatField(java_data_bean, m_data_bean_block.score, c_data_bean->score);
    // byte array
    len = NELEM(c_data_bean->data);
    LOGD("data array len: %d", len);
    jbyteArray data = env->NewByteArray(len);
    env->SetByteArrayRegion(data, 0, len, c_data_bean->data);
    env->SetObjectField(java_data_bean, m_data_bean_block.data, data);
    // double dimen int array
    len = NELEM(c_data_bean->double_dimen_array);
    LOGD("double dimen int array len: %d", len);
    jclass clazz = env->FindClass("[I"); // 一维数组的类
    jobjectArray double_dimen_array = env->NewObjectArray(len, clazz, NULL);
    for (int i = 0; i < len; i++) {
        jsize sub_len = NELEM(c_data_bean->double_dimen_array[i]);
        LOGD("sub_len: %d", sub_len);
        jintArray int_array = env->NewIntArray(sub_len);
        env->SetIntArrayRegion(int_array, 0, sub_len, c_data_bean->double_dimen_array[i]);
        env->SetObjectArrayElement(double_dimen_array, i, int_array);
    }
    env->SetObjectField(java_data_bean, m_data_bean_block.double_dimen_array, double_dimen_array);

    return java_data_bean;
}
```

#### b. Java转C
```cpp
void data_bean_java_to_c(JNIEnv *env, jobject data_bean_in, jni_data_bean *data_bean_out) {
    if (data_bean_in == nullptr) {
        LOGW("input data is null!");
        return;
    }
    LOGD("start data_bean_java_to_c");

    // 1. assign rect
    jobject rect = env->GetObjectField(data_bean_in, m_data_bean_block.rect);
    data_bean_out->rect.left = env->GetIntField(rect, m_rect_block.left);
    data_bean_out->rect.top = env->GetIntField(rect, m_rect_block.top);
    data_bean_out->rect.right = env->GetIntField(rect, m_rect_block.right);
    data_bean_out->rect.bottom = env->GetIntField(rect, m_rect_block.bottom);

    // 2. point array
    jobjectArray point_array = (jobjectArray) env->GetObjectField(data_bean_in, m_data_bean_block.points);
    jsize len = env->GetArrayLength(point_array);
    // len = NELEM(data_bean_out->points);
    LOGD("point array len: %d", len); // 注意这个 len 必须等于 NELEM(data_bean_out->points)
    for (int i = 0; i < len; i++) {
        jobject point = env->GetObjectArrayElement(point_array, i);
        data_bean_out->points[i].x = env->GetFloatField(point, m_point_block.x);
        data_bean_out->points[i].y = env->GetFloatField(point, m_point_block.y);
    }

    // 3. inner class
    jobject inner = env->GetObjectField(data_bean_in, m_data_bean_block.inner);
    jstring message = (jstring) env->GetObjectField(inner, m_inner_block.message);
    data_bean_out->message = env->GetStringUTFChars(message, 0);

    // 4. other
    data_bean_out->id = env->GetIntField(data_bean_in, m_data_bean_block.id);
    data_bean_out->score = env->GetFloatField(data_bean_in, m_data_bean_block.score);
    // byte array
    jbyteArray byte_array = (jbyteArray) env->GetObjectField(data_bean_in, m_data_bean_block.data);
    jbyte *data = env->GetByteArrayElements(byte_array, 0);
    len = env->GetArrayLength(byte_array);
    LOGD("byte array len: %d", len);
    memcpy(data_bean_out->data, data, len * sizeof(jbyte));
    env->ReleaseByteArrayElements(byte_array, data, 0);
    // double dimen int array
    jobjectArray array = (jobjectArray) env->GetObjectField(data_bean_in, m_data_bean_block.double_dimen_array);
    len = env->GetArrayLength(array); // 获取行数
    LOGD("double dimen int array len: %d", len);
    for (int i = 0; i < len; i++) {
        jintArray sub_array = (jintArray) env->GetObjectArrayElement(array, i); // 这步得到的就是一维数组了
        jint *int_array = env->GetIntArrayElements(sub_array, 0);
        jsize sub_len = env->GetArrayLength(sub_array); // 获取列数
        LOGD("sub_len: %d", sub_len);
        memcpy(data_bean_out->double_dimen_array[i], int_array, sub_len * sizeof(jint));
        env->ReleaseIntArrayElements(sub_array, int_array, 0);
    }
    LOGD("end data_bean_java_to_c");
}
```

### 5. 其它
以上其实大部分逻辑都完成了，其余没有列出来的代码和文件，可以在下面地址中找到：

[https://github.com/afei-cn/JniDemo](https://github.com/afei-cn/JniDemo)

这个工程可以直接在 Android 上运行和演示，并打印出相应信息。

## 四、相关链接
1. JNI数据类型和类型描述符介绍：[https://blog.csdn.net/afei__/article/details/80899758](https://blog.csdn.net/afei__/article/details/80899758)
2. JNI 方法及使用示例：[https://blog.csdn.net/afei__/article/details/81016413](https://blog.csdn.net/afei__/article/details/81016413)
3. NDK 学习系列汇总篇：[https://blog.csdn.net/afei__/article/details/81290711](https://blog.csdn.net/afei__/article/details/81290711)
