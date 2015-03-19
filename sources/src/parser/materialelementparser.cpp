#include "parser/materialelementparser.h"

#include <unordered_map>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/parserutils.hpp"
#include "parser/skipelementparser.h"

#include "citygml/material.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"

#include <stdexcept>

namespace citygml {

    MaterialElementParser::MaterialElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<Material>)> callback)
        : GMLObjectElementParser(documentParser, factory, logger)
    {
        m_model = nullptr;
        m_callback = callback;
    }

    std::string MaterialElementParser::elementParserName() const
    {
        return "MaterialElementParser";
    }

    bool MaterialElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::APP_MaterialNode || node == NodeType::APP_X3DMaterialNode;
    }

    bool MaterialElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::APP_MaterialNode.name() << "> got " << node << " at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = std::shared_ptr<Material>(m_factory.createMaterial(attributes.getCityGMLIDAttribute()));

        return true;
    }

    bool MaterialElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        m_callback(m_model);
        return true;
    }

    bool MaterialElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("MaterialElementParser::parseChildElementStartTag called before MaterialElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_NameNode
            || node == NodeType::APP_DiffuseColorNode
            || node == NodeType::APP_EmissiveColorNode
            || node == NodeType::APP_SpecularColorNode
            || node == NodeType::APP_ShininessNode
            || node == NodeType::APP_TransparencyNode
            || node == NodeType::APP_AmbientIntensityNode
            || node == NodeType::APP_IsFrontNode
            || node == NodeType::APP_isSmoothNode) {
            return true;
        } else if (node == NodeType::APP_TargetNode) {
            m_lastTargetDefinitionID = attributes.getCityGMLIDAttribute();
            return true;
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool MaterialElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("MaterialElementParser::parseChildElementEndTag called before MaterialElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_NameNode) {

            m_model->setAttribute(node.name(), characters);
        } else if (node == NodeType::APP_DiffuseColorNode) {

            m_model->setDiffuse(parseValue<TVec3f>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_EmissiveColorNode) {

            m_model->setEmissive(parseValue<TVec3f>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_SpecularColorNode) {

            m_model->setSpecular(parseValue<TVec3f>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_ShininessNode) {

            m_model->setShininess(parseValue<float>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_TransparencyNode) {

            m_model->setTransparency(parseValue<float>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_AmbientIntensityNode) {

            m_model->setAmbientIntensity(parseValue<float>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_IsFrontNode) {

            m_model->setIsFront(parseValue<bool>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_isSmoothNode) {

            m_model->setIsSmooth(parseValue<bool>(characters, m_logger, getDocumentLocation()));
        } else if (node == NodeType::APP_TargetNode) {

            m_factory.createMaterialTargetDefinition(parseReference(characters, m_logger, getDocumentLocation()), m_model, m_lastTargetDefinitionID);
            m_lastTargetDefinitionID = "";
        }  else {
            return GMLObjectElementParser::parseChildElementEndTag(node, characters);
        }
        return true;
    }

    Object* MaterialElementParser::getObject()
    {
        return m_model.get();
    }


}
