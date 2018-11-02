package net.qiujuer.opus;

/**
 * Opus Constant values
 *
 * @author qiujuer Email: qiujuer@live.cn
 * @version 1.0.0
 * Create at: 2018/11/1
 */
@SuppressWarnings("WeakerAccess")
public class OpusConstant {
    static final String LIB_NAME = "opus-lib";
    public static final int OPUS_PCM_STRUCT_SIZE_OF_BYTE;

    /**
     * No error @hideinitializer
     */
    public static final int OPUS_OK = 0;
    /**
     * One or more invalid/out of range arguments @hideinitializer
     */
    public static final int OPUS_BAD_ARG = -1;
    /**
     * Not enough bytes allocated in the buffer @hideinitializer
     */
    public static final int OPUS_BUFFER_TOO_SMALL = -2;
    /**
     * An internal error was detected @hideinitializer
     */
    public static final int OPUS_INTERNAL_ERROR = -3;
    /**
     * The compressed data passed is corrupted @hideinitializer
     */
    public static final int OPUS_INVALID_PACKET = -4;
    /**
     * Invalid/unsupported request number @hideinitializer
     */
    public static final int OPUS_UNIMPLEMENTED = -5;
    /**
     * An encoder or decoder structure is invalid or already freed @hideinitializer
     */
    public static final int OPUS_INVALID_STATE = -6;
    /**
     * Memory allocation has failed @hideinitializer
     */
    public static final int OPUS_ALLOC_FAIL = -7;


    static {
        System.loadLibrary(LIB_NAME);
        OPUS_PCM_STRUCT_SIZE_OF_BYTE = nOpusPcmStructSizeOfByte();
    }

    private static native int nOpusPcmStructSizeOfByte();
}
