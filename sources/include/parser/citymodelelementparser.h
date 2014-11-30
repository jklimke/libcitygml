#pragma once

#include <parser/gmlfeaturecollectionparser.h>

#include <functional>

namespace citygml {

    class CityModel;

    class CityModelElementParser : public GMLFeatureCollectionElementParser {
    public:
        CityModelElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(CityModel*)> callback);

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
        std::function<void(CityModel*)> m_callback;
        CityModel* m_model;
    };

}
