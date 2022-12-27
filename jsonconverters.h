#pragma once

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include "drumpadpresets.h"

namespace json_converters
{
QJsonObject loadJson(const QByteArray &buffer);

QString parseString(const QJsonValue &jsonValue);
std::vector<QString> parseStringVector(const QJsonValue &jsonValue);
int parseInt(const QJsonValue &jsonValue);
bool parseBool(const QJsonValue &jsonValue);
std::vector<int> parseIntVector(const QJsonValue &jsonValue);
std::vector<int> parseIntVectorIgnoreNulls(const QJsonValue &jsonValue);

drumpad_presets::PresetsConfig parseDrumPadPresetsConfig(const QJsonObject &jsonObj);
std::vector<drumpad_presets::Category> parseCategoryVector(const QJsonValue &jsonValue);
std::map<QString, drumpad_presets::Preset> parsePresetMap(const QJsonValue &jsonValue);
drumpad_presets::Category parseCategory(const QJsonValue &jsonValue);
drumpad_presets::Filter parseFilter(const QJsonValue &jsonValue);
drumpad_presets::Preset parsePreset(const QJsonValue &jsonValue);
std::array<drumpad_presets::File, 24> parseFileArray(const QJsonValue &jsonValue);
drumpad_presets::File parseFile(const QJsonValue &jsonValue);
std::vector<drumpad_presets::Sequence> parseSequenceVector(const QJsonValue &jsonValue);
drumpad_presets::Sequence parseSequence(const QJsonValue &jsonValue);
std::map<QString, std::vector<drumpad_presets::Sequence>> parseSequenceVectorMap(const QJsonValue &jsonValue);
drumpad_presets::SequencePad parseSequencePad(const QJsonValue &jsonValue);
std::vector<drumpad_presets::SequencePad> parseSequencePadVector(const QJsonValue &jsonValue);
std::map<QString, std::vector<drumpad_presets::SequencePad>> parseSequencePadVectorMap(const QJsonValue &jsonValue);
}
