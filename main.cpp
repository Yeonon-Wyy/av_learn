
#if defined(__cplusplus)
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#if defined(__cplusplus)
}
#endif

#include <string>
#include <iostream>
#include <fstream>
#include "device.h"
#include "resample.h"
#include "codec.h"
#include "log.h"

void initParam()
{
    avdevice_register_all();    
    // av_log_set_level(AV_LOG_DEBUG);
}

int main()
{
    initParam();

    // create a device
    Device device("hw:0", DeviceType::AUDIO);

    SwrContextParam swrCtxParam = 
    {
        .out_ch_layout = AV_CH_LAYOUT_STEREO,
        .out_sample_fmt = AV_SAMPLE_FMT_S16,
        .out_sample_rate = 44100,
        .in_ch_layout = AV_CH_LAYOUT_STEREO,
        .in_sample_fmt = AV_SAMPLE_FMT_S16,
        .in_sample_rate = 48000,
        .log_offset = 0,
        .log_ctx = nullptr
    };

    AudioEncoderParam audioEncodeParam
    {
        .needEncode = true,
        .codecName = "libfdk_aac",
        .sampleFmt = AV_SAMPLE_FMT_S16,
        .channelLayout = AV_CH_LAYOUT_STEREO,
        .sampleRate = 44100,
        .bitRate = 0,
        .profile = FF_PROFILE_AAC_HE_V2,
        .byName = true,
    };

    AudioCodecParam encoderParam =
    {
        .encodeParam = audioEncodeParam
    };
    // start record and save output file
    device.readAudio("", "out.aac", swrCtxParam, encoderParam);

    Device device2("/home/yeonon/learn/av/demo/build/sample-6s.mp3", DeviceType::ENCAPSULATE_FILE);
    device2.readVideoData();
    device2.readAudioDataToPCM("out3.pcm", AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100);

    SwrContextParam swrCtxParam2 = 
    {
        .out_ch_layout = AV_CH_LAYOUT_STEREO,
        .out_sample_fmt = AV_SAMPLE_FMT_S16,
        .out_sample_rate = 44100,
        .in_ch_layout = AV_CH_LAYOUT_STEREO,
        .in_sample_fmt = AV_SAMPLE_FMT_S16,
        .in_sample_rate = 44100,
        .log_offset = 0,
        .log_ctx = nullptr
    };

    device2.readAudio("/home/yeonon/learn/av/demo/build/out3.pcm", "out100.aac", swrCtxParam2, encoderParam);

    return 0;
}