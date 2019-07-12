#include "Register.h"
#include "LogUtil.h"

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

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
        LOGE("register_data_bean_class  failed");
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