#pragma once

#include "loopstationpresets.h"

class QJsonObject;
class QJsonValue;

namespace json_converters {
namespace loopstation {

loopstation_presets::PresetsConfig parsePresetsConfig(const QJsonObject &jsonObj);
std::vector<loopstation_presets::Category> parseCategoryVector(const QJsonValue &jsonValue);
std::map<QString, loopstation_presets::Preset> parsePresetMap(const QJsonValue &jsonValue);
loopstation_presets::Category parseCategory(const QJsonValue &jsonValue);
loopstation_presets::Filter parseFilter(const QJsonValue &jsonValue);
loopstation_presets::Preset parsePreset(const QJsonValue &jsonValue);
std::vector<loopstation_presets::Lesson> parseLessons(const QJsonValue &jsonValue);
loopstation_presets::Lesson parseLesson(const QJsonValue &jsonValue);
std::map<QString, std::vector<loopstation_presets::LessonPad>> parseLessonPadVectorMap(const QJsonValue &jsonValue);
std::vector<loopstation_presets::LessonPad> parseLessonPadVector(const QJsonValue &jsonValue);
loopstation_presets::LessonPad parseLessonPad(const QJsonValue &jsonValue);

} // namespace loopstation
} // namespace json_converters
