long native_opus_encoder_init(int samplingRate, int numberOfChannels, int application);
int native_opus_encoder_set_bitrate(long opusPtr, int bitrate);
int native_opus_encoder_set_complexity(long opusPtr, int complexity);
int native_opus_encoder_encode_bytes(long opusPtr, const unsigned char *in, int frames, const unsigned char *out, int outSize);
int native_opus_encoder_release(long opusPtr);

long native_opus_decoder_init(int samplingRate, int numberOfChannels);
int native_opus_decoder_decode_bytes(long ptr, const unsigned char *in, int inSize, const unsigned char *out, int frames);
int native_opus_decoder_release(long ptr);