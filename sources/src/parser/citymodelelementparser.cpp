#include "parser/citymodelelementparser.h"

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/cityobjectelementparser.h"
#include "parser/appearanceelementparser.h"

#include "citygml/citymodel.h"
#include "citygml/citygmllogger.h"
#include "citygml/citygmlfactory.h"
#include "citygml/cityobject.h"


namespace citygml {

    CityModelElementParser::CityModelElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void (CityModel*)> callback)
        : GMLFeatureCollectionElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
        m_model = nullptr;
    }

    bool CityModelElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::CORE_CityModelNode;
    }

    std::string CityModelElementParser::elementParserName() const
    {
        return "CityModelElementParser";
    }

    bool CityModelElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (node != NodeType::CORE_CityModelNode) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::CORE_CityModelNode.name() << "> got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createCityModel(attributes.getCityGMLIDAttribute());
        return true;
    }

    bool CityModelElementParser::parseElementEndTag(const NodeType::XMLNode& node, const std::string&)
    {
        if (node != NodeType::CORE_CityModelNode) {
            CITYGML_LOG_WARN(m_logger, "Expected end tag <" << NodeType::CORE_CityModelNode.name() << "> got <" << node.name() << "> at " << getDocumentLocation());
        }

        m_callback(m_model);
        return true;
    }

    bool CityModelElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("CityModelElementParser::parseChildElementStartTag called before CityModelElementParser::parseElementStartTag");
        }

        if (node == NodeType::CORE_CityObjectMemberNode) {
            setParserForNextElement(new CityObjectElementParser(m_documentParser, m_factory, m_logger, [this](CityObject* obj) {
                                        this->m_model->addRootObject(obj);
                                    }));
            return true;
        } else if (node == NodeType::APP_AppearanceNode // Compatibility with CityGML 1.0 (in CityGML 3 CityObjects can only contain appearanceMember elements)
                   || node == NodeType::APP_AppearanceMemberNode) {

            setParserForNextElement(new AppearanceElementParser(m_documentParser, m_factory, m_logger));
            return true;
        } else {
            return GMLFeatureCollectionElementParser::parseChildElementStartTag(node, attributes);
        }
        return false;
    }

    bool CityModelElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("CityModelElementParser::parseChildElementEndTag called before CityModelElementParser::parseElementStartTag");
        }

        if (node == NodeType::CORE_CityObjectMemberNode
         || node == NodeType::APP_AppearanceNode
         || node == NodeType::APP_AppearanceMemberNode) {
            return true;
        } else {
            return GMLFeatureCollectionElementParser::parseChildElementEndTag(node, characters);
        }
        return false;
    }

    FeatureObject* CityModelElementParser::getFeatureObject()
    {
        return m_model;
    }

}
