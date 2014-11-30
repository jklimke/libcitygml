#include "parser/nodetypes.h"

namespace citygml {

    int NodeType::typeCount = -1;
    std::unordered_map<std::string, NodeType::XMLNode*> NodeType::nodeNameTypeMap;
    std::unordered_map<std::string, NodeType::XMLNode*> NodeType::nodeNameWithPrefixTypeMap;

    NodeType::XMLNode::XMLNode(std::string name, std::string prefix): m_name(name), m_prefix(prefix)
    {
        NodeType::nodeNameTypeMap[name] = this;
        NodeType::nodeNameWithPrefixTypeMap[prefix + ":" + name] = this;
        m_typeID = NodeType::typeCount++;
    }

    const std::string&NodeType::XMLNode::name() const
    {
        return m_name;
    }

    const std::string&NodeType::XMLNode::prefix() const
    {
        return m_prefix;
    }

    int NodeType::XMLNode::typeID() const
    {
        return m_typeID;
    }

    bool NodeType::XMLNode::operator==(const NodeType::XMLNode& other) const
    {
        return typeID() == other.typeID();
    }

    bool NodeType::XMLNode::operator!=(const NodeType::XMLNode& other) const
    {
        return !(*this == other);
    }

    bool NodeType::XMLNode::valid() const
    {
        return !m_name.empty();
    }

    const NodeType::XMLNode&NodeType::getXMLNodeFor(const std::string& name)
    {
        {
            auto it = nodeNameWithPrefixTypeMap.find(name);
            if (it != nodeNameWithPrefixTypeMap.end()) {
                return *it->second;
            }
        }

        std::string prefix = "";
        std::string nodeName = name;

        size_t pos = name.find_first_of( ":" );
        if ( pos != std::string::npos ) {
            prefix = name.substr( 0, pos );
            nodeName = name.substr(pos);
        }

        auto it = nodeNameTypeMap.find(name);

        if (it == nodeNameTypeMap.end()) {
            return InvalidNode();
        } else {
            return *it->second;
        }
    }

    std::ostream& operator<<(std::ostream& os, const NodeType::XMLNode& o)
    {
        if (!o.valid()) {
            os << "InvalidNode";
        } else {
            if (!o.prefix().empty()) {
                os << o.prefix() << ":";
            }
            os << o.name();
        }

        return os;
    }

}
