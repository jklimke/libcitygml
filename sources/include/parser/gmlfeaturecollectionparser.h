#pragma once

#include <parser/gmlobjectparser.h>

namespace citygml {

    class Envelope;
    class FeatureObject;

    /**
     * @brief abstract base class for all CityGMLElementParser's that parse citygml elements which inherit from gml:AbstractFeatureCollectionType
     */
    class GMLFeatureCollectionElementParser : public GMLObjectElementParser {
    public:
        GMLFeatureCollectionElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);

    protected:
        // CityGMLElementParser interface
        bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

        virtual FeatureObject* getFeatureObject() = 0;

        // GMLObjectElementParser interface
        Object* getObject() override;

        const Envelope& getEnvelope() const;
        bool getSourceSRSOverride() const;

    private:
        Envelope* m_bounds;
        bool m_sourceSRSOverride;


    };

}
