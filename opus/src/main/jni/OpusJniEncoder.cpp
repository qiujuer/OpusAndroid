//
// Created by qiujuer on 2018/10/28.
//
#include <jni.h>
#include "OpusJniEncoder.h"
#include "opus/include/opus.h"
#define CLASS_NAME_PATH "net/qiujuer/opus/OpusEncoder"
extern "C"
{
#include "native-opus.h"
}

jlong Java_OpusEncoder_Create(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels, jint application)
{
    return (jlong)native_opus_encoder_init((int)samplingRate, (int)numberOfChannels, (int)application);
}

jint Java_OpusEncoder_SetBitrate(JNIEnv *env, jobject obj, jlong opusPtr, jint bitrate)
{
    return native_opus_encoder_set_bitrate(opusPtr, bitrate);
}

jint Java_OpusEncoder_SetComplexity(JNIEnv *env, jobject obj, jlong opusPtr, jint complexity)
{
    return native_opus_encoder_set_complexity(opusPtr, complexity);
}

jint Java_OpusEncoder_EncodeBytes(JNIEnv *env, jobject obj, jlong opusPtr, jbyteArray in, jbyteArray out, jint frames)
{
    jint outputArraySize = env->GetArrayLength(out);

    jbyte *audioSignal = env->GetByteArrayElements(in, 0);
    jbyte *encodedSignal = env->GetByteArrayElements(out, 0);

    int samples = native_opus_encoder_encode_bytes(opusPtr, (const unsigned char *)audioSignal, frames,
                                                   (const unsigned char *)encodedSignal, frames);

    env->ReleaseByteArrayElements(in, audioSignal, JNI_ABORT);
    env->ReleaseByteArrayElements(out, encodedSignal, 0);

    return samples;
}

jboolean Java_OpusEncoder_Release(JNIEnv *env, jobject obj, jlong opusPtr)
{
    return native_opus_encoder_release(opusPtr);
}

static JNINativeMethod methods[] = {
    {"nCreate", "(III)J", (void *)Java_OpusEncoder_Create},
    {"nSetBitrate", "(JI)I", (void *)Java_OpusEncoder_SetBitrate},
    {"nSetComplexity", "(JI)I", (void *)Java_OpusEncoder_SetComplexity},
    {"nEncodeBytes", "(J[B[BI)I", (void *)Java_OpusEncoder_EncodeBytes},
    {"nRelease", "(J)Z", (void *)Java_OpusEncoder_Release}};

int registerOpusEncoderJniMethods(JNIEnv *env)
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