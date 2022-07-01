#include "parser/gmlobjectparser.h"

#include <citygml/object.h>
#include "parser/attributes.h"
#include <citygml/citygmlfactory.h>
#include "parser/documentlocation.h"
#include <regex>
#include <iostream>

namespace citygml {

    GMLObjectElementParser::GMLObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger)
        : CityGMLElementParser(documentParser, factory, logger)
        ,m_lastCodeSpace("")
        ,m_adeDataComingFlg(false)
        ,m_attributeHierarchy(std::make_shared<std::vector<AttributesMap>>())
        ,m_skipped_tag(std::make_shared<std::vector<std::string>>()) {

    }

    bool GMLObjectElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) {
        if (getObject() == nullptr) {
            throw std::runtime_error("Invalid call to GMLObjectElementParser::parseChildElementStartTag");
        }

        if (node == NodeType::GML_DescriptionNode
            || node == NodeType::GML_IdentifierNode
            || node == NodeType::GML_NameNode
            || node == NodeType::GML_DescriptionReferenceNode
            || node == NodeType::GML_MetaDataPropertyNode) {

            return true;
        }

        if (node != NodeType::InvalidNode) return true;
        
        size_t pos = node.name().find_first_of(":");
        if (pos != std::string::npos) {
            if (isupper(node.name().substr(pos + 1).at(0))) {// Ignore Tag start with capital letter
                m_adeDataComingFlg = false;
                return true;
            }
        }
        AttributesMap attributeSet;
        m_attributeHierarchy->push_back(attributeSet);

        if(!m_adeDataComingFlg) m_lastCodeSpace = attributes.getAttribute("codeSpace");// ignore attributes because it's not correct one when m_adeDataComingFlg is true
        m_adeDataComingFlg = false;

        return true;
    }

    bool GMLObjectElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) {
        if (getObject() == nullptr) {
            throw std::runtime_error("Invalid call to GMLObjectElementParser::parseChildElementEndTag");
        }

        if (node == NodeType::GML_DescriptionNode
            || node == NodeType::GML_IdentifierNode
            || node == NodeType::GML_NameNode
            || node == NodeType::GML_DescriptionReferenceNode
            || node == NodeType::GML_MetaDataPropertyNode) {

            getObject()->setAttribute(node.name(), characters);
            return true;
        }

        if (node != NodeType::InvalidNode) return true;

        size_t pos = node.name().find_first_of(":");
        if (pos != std::string::npos) {
            if (isupper(node.name().substr(pos + 1).at(0))) {// Ignore Tag start with capital letter
                return true;
            }
        }

        if (m_skipped_tag->size() != 0) {
            if (m_skipped_tag->back() == node.name()) {
                m_skipped_tag->pop_back();
                return true;
            }
        }

        auto& attributesMap = m_attributeHierarchy->back();

        if (m_attributeHierarchy->size() == 1) { // no parent tag
            if (attributesMap.size() == 0) { // no child tag
                if (m_lastCodeSpace == "") {
                    getObject()->setAttribute(node.name(), characters, detectAttributeType(characters));
                } else {
                    const auto codeValue = m_factory.getCodeValue(m_lastCodeSpace, getDocumentLocation().getDocumentFilePath(), characters);
                    getObject()->setAttribute(node.name(), codeValue, detectAttributeType(codeValue));
                }
            } else { // have child tag
                getObject()->getAttributes()[node.name()] = attributesMap;
            }
            m_attributeHierarchy->pop_back();
        } else { // have parent tag
            if (attributesMap.size() == 0) { // no child tag
                m_attributeHierarchy->pop_back();
                auto& parent_attributesMap = m_attributeHierarchy->back();
                if (m_lastCodeSpace == "") {
                    parent_attributesMap[node.name()] = AttributeValue(characters, detectAttributeType(characters));
                } else {
                    const auto codeValue = m_factory.getCodeValue(m_lastCodeSpace, getDocumentLocation().getDocumentFilePath(), characters);
                    parent_attributesMap[node.name()] = AttributeValue(codeValue, detectAttributeType(codeValue));
                }
            } else { // have child tag
                auto my_attributesMap = attributesMap;
                m_attributeHierarchy->pop_back();
                auto& parent_attributesMap = m_attributeHierarchy->back();
                parent_attributesMap[node.name()] = my_attributesMap;
            }
        }
        
        return true;
    }

    const AttributeType GMLObjectElementParser::detectAttributeType(const std::string& characters) {
        if (characters == "true" || characters == "false") {
            return AttributeType::Boolean;
        }
        std::regex re_int(R"([0-9]+)");
        if (std::regex_match(characters, re_int)) {
            return AttributeType::Integer;
        }
        std::regex re_double(R"([0-9]+\.[0-9]*)");
        if (std::regex_match(characters, re_double)) {
            return AttributeType::Double;
        }
        std::regex re_date(R"(\d{4}-\d\d-\d\d)");
        if (std::regex_match(characters, re_date)) {
            return AttributeType::Date;
        }
        std::regex re_uri(R"(http.*)");
        if (std::regex_match(characters, re_uri)) {
            return AttributeType::Uri;
        }

        return AttributeType::String;
    }

    void GMLObjectElementParser::setAdeDataComingFlg(bool flg) {
        m_adeDataComingFlg = flg;
    }

    bool GMLObjectElementParser::getAdeDataComingFlg() {
        return m_adeDataComingFlg;
    }

    bool GMLObjectElementParser::parseChildElementBothTag(const NodeType::XMLNode& node, const std::string& characters) { // Only called from UnknownElementParser::endElement

        size_t pos = node.name().find_first_of(":");
        if (pos != std::string::npos) {
            if (isupper(node.name().substr(pos + 1).at(0))) {// Ignore Tag start with capital letter
                return true;
            }
        }

        if (m_attributeHierarchy->size() == 0) { // no parent tag
            getObject()->setAttribute(node.name(), characters, detectAttributeType(characters));
        }
        else { // have parent tag
            auto& parent_attributesMap = m_attributeHierarchy->back();
            parent_attributesMap[node.name()] = AttributeValue(characters, detectAttributeType(characters));
        }
        return true;
    }

    void GMLObjectElementParser::setSkippedTag(std::string tag_name) {
        m_skipped_tag->push_back(tag_name);
    }
}
