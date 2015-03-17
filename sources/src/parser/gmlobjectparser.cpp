#include "parser/gmlobjectparser.h"

#include <citygml/object.h>

namespace citygml {

    GMLObjectElementParser::GMLObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger)
        : CityGMLElementParser(documentParser, factory, logger)
    {

    }

    bool GMLObjectElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (getObject() == nullptr) {
            throw std::runtime_error("Invalid call to GMLObjectElementParser::parseChildElementStartTag");
        }

        if (   node == NodeType::GML_DescriptionNode
            || node == NodeType::GML_IdentifierNode
            || node == NodeType::GML_NameNode
            || node == NodeType::GML_DescriptionReferenceNode
            || node == NodeType::GML_MetaDataPropertyNode) {

            return true;
        }

        return false;
    }

    bool GMLObjectElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (getObject() == nullptr) {
            throw std::runtime_error("Invalid call to GMLObjectElementParser::parseChildElementEndTag");
        }

        if (   node == NodeType::GML_DescriptionNode
            || node == NodeType::GML_IdentifierNode
            || node == NodeType::GML_NameNode
            || node == NodeType::GML_DescriptionReferenceNode
            || node == NodeType::GML_MetaDataPropertyNode) {

                getObject()->setAttribute(node.name(), characters);
                return true;
        }

        return false;
    }

}
