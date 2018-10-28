//
// Created by qiujuer on 2018/10/28.
//
#include <jni.h>
#include <malloc.h>
#include "OpusJniDecoder.h"
#include "opus/include/opus.h"
#define CLASS_NAME_PATH "net/qiujuer/opus/OpusDecoder"
extern "C"
{
#include "native-opus.h"
}

jlong Java_OpusDecoder_Create(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels)
{
    return (jlong)native_opus_decoder_init((int)samplingRate, (int)numberOfChannels);
}

jint Java_OpusDecoder_DecodeBytes(JNIEnv *env, jobject obj, jlong opusPtr, jbyteArray in, jbyteArray out, jint frames)
{
    jint inputArraySize = env->GetArrayLength(in);

    jbyte *encodedData = env->GetByteArrayElements(in, 0);
    jbyte *decodedData = env->GetByteArrayElements(out, 0);

    int samples = native_opus_decoder_decode_bytes(opusPtr, (const unsigned char *)encodedData, inputArraySize,
                                                   (const unsigned char *)decodedData, frames);

    env->ReleaseByteArrayElements(in, encodedData, JNI_ABORT);
    env->ReleaseByteArrayElements(out, decodedData, 0);

    return samples;
}

jboolean Java_OpusDecoder_Release(JNIEnv *env, jobject obj, jlong opusPtr)
{
    return native_opus_decoder_release(opusPtr);
}

static JNINativeMethod methods[] = {
    {"nCreate", "(II)J", (void *)Java_OpusDecoder_Create},
    {"nDecodeBytes", "(J[B[BI)I", (void *)Java_OpusDecoder_DecodeBytes},
    {"nRelease", "(J)Z", (void *)Java_OpusDecoder_Release}};

int registerOpusDecoderJniMethods(JNIEnv *env)
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