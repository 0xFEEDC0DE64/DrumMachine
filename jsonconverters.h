#pragma once

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include "presets.h"

namespace json_converters
{
QJsonObject loadJson(const QByteArray &buffer);

QString parseString(const QJsonValue &jsonValue);
std::vector<QString> parseStringVector(const QJsonValue &jsonValue);
int parseInt(const QJsonValue &jsonValue);
bool parseBool(const QJsonValue &jsonValue);
std::vector<int> parseIntVector(const QJsonValue &jsonValue);
std::vector<int> parseIntVectorIgnoreNulls(const QJsonValue &jsonValue);

presets::PresetsConfig parsePresetsConfig(const QJsonObject &jsonObj);
std::vector<presets::Category> parseCategoryVector(const QJsonValue &jsonValue);
std::map<QString, presets::Preset> parsePresetMap(const QJsonValue &jsonValue);
presets::Category parseCategory(const QJsonValue &jsonValue);
presets::Filter parseFilter(const QJsonValue &jsonValue);
presets::Preset parsePreset(const QJsonValue &jsonValue);
std::array<presets::File, 24> parseFileArray(const QJsonValue &jsonValue);
presets::File parseFile(const QJsonValue &jsonValue);
std::vector<presets::Sequence> parseSequenceVector(const QJsonValue &jsonValue);
presets::Sequence parseSequence(const QJsonValue &jsonValue);
std::map<QString, std::vector<presets::Sequence>> parseSequenceVectorMap(const QJsonValue &jsonValue);
presets::SequencePad parseSequencePad(const QJsonValue &jsonValue);
std::vector<presets::SequencePad> parseSequencePadVector(const QJsonValue &jsonValue);
std::map<QString, std::vector<presets::SequencePad>> parseSequencePadVectorMap(const QJsonValue &jsonValue);
}
