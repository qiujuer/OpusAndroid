package net.qiujuer.opus;

import android.support.annotation.IntDef;
import android.support.annotation.IntRange;
import android.support.annotation.NonNull;

import java.io.IOException;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

public class OpusEncoder {
    public static final int OPUS_AUTO = -1;
    public static final int OPUS_BITRATE_MAX = -1;
    public static final int OPUS_COMPLEXITY_MAX = 10;

    @SuppressWarnings("WeakerAccess")
    @Retention(RetentionPolicy.SOURCE)
    @IntDef({OPUS_APPLICATION_VOIP, OPUS_APPLICATION_AUDIO, OPUS_APPLICATION_RESTRICTED_LOWDELAY})
    public @interface ApplicationType {
    }

    public static final int OPUS_APPLICATION_VOIP = 2048;
    public static final int OPUS_APPLICATION_AUDIO = 2049;
    public static final int OPUS_APPLICATION_RESTRICTED_LOWDELAY = 2051;

    // Native pointer to OpusAndroidEncoder.
    private long mAddress;
    private final int mChannels;

    public OpusEncoder(@Annotations.SamplingRate int sampleRate,
                       @Annotations.NumberOfChannels int channels,
                       @ApplicationType int application) {
        long ptr = nCreate(sampleRate, channels, application);
        if (ptr == 0) {
            throw new RuntimeException("Initialize Opus Encoder error!");
        }
        mAddress = ptr;
        mChannels = channels;
    }

    public void setBitrate(int bitrate) {
        nSetBitrate(mAddress, bitrate);
    }

    public void setComplexity(@IntRange(from = 0, to = 10) int complexity) {
        nSetComplexity(mAddress, complexity);
    }

    public int encode(@NonNull byte[] pcm, @NonNull byte[] coded, @IntRange(from = 1) int frames) throws IOException {
        return encode(pcm, 0, pcm.length, coded, 0, coded.length, frames);
    }

    public int encode(@NonNull byte[] pcm, @NonNull byte[] coded, @IntRange(from = 0) int codedOffset,
                      @IntRange(from = 1) int frames) throws IOException {
        return encode(pcm, 0, pcm.length, coded, codedOffset, coded.length - codedOffset, frames);
    }

    public int encode(@NonNull byte[] pcm, @IntRange(from = 0) int pcmOffset,
                      @NonNull byte[] coded, @IntRange(from = 0) int codedOffset,
                      @IntRange(from = 1) int frames) throws IOException {
        return encode(pcm, pcmOffset, pcm.length - pcmOffset, coded, codedOffset, coded.length - codedOffset, frames);
    }

    public int encode(@NonNull byte[] pcm, @IntRange(from = 0) int pcmOffset, @IntRange(from = 1) int pcmLength,
                      @NonNull byte[] coded, @IntRange(from = 0) int codedOffset, @IntRange(from = 1) int codedLength,
                      @IntRange(from = 1) int frames) throws IOException {

        if (mAddress == 0) {
            throw new IOException("Opus Released!");
        }

        if (frames < 1) {
            throw new IllegalArgumentException("Frames must from 1!");
        }

        if (pcm.length == 0 || coded.length == 0) {
            throw new IllegalArgumentException("Decode data array and pcm array not allow empty!");
        }


        if (codedOffset < 0 || pcmOffset < 0 || codedLength < 0 || pcmLength < 0 ||
                codedOffset > coded.length - codedLength || pcmOffset > pcm.length - pcmLength) {
            throw new ArrayIndexOutOfBoundsException("coded.length=" + coded.length + " codedOffset=" + codedOffset + " codedLength=" + codedLength +
                    " pcm.length=" + pcm.length + " pcmOffset=" + pcmOffset + " pcmLength=" + pcmLength);
        }

        if (pcmLength < mChannels * frames * OpusConstant.OPUS_PCM_STRUCT_SIZE_OF_BYTE) {
            throw new IllegalArgumentException("PcmLength must >= frames*channels*OpusConstant.OPUS_PCM_STRUCT_SIZE_OF_BYTE");
        }

        return nEncodeBytes(mAddress, pcm, pcmOffset, coded, codedOffset, codedLength, frames);
    }

    public void release() {
        nRelease(mAddress);
        mAddress = 0;
    }

    private native long nCreate(int samplingRate, int numberOfChannels, int application);

    private native int nSetBitrate(long opusPtr, int bitrate);

    private native int nSetComplexity(long opusPtr, int complexity);

    private native int nEncodeBytes(long opusPtr, byte[] pcm, int pcmOffset,
                                    byte[] coded, int codedOffset, int codedLength, int frames);

    private native boolean nRelease(long opusPtr);

    static {
        System.loadLibrary(OpusConstant.LIB_NAME);
    }
}
