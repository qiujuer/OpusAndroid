#include <malloc.h>
#include "opus/include/opus.h"
#include "native-opus.h"
// Encoder
long native_opus_encoder_init(int samplingRate, int numberOfChannels, int application)
{
    int error;
    int size;

    size = opus_encoder_get_size(1);
    OpusEncoder *enc = malloc(size);
    error = opus_encoder_init(enc, samplingRate, numberOfChannels, application);

    if (error)
    {
        free(enc);
        return 0;
    }
    else
    {
        return enc;
    }
}

int native_opus_encoder_set_bitrate(long opusPtr, int bitrate)
{
    OpusEncoder *enc = (OpusEncoder *)(opusPtr);
    return opus_encoder_ctl(enc, OPUS_SET_BITRATE(bitrate));
}

int native_opus_encoder_set_complexity(long opusPtr, int complexity)
{
    OpusEncoder *enc = (OpusEncoder *)(opusPtr);
    return opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(complexity));
}

int native_opus_encoder_encode_bytes(long opusPtr, const unsigned char *in, int frames, const unsigned char *out, int outSize)
{
    OpusEncoder *enc = (OpusEncoder *)(opusPtr);

    if (((unsigned long)in) % 2)
    {
        // Unaligned...
        return OPUS_BAD_ARG;
    }

    return opus_encode(enc, (const opus_int16 *)in, frames,
                       (unsigned char *)out, outSize);
}

int native_opus_encoder_release(long opusPtr)
{
    OpusEncoder *enc = (OpusEncoder *)(opusPtr);
    free(enc);
    return 1;
}

// Decoder
long native_opus_decoder_init(int samplingRate, int numberOfChannels)
{
    int size;
    int error;

    size = opus_decoder_get_size(numberOfChannels);
    OpusDecoder *dec = malloc(size);
    error = opus_decoder_init(dec, samplingRate, numberOfChannels);

    if (error)
    {
        free(dec);
        return 0;
    }
    else
    {
        return dec;
    }
}

int native_opus_decoder_decode_bytes(long ptr, const unsigned char *in, int inSize, const unsigned char *out, int frames)
{
    OpusDecoder *dec = (OpusDecoder *)ptr;
    return opus_decode(dec, in, inSize, (opus_int16 *)out, frames, 0);
}

int native_opus_decoder_release(long ptr)
{
    OpusDecoder *dec = (OpusDecoder *)ptr;
    free(dec);
    return 1;
}
