#pragma once

#include <string>
#include <map>

#include <citygml/citygml_api.h>

namespace citygml
{

/**
 * @brief The AttributeType enum represents the data type of an object attribute
 */
enum class AttributeType
{
    String,
    Double,
    Integer,
    Date,
    Uri
};

/**
 * @brief The AttributeValue class stores an attribute value and its data type
 */
class LIBCITYGML_EXPORT AttributeValue
{
public:
    AttributeValue();
    AttributeValue(const char* value);
    AttributeValue(const std::string& value, AttributeType type=AttributeType::String);
    AttributeValue(double value);
    AttributeValue(int value);


    void setType(AttributeType type);
    AttributeType getType() const;

    void setValue(const std::string& value, AttributeType type=AttributeType::String);
    void setValue(double value);
    void setValue(int value);

    std::string asString() const;
    double asDouble(double defaultValue=0.0) const;
    int asInteger(int defaultValue=0) const;
private:
    AttributeType m_type;
    std::string m_value;
};

LIBCITYGML_EXPORT std::ostream& operator<<(std::ostream& os, const AttributeValue& o);

typedef std::map<std::string, AttributeValue> AttributesMap;

} // namespace citygml
