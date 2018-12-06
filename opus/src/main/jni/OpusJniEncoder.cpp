//
// Created by qiujuer on 2018/10/28.
//
#include <jni.h>
#include "OpusJniEncoder.h"
#include "native-log.h"
#define CLASS_NAME_PATH "net/qiujuer/opus/OpusEncoder"
extern "C"
{
#include "native-opus.h"
#include "opus/include/opus.h"
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

jint Java_OpusEncoder_EncodeBytes(JNIEnv *env, jobject obj, jlong opusPtr,
                                  jbyteArray pcm, jint pcmOffset,
                                  jbyteArray coded, jint codedOffset, jint codedLength, jint frames)
{
    //LOGI("Encoder encode: %ld, pPos:%d, cPos:%d, cLen:%d, frames:%d", (long)opusPtr, (int)pcmOffset, (int)codedOffset, (int)codedLength, (int)frames);

    jboolean isCopy = JNI_FALSE;
    jbyte *pcmData = env->GetByteArrayElements(pcm, &isCopy);
    jbyte *codedData = env->GetByteArrayElements(coded, &isCopy);

    unsigned char *pcmPacket = (unsigned char *)pcmData;
    if (pcmOffset > 0)
    {
        pcmPacket += pcmOffset;
    }

    unsigned char *encodedPacket = (unsigned char *)codedData;
    if (codedOffset > 0)
    {
        encodedPacket += codedOffset;
    }

    OpusEncoder *enc = (OpusEncoder *)((long)opusPtr);
    int samples = opus_encode(enc, (const opus_int16 *)pcmPacket, frames, encodedPacket, (int)codedLength);

    env->ReleaseByteArrayElements(pcm, pcmData, JNI_ABORT);
    env->ReleaseByteArrayElements(coded, codedData, JNI_COMMIT);

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
    {"nEncodeBytes", "(J[BI[BIII)I", (void *)Java_OpusEncoder_EncodeBytes},
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