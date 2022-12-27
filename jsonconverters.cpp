#include "jsonconverters.h"

#include <stdexcept>

#include <QJsonParseError>

namespace json_converters {

QString jsonTypeToString(QJsonValue::Type type)
{
    switch (type)
    {
    case QJsonValue::Null: return "Null";
    case QJsonValue::Bool: return "Bool";
    case QJsonValue::Double: return "Double";
    case QJsonValue::String: return "String";
    case QJsonValue::Array: return "Array";
    case QJsonValue::Object: return "Object";
    case QJsonValue::Undefined: return "Undefined";
    default:
        qWarning() << "unknown jsonType" << type;
        return QString{"unknown jsonType %0"}.arg(type);
    }
}

QJsonObject loadJson(const QByteArray &buffer)
{
    QJsonParseError error;
    QJsonDocument document{QJsonDocument::fromJson(buffer, &error)};
    if (error.error != QJsonParseError::NoError)
        throw std::runtime_error{QString{"Could not parse JSON because %0"}.arg(error.errorString()).toStdString()};

    if (!document.isObject())
        throw std::runtime_error{"JSON is not an object"};

    return document.object();
}

QString parseString(const QJsonValue &jsonValue)
{
    if (!jsonValue.isString())
        throw std::runtime_error{QString{"json value for string is not a string but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    return jsonValue.toString();
}

std::vector<QString> parseStringVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for string vector is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<QString> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseString(jsonValue));

    return vector;
}

int parseInt(const QJsonValue &jsonValue)
{
    if (!jsonValue.isDouble())
        throw std::runtime_error{QString{"json value for int is not a double but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    return jsonValue.toDouble();
}

bool parseBool(const QJsonValue &jsonValue)
{
    if (!jsonValue.isBool())
        throw std::runtime_error{QString{"json value for bool is not a bool but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    return jsonValue.toBool();
}

bool parseBoolStr(const QJsonValue &jsonValue)
{
    if (jsonValue.isBool())
        return jsonValue.toBool();
    else if (jsonValue.isString())
    {
        if (const auto &str = jsonValue.toString(); str == "true")
            return true;
        else if (str == "false")
            return false;
        else
            throw std::runtime_error{QString{"json value for str-bool has invalid str value %0"}.arg(str).toStdString()};
    }
    else
        throw std::runtime_error{QString{"json value for str-bool is not a bool or string but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

}

std::vector<int> parseIntVector(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for int vector is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<int> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        vector.emplace_back(parseInt(jsonValue));

    return vector;
}

std::vector<int> parseIntVectorIgnoreNulls(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for int vector is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    std::vector<int> vector;

    for (const auto &jsonValue : jsonValue.toArray())
        if (!jsonValue.isNull())
            vector.emplace_back(parseInt(jsonValue));

    return vector;
}

template<std::size_t LENGTH>
std::array<QString, LENGTH> parseStringArray(const QJsonValue &jsonValue)
{
    if (!jsonValue.isArray())
        throw std::runtime_error{QString{"json value for array of PadType is not an array but %0"}.arg(jsonTypeToString(jsonValue.type())).toStdString()};

    const auto jsonArr = jsonValue.toArray();

    if (jsonArr.size() != LENGTH)
        throw std::runtime_error{QString{"json value for PadType array doesn't have exactly %0 entries but %1"}.arg(LENGTH).arg(jsonArr.size()).toStdString()};

    std::array<QString, LENGTH> array;

    std::transform(std::cbegin(jsonArr), std::cend(jsonArr), std::begin(array), parseString);

    return array;
}

template std::array<QString, 48> parseStringArray(const QJsonValue &jsonValue);

} // namespace json_converters
