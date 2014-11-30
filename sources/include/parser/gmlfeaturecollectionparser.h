#pragma once

#include <parser/citygmlelementparser.h>

namespace citygml {

    class Envelope;
    class FeatureObject;

    /**
     * @brief abstract base class for all CityGMLElementParser's that parse citygml elements which inherit from gml:AbstractFeatureCollectionType
     */
    class GMLFeatureCollectionElementParser : public CityGMLElementParser {
    public:
        GMLFeatureCollectionElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);

    protected:
        // CityGMLElementParser interface
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

        virtual FeatureObject* getFeatureObject() = 0;

    private:
        Envelope* m_bounds;
    };

}
