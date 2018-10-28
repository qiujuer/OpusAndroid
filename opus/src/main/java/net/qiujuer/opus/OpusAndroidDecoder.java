package net.qiujuer.opus;

import android.util.Log;

/**
 *
 */
public class OpusAndroidDecoder {
    /* Native pointer to OpusAndroidDecoder */
    private long address;

    public OpusAndroidDecoder(@Annotations.SamplingRate int sampleRate,
                              @Annotations.NumberOfChannels int channels) {
        long ptr = nCreate(sampleRate, channels);
        if (ptr == 0) {

        }
        Log.e("TAG", "OpusAndroidDecoder() called with: sampleRate = [" + sampleRate + "], channels = [" + channels + "]" + ptr);
        address = ptr;
    }

    public int decode(byte[] in, byte[] out, int frames) {
        return nDecodeBytes(address, in, out, frames);
    }

    public boolean release() {
        return nRelease(address);
    }


    private native long nCreate(int samplingRate, int numberOfChannels);

    private native int nDecodeBytes(long opusPtr, byte[] in, byte[] out, int frames);

    private native boolean nRelease(long opusPtr);

    static {
        System.loadLibrary("opus-lib");
    }
}
