package net.qiujuer.opus;

import android.support.annotation.IntRange;
import android.support.annotation.NonNull;

import java.io.IOException;

public class OpusDecoder {
    /* Native pointer to OpusAndroidDecoder */
    private volatile long mAddress;
    private final int mChannels;

    public OpusDecoder(@Annotations.SamplingRate int sampleRate,
                       @Annotations.NumberOfChannels int channels) {
        long ptr = nCreate(sampleRate, channels);
        if (ptr == 0) {
            throw new RuntimeException("Initialize Opus Decoder error!");
        }
        mAddress = ptr;
        mChannels = channels;
    }

    public int decode(byte[] data, byte[] pcm) throws IOException {
        return decode(data, data.length, pcm);
    }

    public int decode(byte[] data, int dataLength, byte[] pcm) throws IOException {
        return decode(data, 0, dataLength, pcm, 0, pcm.length);
    }

    public int decode(byte[] data, byte[] pcm, int frames) throws IOException {
        return decode(data, data.length, pcm, frames);
    }

    public int decode(byte[] data, int dataLength, byte[] pcm, int frames) throws IOException {
        return decode(data, 0, dataLength, pcm, 0, pcm.length, frames);
    }

    public int decode(@NonNull byte[] data, @IntRange(from = 0) int dataOffset, @IntRange(from = 1) int dataLength,
                      @NonNull byte[] pcm, @IntRange(from = 0) int pcmOffset, @IntRange(from = 1) int pcmLength) throws IOException {
        if (mAddress == 0) {
            throw new IOException("Opus Released!");
        }

        if (data.length == 0 || pcm.length == 0) {
            throw new IllegalArgumentException("Decode data array and pcm array not allow empty!");
        }

        if (dataOffset < 0 || pcmOffset < 0 || dataLength < 1 || pcmLength < 1 ||
                dataOffset > data.length - dataLength || pcmOffset > pcm.length - pcmLength) {
            throw new ArrayIndexOutOfBoundsException("data.length=" + data.length + " dataOffset=" + dataOffset + " dataLength=" + dataLength +
                    " pcm.length=" + pcm.length + " pcmOffset=" + pcmOffset + " pcmLength=" + pcmLength);
        }

        int countOfSamples = nDecodeBytes(mAddress, data, dataOffset, dataLength, pcm, pcmOffset, pcmLength);
        if (countOfSamples > 2) {
            return countOfSamples * mChannels * OpusConstant.OPUS_PCM_STRUCT_SIZE_OF_BYTE;
        }
        return countOfSamples;
    }

    public int decode(@NonNull byte[] data, @IntRange(from = 0) int dataOffset, @IntRange(from = 1) int dataLength,
                      @NonNull byte[] pcm, @IntRange(from = 0) int pcmOffset, @IntRange(from = 1) int pcmLength,
                      @IntRange(from = 1) int frames) throws IOException {
        if (mAddress == 0) {
            throw new IOException("Opus Released!");
        }

        if (frames < 1) {
            throw new IllegalArgumentException("Frames must from 1!");
        }

        if (data.length == 0 || pcm.length == 0) {
            throw new IllegalArgumentException("Decode data array and pcm array not allow empty!");
        }

        if (dataOffset < 0 || pcmOffset < 0 || dataLength < 0 || pcmLength < 0 ||
                dataOffset > data.length - dataLength || pcmOffset > pcm.length - pcmLength) {
            throw new ArrayIndexOutOfBoundsException("data.length=" + data.length + " dataOffset=" + dataOffset + " dataLength=" + dataLength +
                    " pcm.length=" + pcm.length + " pcmOffset=" + pcmOffset + " pcmLength=" + pcmLength);
        }

        if (pcmLength < mChannels * frames * OpusConstant.OPUS_PCM_STRUCT_SIZE_OF_BYTE) {
            throw new IllegalArgumentException("PcmLength must >= frames*channels*OpusConstant.OPUS_PCM_STRUCT_SIZE_OF_BYTE");
        }

        int countOfSamples = nDecodeBytesWithFrames(mAddress, data, dataOffset, dataLength, pcm, pcmOffset, frames);
        if (countOfSamples > 2) {
            return countOfSamples * mChannels * OpusConstant.OPUS_PCM_STRUCT_SIZE_OF_BYTE;
        }
        return countOfSamples;
    }

    public void release() {
        nRelease(mAddress);
        mAddress = 0;
    }

    private native long nCreate(int samplingRate, int numberOfChannels);

    private native int nDecodeBytes(long address, byte[] data, int dataOffset, int dataLength, byte[] pcm, int pcmOffset, int pcmLength);

    private native int nDecodeBytesWithFrames(long address, byte[] data, int dataOffset, int dataLength, byte[] pcm, int pcmOffset, int frames);

    private native void nRelease(long opusPtr);

    static {
        System.loadLibrary(OpusConstant.LIB_NAME);
    }
}
