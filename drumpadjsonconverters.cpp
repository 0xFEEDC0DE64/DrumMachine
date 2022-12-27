#include "drumpadjsonconverters.h"

#include "jsonconverters.h"

namespace json_converters {
namespace drumpad {

drumpad_presets::PresetsConfig parsePresetsConfig(const QJsonObject &jsonObj)
{
    drumpad_presets::PresetsConfig presetConfig;

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

std::vector<drumpad_presets::Category> parseCategoryVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for vector of Category is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<drumpad_presets::Category> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseCategory(jsonValue));

    return vector;
}

std::map<QString, drumpad_presets::Preset> parsePresetMap(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Preset map is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    std::map<QString, drumpad_presets::Preset> map;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
        map[iter.key()] = parsePreset(iter.value());

    return map;
}

drumpad_presets::Category parseCategory(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Category is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    drumpad_presets::Category category;

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

drumpad_presets::Filter parseFilter(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Filters is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    drumpad_presets::Filter filters;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        if (iter.key() == "tags")
            filters.tags = parseStringVector(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Filters"}.arg(iter.key(), jsonTypeToString(iter->type())).toStdString()};
    }

    return filters;
}

drumpad_presets::Preset parsePreset(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Preset is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    drumpad_presets::Preset preset;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        const auto key = iter.key();
        if (key == "id")
            preset.id = parseString(iter.value());
        else if (key == "name")
            preset.name = parseString(iter.value());
        else if (key == "author")
            preset.author = parseString(iter.value());
        else if (key == "orderBy")
            preset.orderBy = parseString(iter.value());
        else if (key == "version")
            preset.version = parseString(iter.value());
        else if (key == "tempo")
            preset.tempo = parseInt(iter.value());
        else if (key == "icon")
            preset.icon = parseString(iter.value());
        else if (key == "price")
            preset.price = parseInt(iter.value());
        else if (key == "priceForSession")
            preset.priceForSession = parseInt(iter.value());
        else if (key == "hasInfo")
            preset.hasInfo = parseBool(iter.value());
        else if (key == "tags")
            preset.tags = parseStringVector(iter.value());
        else if (key == "DELETED")
            preset.DELETED = parseBool(iter.value());
        else if (key == "difficulty")
            preset.difficulty = parseInt(iter.value());
        else if (key == "sample")
            preset.sample = parseInt(iter.value());
        else if (key == "audioPreview1Name")
            preset.audioPreview1Name = parseString(iter.value());
        else if (key == "audioPreview1URL")
            preset.audioPreview1URL = parseString(iter.value());
        else if (key == "audioPreview2Name")
            preset.audioPreview2Name = parseString(iter.value());
        else if (key == "audioPreview2URL")
            preset.audioPreview2URL = parseString(iter.value());
        else if (key == "imagePreview1")
            preset.imagePreview1 = parseString(iter.value());
        else if (key == "videoPreview")
            preset.videoPreview = parseString(iter.value());
        else if (key == "videoTutorial")
            preset.videoTutorial = parseString(iter.value());
        else if (key == "files")
            preset.files = parseFileArray(iter.value());
        else if (key == "beatSchool")
            preset.beatSchool = parseSequenceVectorMap(iter.value());
        else if (key == "easyPlay")
            preset.easyPlay = parseSequenceVectorMap(iter.value());
        else if (key == "timestamp")
            preset.timestamp = QDateTime::fromSecsSinceEpoch(iter.value().toDouble());
        else if (key == "middleDescription")
        {
            //preset.middleDescription = parseString(iter.value());

            // example value:
            // [
            //   {
            //     "type": "ButtonsRow",
            //     "buttons": [
            //       {
            //         "type": "ImageButton",
            //         "action": "openLink",
            //         "link": "https://www.soundcloud.com/matta-music",
            //         "icon": "soundcloud"
            //       },
            //       {
            //         "type": "ImageButton",
            //         "action": "openLink",
            //         "link": "https://www.facebook.com/MattaOfficial",
            //         "icon": "facebook"
            //       },
            //       {
            //         "type": "ImageButton",
            //         "action": "openLink",
            //         "link": "https://www.twitter.com/matta_music",
            //         "icon": "twitter"
            //       },
            //       {
            //         "type": "ImageButton",
            //         "action": "openLink",
            //         "link": "https://pro.beatport.com/artist/matta/118779",
            //         "icon": "beatport"
            //       },
            //       {
            //         "type": "ImageButton",
            //         "action": "openLink",
            //         "link": "https://www.mattaofficial.com",
            //         "icon": "mattaofficial"
            //       }
            //     ]
            //   }
            // ]
        }
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Preset"}.arg(key, jsonTypeToString(iter->type())).toStdString()};
    }

    return preset;
}

std::array<drumpad_presets::File, 24> parseFileArray(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for File array is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    if (jsonObj.size() != 24)
        throw std::runtime_error{QString{"json value for File array doesn't have exactly 24 entries but %0"}.arg(jsonObj.size()).toStdString()};

    std::array<drumpad_presets::File, 24> array;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        bool ok;
        const auto index = iter.key().toInt(&ok);

        if (!ok || index < 1 || index >= 25)
            throw std::runtime_error{QString{"unknown key %0 (%1) for File"}.arg(iter.key(), jsonTypeToString(iter->type())).toStdString()};

        array[index - 1] = parseFile(iter.value());
    }

