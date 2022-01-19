//
//  rectifiedgridcoverageparser.cpp
//  citygml
//
//  Created by 松本青空 on 2022/01/18.
//

#include "parser/rectifiedgridcoverageparser.h"

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/cityobjectelementparser.h"
#include "parser/appearanceelementparser.h"

#include <citygml/citymodel.h>
#include <citygml/citygmllogger.h>
#include <citygml/citygmlfactory.h>
#include <citygml/cityobject.h>

#include <stdexcept>

namespace citygml {
    RectifiedGridCoverageParser::RectifiedGridCoverageParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(RectifiedGridCoverage *)> callback)
        : GMLFeatureCollectionElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
        m_model = nullptr;
    }

    bool RectifiedGridCoverageParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::GML_RectifiedGridCoverageNode;
    }

    std::string RectifiedGridCoverageParser::elementParserName() const
    {
        return "RectifiedGridCoverageParser";
    }

    bool RectifiedGridCoverageParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (node != NodeType::GML_RectifiedGridCoverageNode) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::CORE_CityModelNode.name() << "> got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createRectifiedGridCoverage(attributes.getCityGMLIDAttribute());
        return true;
    }

    bool RectifiedGridCoverageParser::parseElementEndTag(const NodeType::XMLNode& node, const std::string&)
    {
        if (node != NodeType::GML_RectifiedGridCoverageNode) {
            CITYGML_LOG_WARN(m_logger, "Expected end tag <" << NodeType::CORE_CityModelNode.name() << "> got <" << node.name() << "> at " << getDocumentLocation());
        }

        if (getSourceSRSOverride()) {
            Envelope *envelope = new Envelope(getEnvelope().srsName());
            envelope->setLowerBound(m_model->getEnvelope().getLowerBound());
            envelope->setUpperBound(m_model->getEnvelope().getUpperBound());
            m_model->setEnvelope(envelope);
        }
        m_callback(m_model);
        return true;
    }


    bool RectifiedGridCoverageParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("RectifiedGridCoverage::parseChildElementStartTag called before RectifiedGridCoverage::parseElementStartTag");
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool RectifiedGridCoverageParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("RectifiedGridCoverage::parseChildElementEndTag called before RectifiedGridCoverage::parseElementStartTag");
        }

        return GMLObjectElementParser::parseChildElementEndTag(node, characters);
    }

    FeatureObject* RectifiedGridCoverageParser::getFeatureObject()
    {
        return m_model;
    }

}
