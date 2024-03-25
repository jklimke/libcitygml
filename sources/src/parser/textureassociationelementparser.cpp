#include "parser/textureassociationelementparser.h"

#include <unordered_map>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/parserutils.hpp"
#include "parser/skipelementparser.h"

#include <citygml/cityobject.h>
#include <citygml/citygmlfactory.h>
#include <citygml/citygmllogger.h>
#include <citygml/texture.h>
#include <citygml/texturetargetdefinition.h>
#include <citygml/texturecoordinates.h>

#include <stdexcept>

namespace citygml {

    TextureAssociationElementParser::TextureAssociationElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::shared_ptr<Texture> texture)
        : GMLObjectElementParser(documentParser, factory, logger)
    {
        m_model = texture;
    }

    std::string TextureAssociationElementParser::elementParserName() const
    {
        return "TextureAssociationElementParser";
    }

    bool TextureAssociationElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::APP_TextureAssociationNode;
    }

    bool TextureAssociationElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (node != NodeType::APP_TextureAssociationNode) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::APP_TextureAssociationNode.name() << "> got " << node << " at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        return true;
    }

    bool TextureAssociationElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        return true;
    }

    bool TextureAssociationElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("TextureAssociationElementParser::parseChildElementStartTag called before TextureAssociationElementParser::parseElementStartTag");
        }

        if (node == NodeType::APP_TextureParameterizationNode
            || node == NodeType::APP_TexCoordListNode
            || node == NodeType::APP_RingNode) {
            return true;
        } else if (node == NodeType::APP_TargetNode) {
            if (!m_lastTargetDefinitionID.empty()) {
                CITYGML_LOG_WARN(m_logger, "Multiple texture target definitions detected at: " << getDocumentLocation());
            }
            m_lastTargetDefinitionID = attributes.getCityGMLIDAttribute();
            return true;
        } else if (node == NodeType::APP_TextureCoordinatesNode) {
            if (!m_texCoordGmlId.empty()) {
                CITYGML_LOG_WARN(m_logger, "Multiple texture coordinates definitions detected at: " << getDocumentLocation());
            }
            m_texCoordGmlId = attributes.getCityGMLIDAttribute();
            return true;
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool TextureAssociationElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("TextureAssociationElementParser::parseChildElementEndTag called before TextureAssociationElementParser::parseElementStartTag");
        }

        if (node == NodeType::APP_TextureParameterizationNode) {
            // Do nothing (target and texture coords are set in child elements)
        } else if (node == NodeType::APP_TexCoordListNode) {
            std::shared_ptr<TextureCoordinates> texCoords = std::make_shared<TextureCoordinates>(m_texCoordGmlId, m_ringId);
            texCoords->setCoords(m_texCoordData);
            if (m_currentTexTargetDef != nullptr) {
                m_currentTexTargetDef->addTexCoordinates(texCoords);
            } else {
                CITYGML_LOG_WARN(m_logger, "TexCoordList node finished before texture target definition was set at " << getDocumentLocation());
            }

        } else if (node == NodeType::APP_TextureCoordinatesNode) {

            if (m_texCoordData.empty()) {
                m_texCoordData = parseVecList<TVec2f>(characters, m_logger, getDocumentLocation());
            } else {
                CITYGML_LOG_WARN(m_logger, "Unexpected end tag <" << NodeType::APP_TextureCoordinatesNode << " at: " << getDocumentLocation());
            }
        } else if (node == NodeType::APP_TargetNode) {
            if (m_currentTexTargetDef == nullptr) {
                const std::string uri = parseReference(characters, m_logger, getDocumentLocation());
                m_currentTexTargetDef = m_factory.createTextureTargetDefinition(uri, m_model, m_lastTargetDefinitionID);
                m_lastTargetDefinitionID = "";
            } else {
                CITYGML_LOG_WARN(m_logger, "Unexpected end tag <" << NodeType::APP_TargetNode << " at: " << getDocumentLocation());
            }
        } else if (node == NodeType::APP_RingNode) {
            m_ringId = parseReference(characters, m_logger, getDocumentLocation());
        } else {
            return GMLObjectElementParser::parseChildElementEndTag(node, characters);
        }
        return true;
    }

    Object* TextureAssociationElementParser::getObject()
    {
        return m_model.get();
    }
}
