#pragma once

#include <QAudioFormat>

static constexpr int frameRate = 44100;
static constexpr int channelCount = 2;
static constexpr int sampleSize = 32;
static constexpr auto codec = "audio/pcm";
static constexpr QAudioFormat::Endian byteOrder = QAudioFormat::LittleEndian;
static constexpr QAudioFormat::SampleType sampleType = QAudioFormat::Float;
using sample_t = float;
using frame_t = std::array<sample_t, channelCount>;

const QAudioFormat &audioFormat();
