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

const std::string & AttributeValue::asString() const
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


// AttributesMap to string
namespace{

    std::string indent(int num) {
        std::stringstream ss;
        for(int i=0; i<num; i++){
            ss << "  ";
        }
        return ss.str();
    }

    std::string attributesMapToStringRecursive(const AttributesMap& attributesMap, int depth){
        std::stringstream ss;
        ss << indent(depth++) << u8"[\n";
        for(auto& pair : attributesMap){
            auto& key = pair.first;
            auto& value = pair.second;
            ss << indent(depth) << u8"{ " << key << u8" => ";
            if(value.getType() == AttributeType::AttributeSet){
                ss << u8"\n" << attributesMapToStringRecursive(value.asAttributeSet(), depth+1);
                ss << indent(depth) << u8"}\n";
            }else{
                ss << value.asString() << u8" }\n";
            }
        }
        ss << indent(--depth) << u8"]\n";
        return ss.str();
    }

    std::string attributesMapToString(const AttributesMap &attributesMap) {
        return attributesMapToStringRecursive(attributesMap, 0);
    }
}


std::ostream& operator<<(std::ostream& os, const AttributeValue& o)
{
    if (o.getType() == AttributeType::AttributeSet) {
        os << attributesMapToString(o.asAttributeSet());
        return os;
    }
    os << o.asString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const AttributesMap& o){
    os << attributesMapToString(o);
    return os;
}

} // namespace citygml
