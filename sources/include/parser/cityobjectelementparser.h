#pragma once

#include <parser/gmlfeaturecollectionparser.h>

#include <functional>

namespace citygml {

    class CityObject;

    class CityObjectElementParser : public GMLFeatureCollectionElementParser {
    public:
        CityObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(CityObject*)> callback);

        // ElementParser interface
        virtual std::string elementParserName() const;
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
        CityObject* m_model;
        std::function<void(CityObject*)> m_callback;
        std::string m_lastAttributeName;

        void parseGeometryForLODLevel(int lod);
        void parseImplicitGeometryForLODLevel(int lod);
    };

}
