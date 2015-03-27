#pragma once

#include <parser/citygmlelementparser.h>

namespace citygml {

    class Object;

    /**
     * @brief abstract base class for all CityGMLElementParser's that parse citygml elements which inherit from gml:AbstractGMLType
     */
    class GMLObjectElementParser : public CityGMLElementParser {
    public:
        GMLObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);

    protected:
        // CityGMLElementParser interface
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

        /**
         * @brief returns the object in which the parsed information will be stored
         */
        virtual Object* getObject() = 0;

    };

}
