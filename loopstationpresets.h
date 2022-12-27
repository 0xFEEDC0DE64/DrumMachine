#pragma once

#include <vector>
#include <map>
#include <optional>
#include <array>

#include <QString>
#include <QDateTime>

namespace loopstation_presets {

struct Filter
{
    std::optional<std::vector<QString>> tags;
};

struct Category
{
    std::optional<QString> title;
    std::optional<Filter> filter;
};

struct LessonPad
{
    std::optional<int> tap;
};

struct Lesson
{
    std::optional<int> version;
    std::optional<QString> name;
    std::optional<int> id;
    std::optional<int> orderBy;
    std::optional<std::map<QString, std::vector<LessonPad>>> pads;
};

struct Preset
{
    std::optional<QString> id;
    std::optional<QString> audioPreviewUrl;
    std::optional<QString> author;
    std::optional<int> bpm;
    std::optional<std::vector<Lesson>> lessons;
    std::optional<QString> coverUrl;
    std::optional<int> loopLength;
    std::optional<QString> orderBy;
    std::optional<std::array<QString, 48>> pads;
    std::optional<bool> premium;
    std::optional<std::vector<QString>> tags;
    std::optional<QString> title;
    // TODO beatschool
    std::optional<bool> DELETED;
};

struct PresetsConfig
{
    std::optional<std::vector<Category>> categories;
    std::optional<std::map<QString, Preset>> presets;
};

} // namespace loopstation_presets
