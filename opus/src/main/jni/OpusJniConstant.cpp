#include <jni.h>
#include "OpusJniConstant.h"
#define CLASS_NAME_PATH "net/qiujuer/opus/OpusConstant"
extern "C"
{
#include "opus/include/opus.h"
}

jint Java_OpusConstant_OpusPcmStructSizeOfByte(JNIEnv *env, jobject obj)
{
    return (jint)sizeof(opus_int16);
}

static JNINativeMethod methods[] = {
    {"nOpusPcmStructSizeOfByte", "()I", (void *)Java_OpusConstant_OpusPcmStructSizeOfByte}};

int registerOpusConstantJniMethods(JNIEnv *env)
{

    jclass cls = env->FindClass(CLASS_NAME_PATH);
    if (cls == NULL)
    {
        return JNI_ERR;
    }

    jint nRes = env->RegisterNatives(cls, methods, sizeof(methods) / sizeof(methods[0]));
    if (nRes < 0)
    {
        return JNI_ERR;
    }

    return JNI_OK;
}