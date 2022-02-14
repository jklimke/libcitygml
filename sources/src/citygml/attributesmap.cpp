#include <citygml/attributesmap.h>

#include <sstream>

namespace citygml {

AttributeValue::AttributeValue()
    : m_type(AttributeType::String)
{
}

AttributeValue::AttributeValue(const char* value)
    : m_type(AttributeType::String)
    , m_value(value)
    , m_attribute_set()
{

}

AttributeValue::AttributeValue(const std::string& value, AttributeType type)
    : m_type(type)
    , m_value(value)
    , m_attribute_set()
{

}

AttributeValue::AttributeValue(double value)
{
    setValue(value);
}

AttributeValue::AttributeValue(int value)
{
    setValue(value);
}

AttributeValue::AttributeValue(const AttributesMap& value)
{
    setValue(value);
}

void AttributeValue::setType(AttributeType type)
{
    m_type = type;
}

AttributeType AttributeValue::getType() const
{
    return m_type;
}

void AttributeValue::setValue(const std::string& value, AttributeType type)
{
    m_type = type;
    m_value = value;
}

void AttributeValue::setValue(double value)
{
    m_type = AttributeType::Double;

    std::stringstream sstream;
    sstream << value;
    m_value = sstream.str();
}

void AttributeValue::setValue(int value)
{
    m_type = AttributeType::Integer;

    std::stringstream sstream;
    sstream << value;
    m_value = sstream.str();
}

void AttributeValue::setValue(const AttributesMap& value)
{
    m_type = AttributeType::AttributeSet;

    m_attribute_set = value;
}

std::string AttributeValue::asString() const
{
    return m_value;
}

double AttributeValue::asDouble(double defaultValue) const
{
    double value = defaultValue;

    if (m_type == AttributeType::Double)
    {
        std::stringstream sstream;
        sstream << m_value;
        sstream >> value;
    }

    return value;
}

int AttributeValue::asInteger(int defaultValue) const
{
    int value = defaultValue;

    if (m_type == AttributeType::Integer)
    {
        std::stringstream sstream;
        sstream << m_value;
        sstream >> value;
    }

    return value;
}

AttributesMap& AttributeValue::asAttributeSet() {
    return m_attribute_set;
}

const AttributesMap& AttributeValue::asAttributeSet() const
{
    return m_attribute_set;
}

std::ostream& operator<<(std::ostream& os, const AttributeValue& o)
{
    if (o.getType() == AttributeType::AttributeSet) {
        for (auto entry : o.asAttributeSet()) {
            os << "{" << entry.first << ": " << entry.second << ",";
        }
    }
    os << o.asString();
    return os;
}

} // namespace citygml
