package net.qiujuer.opus;

public class OpusDecoder {
    /* Native pointer to OpusAndroidDecoder */
    private long address;

    public OpusDecoder(@Annotations.SamplingRate int sampleRate,
                       @Annotations.NumberOfChannels int channels) {
        long ptr = nCreate(sampleRate, channels);
        if (ptr == 0) {
            throw new RuntimeException("Initialize Opus Decoder error!");
        }
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
