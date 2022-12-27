#include "loopstationjsonconverters.h"

#include <algorithm>

#include "jsonconverters.h"

namespace json_converters {
namespace loopstation {

loopstation_presets::PresetsConfig parsePresetsConfig(const QJsonObject &jsonObj)
{
    loopstation_presets::PresetsConfig presetConfig;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        if (iter.key() == "categories")
            presetConfig.categories = parseCategoryVector(iter.value());
        else if (iter.key() == "presets")
            presetConfig.presets = parsePresetMap(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for PresetConfig"}.arg(iter.key(), jsonTypeToString(iter->type())).toStdString()};
    }

    return presetConfig;
}

std::vector<loopstation_presets::Category> parseCategoryVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for vector of Category is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<loopstation_presets::Category> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseCategory(jsonValue));

    return vector;
}

std::map<QString, loopstation_presets::Preset> parsePresetMap(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Preset map is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    std::map<QString, loopstation_presets::Preset> map;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
        map[iter.key()] = parsePreset(iter.value());

    return map;
}

loopstation_presets::Category parseCategory(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Category is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    loopstation_presets::Category category;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        if (iter.key() == "title")
            category.title = parseString(iter.value());
        else if (iter.key() == "filter")
            category.filter = parseFilter(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Category"}.arg(iter.key(), jsonTypeToString(iter->type())).toStdString()};
    }

    return category;
}

loopstation_presets::Filter parseFilter(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Filters is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    loopstation_presets::Filter filters;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        if (iter.key() == "tags")
            filters.tags = parseStringVector(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Filters"}.arg(iter.key(), jsonTypeToString(iter->type())).toStdString()};
    }

    return filters;
}

loopstation_presets::Preset parsePreset(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Preset is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    loopstation_presets::Preset preset;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        const auto key = iter.key();
        if (key == "id")
            preset.id = parseString(iter.value());
        else if (key == "audioPreviewUrl")
            preset.audioPreviewUrl = parseString(iter.value());
        else if (key == "author")
            preset.author = parseString(iter.value());
        else if (key == "bpm")
            preset.bpm = parseInt(iter.value());
        else if (key == "coverUrl")
            preset.coverUrl = parseString(iter.value());
        else if (key == "lessons")
            preset.lessons = parseLessons(iter.value());
        else if (key == "loopLength")
            preset.loopLength = parseInt(iter.value());
        else if (key == "orderBy")
            preset.orderBy = parseString(iter.value());
        else if (key == "pads")
            preset.pads = parseStringArray<48>(iter.value());
        else if (key == "premium")
            preset.premium = parseBoolStr(iter.value());
        else if (key == "tags")
            preset.tags = parseStringVector(iter.value());
        else if (key == "title")
            preset.title = parseString(iter.value());
        else if (key == "beatSchool")
        {
            // {}
        }
        else if (key == "DELETED")
            preset.DELETED = parseBool(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Preset"}.arg(key, jsonTypeToString(iter->type())).toStdString()};
    }

    return preset;
}

std::vector<loopstation_presets::Lesson> parseLessons(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for vector of Lesson is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<loopstation_presets::Lesson> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseLesson(jsonValue));

    return vector;
}

loopstation_presets::Lesson parseLesson(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Lesson is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    loopstation_presets::Lesson lesson;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        const auto key = iter.key();
        if (key == "version")
            lesson.version = parseInt(iter.value());
        else if (key == "name")
            lesson.name = parseString(iter.value());
        else if (key == "id")
            lesson.id = parseInt(iter.value());
        else if (key == "orderBy")
            lesson.orderBy = parseInt(iter.value());
        else if (key == "pads")
            lesson.pads = parseLessonPadVectorMap(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Lesson"}.arg(key, jsonTypeToString(iter->type())).toStdString()};
    }

    return lesson;
}

std::map<QString, std::vector<loopstation_presets::LessonPad>> parseLessonPadVectorMap(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for LessonPad vector map is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    std::map<QString, std::vector<loopstation_presets::LessonPad>> map;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
        map[iter.key()] = parseLessonPadVector(iter.value());

    return map;

}

std::vector<loopstation_presets::LessonPad> parseLessonPadVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for vector of LessonPad is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<loopstation_presets::LessonPad> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseLessonPad(jsonValue));

    return vector;
}

loopstation_presets::LessonPad parseLessonPad(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for LessonPad is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    loopstation_presets::LessonPad lessonPad;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        const auto key = iter.key();
        if (key == "tap")
            lessonPad.tap = parseInt(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for LessonPad"}.arg(key, jsonTypeToString(iter->type())).toStdString()};
    }

    return lessonPad;
}

} // namespace loopstation
} // namespace json_converters
