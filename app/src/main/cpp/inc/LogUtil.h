#ifndef _LOG_UTILS_H_
#define _LOG_UTILS_H_

#ifdef __ANDROID__
#include <android/log.h>
#endif
#include <stdio.h>
#include <string.h>

// 根据编译环境选择 Windows 或 Linux
#define Windows
//#define Linux

#ifdef Windows
#define __FILENAME__ (strrchr(__FILE__, '\\') + 1)
#else // Linux
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)
#endif

#ifdef __ANDROID__
// TAG 可自定义
#define TAG "JNI_Demo"
#define LOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define LOGD(format, ...) printf("[%s][%s][%d]: " format "\n",\
        __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGW(format, ...) printf("[%s][%s][%d]: " format "\n",\
        __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGE(format, ...) printf("[%s][%s][%d]: " format "\n",\
        __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#endif

#endif // _LOG_UTILS_H_