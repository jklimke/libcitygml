#pragma once

#include <string>
#include <map>

#include <citygml/citygml_api.h>

namespace citygml
{
class AttributeValue;

typedef std::map<std::string, AttributeValue> AttributesMap;

/**
 * @brief The AttributeType enum represents the data type of an object attribute
 */
enum class AttributeType
{
    String,
    Double,
    Integer,
    Date,
    Uri,
    Measure,
    AttributeSet,
    Boolean
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
    AttributeValue(const AttributesMap& value);


    void setType(AttributeType type);
    AttributeType getType() const;

    void setValue(const std::string& value, AttributeType type=AttributeType::String);
    void setValue(double value);
    void setValue(int value);
    void setValue(const AttributesMap& value);

    const std::string & asString() const;
    double asDouble(double defaultValue=0.0) const;
    int asInteger(int defaultValue=0) const;
    AttributesMap& asAttributeSet();
    [[nodiscard]] const AttributesMap& asAttributeSet() const;

    // Returns formatted string of AttributesMap, includes recursive children.
    static std::string attributesMapToString(const AttributesMap& attributesMap);

private:
    AttributeType m_type;
    std::string m_value;
    // Avoid using string field because of parse latency issue.
    AttributesMap m_attribute_set;

    static std::string attributesMapToStringRecursive(const AttributesMap &attributesMap, int depth);
    static std::string indent(int num);
};

LIBCITYGML_EXPORT std::ostream& operator<<(std::ostream& os, const AttributeValue& o);

} // namespace citygml
