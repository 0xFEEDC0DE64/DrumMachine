#pragma once

#include <vector>
#include <map>
#include <optional>
#include <array>

#include <QString>
#include <QDateTime>

namespace drumpad_presets {

struct Filter
{
    std::optional<std::vector<QString>> tags;
};

struct Category
{
    std::optional<QString> title;
    std::optional<Filter> filter;
};

struct File
{
    std::optional<QString> filename;
    std::optional<QString> color; // purple, red, yellow, green, blue
    std::optional<QString> stopOnRelease;
    std::optional<bool> looped;
    std::optional<int> choke;
};

struct SequencePad
{
    std::optional<int> start;
    std::optional<int> duration;
    std::optional<bool> embient;
};

struct Sequence
{
    std::optional<QString> name;
    std::optional<int> id;
    std::optional<int> version;
    std::optional<int> orderBy;
    std::optional<int> sequencerSize;
    std::optional<std::map<QString, std::vector<SequencePad>>> pads;
    std::optional<std::vector<int>> embientPads;
};

struct Preset
{
    std::optional<QString> id;
    std::optional<QString> name;
    std::optional<QString> author;
    std::optional<QString> orderBy;
    std::optional<QString> version;
    std::optional<int> tempo;
    std::optional<QString> icon;
    std::optional<int> price;
    std::optional<int> priceForSession;
    std::optional<bool> hasInfo;
    std::optional<std::vector<QString>> tags;
    std::optional<bool> DELETED;
    std::optional<int> difficulty;
    std::optional<int> sample;
    std::optional<QString> audioPreview1Name;
    std::optional<QString> audioPreview1URL;
    std::optional<QString> audioPreview2Name;
    std::optional<QString> audioPreview2URL;
    std::optional<QString> imagePreview1;
    std::optional<QString> videoPreview;
    std::optional<QString> videoTutorial;
    std::optional<std::array<drumpad_presets::File, 24>> files;
    std::optional<std::map<QString, std::vector<Sequence>>> beatSchool;
    std::optional<std::map<QString, std::vector<Sequence>>> easyPlay;
    std::optional<QDateTime> timestamp;
};

struct PresetsConfig
{
    std::optional<std::vector<Category>> categories;
    std::optional<std::map<QString, Preset>> presets;
};

} // namespace drumpad_presets
