#pragma once

#include "drumpadpresets.h"

class QJsonObject;
class QJsonValue;

namespace json_converters {
namespace drumpad {

drumpad_presets::PresetsConfig parsePresetsConfig(const QJsonObject &jsonObj);
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

} // namespace drumpad
} // namespace json_converters
