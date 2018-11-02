#include <jni.h>
#include <stdlib.h>
#include "native-log.h"
#include "OpusJniDecoder.h"
#include "OpusJniEncoder.h"
#include "OpusJniConstant.h"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        LOGE("JNI_OnLoad error: failed to getEnv!");
        return JNI_ERR;
    }

    if (registerOpusConstantJniMethods(env) == JNI_ERR)
    {
        LOGE("JNI_OnLoad error: failed to registerOpusConstantJniMethods");
        return JNI_ERR;
    }

    if (registerOpusDecoderJniMethods(env) == JNI_ERR)
    {
        LOGE("JNI_OnLoad error: failed to registerOpusDecoderJniMethods");
        return JNI_ERR;
    }

    if (registerOpusEncoderJniMethods(env) == JNI_ERR)
    {
        LOGE("JNI_OnLoad error: failed to registerOpusEncoderJniMethods");
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}