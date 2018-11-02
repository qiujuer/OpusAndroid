package net.qiujuer.opus;

import android.support.annotation.IntDef;
import android.support.annotation.IntRange;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

public class OpusEncoder {
    // Native pointer to OpusEncoder.
    private long address;

    public static final int OPUS_AUTO = -1;
    public static final int OPUS_BITRATE_MAX = -1;
    public static final int OPUS_COMPLEXITY_MAX = 10;

    @Retention(RetentionPolicy.SOURCE)
    @IntDef({OPUS_APPLICATION_VOIP, OPUS_APPLICATION_AUDIO, OPUS_APPLICATION_RESTRICTED_LOWDELAY})
    public @interface ApplicationType {
    }

    public static final int OPUS_APPLICATION_VOIP = 2048;
    public static final int OPUS_APPLICATION_AUDIO = 2049;
    public static final int OPUS_APPLICATION_RESTRICTED_LOWDELAY = 2051;


    static {
        System.loadLibrary(OpusConstant.LIB_NAME);
    }

    public OpusEncoder(@Annotations.SamplingRate int sampleRate,
                       @Annotations.NumberOfChannels int channels,
                       @ApplicationType int application) {
        long ptr = nCreate(sampleRate, channels, application);
        if (ptr == 0) {
            throw new RuntimeException("Initialize Opus Encoder error!");
        }
        address = ptr;
    }

    public void setBitrate(int bitrate) {
        nSetBitrate(address, bitrate);
    }

    public void setComplexity(@IntRange(from = 0, to = 10) int complexity) {
        nSetComplexity(address, complexity);
    }

    public int encode(byte[] in, byte[] out, int frames) {
        return nEncodeBytes(address, in, out, frames);
    }

    public void release() {
        nRelease(address);
        address = 0;
    }


    private native long nCreate(int samplingRate, int numberOfChannels, int application);

    private native int nSetBitrate(long opusPtr, int bitrate);

    private native int nSetComplexity(long opusPtr, int complexity);

    private native int nEncodeBytes(long opusPtr, byte[] in, byte[] out, int frames);

    private native boolean nRelease(long opusPtr);
}
