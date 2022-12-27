#pragma once

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

namespace json_converters {

QString jsonTypeToString(QJsonValue::Type type);
QJsonObject loadJson(const QByteArray &buffer);
QString parseString(const QJsonValue &jsonValue);
std::vector<QString> parseStringVector(const QJsonValue &jsonValue);
int parseInt(const QJsonValue &jsonValue);
bool parseBool(const QJsonValue &jsonValue);
bool parseBoolStr(const QJsonValue &jsonValue);
std::vector<int> parseIntVector(const QJsonValue &jsonValue);
std::vector<int> parseIntVectorIgnoreNulls(const QJsonValue &jsonValue);
template<std::size_t LENGTH>
std::array<QString, LENGTH> parseStringArray(const QJsonValue &jsonValue);

} // namespace json_converters
