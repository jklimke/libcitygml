#pragma once

#include <functional>
#include "parser/gmlfeaturecollectionparser.h"
#include <citygml/rectifiedgridcoverage.h>


namespace citygml {

    class RectifiedGridCoverageParser : public GMLFeatureCollectionElementParser {
    public:
        RectifiedGridCoverageParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(RectifiedGridCoverage *)> callback);
        
        // ElementParser interface
        virtual std::string elementParserName() const override;
        virtual bool handlesElement(const NodeType::XMLNode &node) const override;
    protected:
        
        // CityGMLElementParser interface
        virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        
        // GMLFeatureCollectionElementParser interface
        virtual FeatureObject* getFeatureObject() override;
        
    private:
        std::function<void(RectifiedGridCoverage*)> m_callback;
        RectifiedGridCoverage * m_model;
    };
}


