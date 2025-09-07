#pragma once

#include <functional>
#include "parser/gmlfeaturecollectionparser.h"
#include <citygml/rectifiedgridcoverage.h>


namespace citygml {

    class RectifiedGridCoverageParser : public GMLFeatureCollectionElementParser {
    public:
        RectifiedGridCoverageParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(RectifiedGridCoverage *)> callback);
        
        // ElementParser interface
        std::string elementParserName() const override;
        bool handlesElement(const NodeType::XMLNode &node) const override;
    protected:
        
        // CityGMLElementParser interface
        bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        
        // GMLFeatureCollectionElementParser interface
        FeatureObject* getFeatureObject() override;
        
    private:
        std::function<void(RectifiedGridCoverage*)> m_callback;
        RectifiedGridCoverage * m_model;
    };
}


