//
// Created by qiujuer on 2018/10/28.
//
#include <jni.h>
#include "OpusJniDecoder.h"
#include "native-log.h"
#define CLASS_NAME_PATH "net/qiujuer/opus/OpusDecoder"
extern "C"
{
#include "native-opus.h"
#include "opus/include/opus.h"
#include "native-opus.h"
}

jlong Java_OpusDecoder_Create(JNIEnv *env, jobject obj, jint samplingRate, jint numberOfChannels)
{
    int error = 0;
    OpusDecoder *dec = opus_decoder_create((int)samplingRate, (int)numberOfChannels, &error);
    if (error)
    {
        LOGE("Decoder create error: %d", error);
        return 0;
    }
    else
    {
        long ptr = (long)dec;
        LOGI("Decoder create pointer: %ld", ptr);
        return (jlong)ptr;
    }
}

jint Java_OpusDecoder_DecodeBytes(JNIEnv *env, jobject obj, jlong opusPtr,
                                  jbyteArray data, jint dataOffset, jint dataLength,
                                  jbyteArray pcm, jint pcmOffset, jint pcmLength)
{
    //LOGI("Decoder decode: %ld, dPos:%d, dLen:%d, pPos:%d, pLen:%d", (long)opusPtr, (int)dataOffset, (int)dataLength, (int)pcmOffset, (int)pcmLength);

    jboolean isCopy = JNI_FALSE;
    jbyte *encodedData = env->GetByteArrayElements(data, &isCopy);
    jbyte *pcmData = env->GetByteArrayElements(pcm, &isCopy);

    unsigned char *encodedPacket = (unsigned char *)encodedData;
    if (dataOffset > 0)
    {
        encodedPacket += dataOffset;
    }

    unsigned char *pcmPacket = (unsigned char *)pcmData;
    if (pcmOffset > 0)
    {
        pcmPacket += pcmOffset;
    }

    OpusDecoder *dec = (OpusDecoder *)((long)opusPtr);

    int frames = opus_decoder_get_nb_samples(dec, encodedPacket, (opus_int32)dataLength);
    //LOGI("Decoder decode get frames: %d", frames);

    int samples = opus_decode(dec, encodedPacket, (int)dataLength, (opus_int16 *)pcmPacket, (int)frames, 0);

    env->ReleaseByteArrayElements(data, encodedData, JNI_ABORT);
    env->ReleaseByteArrayElements(pcm, pcmData, JNI_COMMIT);

    return samples;
}

jint Java_OpusDecoder_DecodeBytesWithFrames(JNIEnv *env, jobject obj, jlong opusPtr,
                                            jbyteArray data, jint dataOffset, jint dataLength,
                                            jbyteArray pcm, jint pcmOffset, jint frames)
{
    //LOGI("Decoder decode: %ld, dPos:%d, dLen:%d, pPos:%d, frames:%d", (long)opusPtr, (int)dataOffset, (int)dataLength, (int)pcmOffset, (int)frames);

    jboolean isCopy = JNI_FALSE;
    jbyte *encodedData = env->GetByteArrayElements(data, &isCopy);
    jbyte *pcmData = env->GetByteArrayElements(pcm, &isCopy);

    unsigned char *encodedPacket = (unsigned char *)encodedData;
    if (dataOffset > 0)
    {
        encodedPacket += dataOffset;
    }

    unsigned char *pcmPacket = (unsigned char *)pcmData;
    if (pcmOffset > 0)
    {
        pcmPacket += pcmOffset;
    }

    OpusDecoder *dec = (OpusDecoder *)((long)opusPtr);
    int samples = opus_decode(dec, encodedPacket, (int)dataLength, (opus_int16 *)pcmPacket, (int)frames, 0);

    env->ReleaseByteArrayElements(data, encodedData, JNI_ABORT);
    env->ReleaseByteArrayElements(pcm, pcmData, JNI_COMMIT);

    return samples;
}

void Java_OpusDecoder_Release(JNIEnv *env, jobject obj, jlong opusPtr)
{
    OpusDecoder *dec = (OpusDecoder *)((long)opusPtr);
    opus_decoder_destroy(dec);
}

static JNINativeMethod methods[] = {
    {"nCreate", "(II)J", (void *)Java_OpusDecoder_Create},
    {"nDecodeBytes", "(J[BII[BII)I", (void *)Java_OpusDecoder_DecodeBytes},
    {"nDecodeBytesWithFrames", "(J[BII[BII)I", (void *)Java_OpusDecoder_DecodeBytesWithFrames},
    {"nRelease", "(J)V", (void *)Java_OpusDecoder_Release}};

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