long native_opus_decoder_init(int samplingRate, int numberOfChannels);
int native_opus_decoder_decode_bytes(long ptr, const unsigned char *in, int inSize, const unsigned char *out, int frames);
int native_opus_decoder_release(long ptr);