    return array;
}

drumpad_presets::File parseFile(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for File is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    drumpad_presets::File file;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        const auto key = iter.key();
        if (key == "filename")
            file.filename = parseString(iter.value());
        else if (key == "color")
            file.color = parseString(iter.value());
        else if (key == "stopOnRelease")
            file.stopOnRelease = parseString(iter.value());
        else if (key == "looped")
            file.looped = parseBool(iter.value());
        else if (key == "choke")
            file.choke = parseInt(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for File"}.arg(key, jsonTypeToString(iter->type())).toStdString()};
    }

    return file;
}

std::vector<drumpad_presets::Sequence> parseSequenceVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for vector of Sequence is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<drumpad_presets::Sequence> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseSequence(jsonValue));

    return vector;
}

drumpad_presets::Sequence parseSequence(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for File is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    drumpad_presets::Sequence sequence;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        const auto key = iter.key();
        if (key == "name")
            sequence.name = parseString(iter.value());
        else if (key == "id")
            sequence.id = parseInt(iter.value());
        else if (key == "version")
            sequence.version = parseInt(iter.value());
        else if (key == "orderBy")
            sequence.orderBy = parseInt(iter.value());
        else if (key == "sequencerSize")
            sequence.sequencerSize = parseInt(iter.value());
        else if (key == "pads")
            sequence.pads = parseSequencePadVectorMap(iter.value());
        else if (key == "embientPads")
            sequence.embientPads = parseIntVectorIgnoreNulls(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Sequence"}.arg(key, jsonTypeToString(iter->type())).toStdString()};
    }

    return sequence;
}

std::map<QString, std::vector<drumpad_presets::Sequence>> parseSequenceVectorMap(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for Sequence vector map is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    std::map<QString, std::vector<drumpad_presets::Sequence>> map;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
        map[iter.key()] = parseSequenceVector(iter.value());

    return map;
}

drumpad_presets::SequencePad parseSequencePad(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for File is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    drumpad_presets::SequencePad sequencePad;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
    {
        if (iter.key() == "start")
            sequencePad.start = parseInt(iter.value());
        else if (iter.key() == "duration")
            sequencePad.duration = parseInt(iter.value());
        else if (iter.key() == "embient")
            sequencePad.embient = parseBool(iter.value());
        else
            throw std::runtime_error{QString{"unknown key %0 (%1) for Sequence"}.arg(iter.key(), jsonTypeToString(iter->type())).toStdString()};
    }

    return sequencePad;
}

std::vector<drumpad_presets::SequencePad> parseSequencePadVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for vector of SequencePad is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<drumpad_presets::SequencePad> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseSequencePad(jsonValue));

    return vector;
}

std::map<QString, std::vector<drumpad_presets::SequencePad>> parseSequencePadVectorMap(const QJsonValue &jsonValue)
{
    if (!jsonValue.isObject())
        throw std::runtime_error{QString{"json value for SequencePad vector map is not an object but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonObj = jsonValue.toObject();

    std::map<QString, std::vector<drumpad_presets::SequencePad>> map;

    for (auto iter = std::cbegin(jsonObj); iter != std::cend(jsonObj); iter++)
        map[iter.key()] = parseSequencePadVector(iter.value());

    return map;
}

} // namespace drumpad
} // namespace json_converters
