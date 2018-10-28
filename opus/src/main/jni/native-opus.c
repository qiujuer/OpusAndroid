#include <malloc.h>
#include "opus/include/opus.h"
#include "native-opus.h"

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
    OpusDecoder *dec;
    dec = (OpusDecoder *)ptr;
    return opus_decode(dec, in, inSize, (opus_int16 *)out, frames, 0);
}

int native_opus_decoder_release(long ptr)
{
    OpusDecoder *dec;
    dec = (OpusDecoder *)ptr;
    free(dec);
    return 1;
}
