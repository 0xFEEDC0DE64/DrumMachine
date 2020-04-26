#include "audioformat.h"

const QAudioFormat &audioFormat()
{
    static const QAudioFormat audioFormat = [](){
        QAudioFormat format;
        format.setSampleRate(sampleRate);
        format.setChannelCount(channelCount);
        format.setSampleSize(sampleSize);
        format.setCodec(codec);
        format.setByteOrder(byteOrder);
        format.setSampleType(sampleType);
        return format;
    }();

    return audioFormat;
}
