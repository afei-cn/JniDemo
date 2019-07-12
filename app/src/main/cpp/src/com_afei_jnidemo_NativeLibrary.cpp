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
    register_classes(env); // 静态注册所有的类
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

void print(jni_data_bean *data_bean) {
    LOGD("rect: [%d, %d, %d, %d]",
         data_bean->rect.left, data_bean->rect.top, data_bean->rect.right, data_bean->rect.bottom);
    LOGD("points: (%f, %f), (%f, %f), (%f, %f), (%f, %f)",
         data_bean->points[0].x, data_bean->points[0].y,
         data_bean->points[1].x, data_bean->points[1].y,
         data_bean->points[2].x, data_bean->points[2].y,
         data_bean->points[3].x, data_bean->points[3].y);
    LOGD("message: %s", data_bean->message);
    LOGD("id: %d", data_bean->id);
    LOGD("score: %f", data_bean->score);
    for (int i = 0; i < 4; i++) {
        LOGD("data[%d]: %d", i, data_bean->data[i]);
    }
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            LOGD("double_dimen_array[%d][%d]: %d", i, j, data_bean->double_dimen_array[i][j]);
        }
    }
}