#include <android/log.h>
#define LOG_TAG "OPUS_NATIVE"
#define  LOGV(...)  do { __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__); } while(0)
#define  LOGD(...)  do { __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__); } while(0)
#define  LOGI(...)  do { __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__); } while(0)
#define  LOGW(...)  do { __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__); } while(0)
#define  LOGE(...)  do { __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__); } while(0)
#define  LOGF(...)  do { __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__); } while(0)