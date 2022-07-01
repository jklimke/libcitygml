#pragma once

#include <parser/citygmlelementparser.h>
#include "citygml/attributesmap.h"
#include <vector>

namespace citygml {

    class Object;

    /**
     * @brief abstract base class for all CityGMLElementParser's that parse citygml elements which inherit from gml:AbstractGMLType
     */
    class GMLObjectElementParser : public CityGMLElementParser {
    public:
        GMLObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);
        const AttributeType detectAttributeType(const std::string& characters);
        void setAdeDataComingFlg(bool flg);
        bool getAdeDataComingFlg();
        bool parseChildElementBothTag(const NodeType::XMLNode& node, const std::string& characters);
        void setSkippedTag(std::string tag_name);

    private:
        std::string m_lastCodeSpace;
        std::shared_ptr<std::vector<AttributesMap>> m_attributeHierarchy;
        bool m_adeDataComingFlg;
        std::shared_ptr<std::vector<std::string>> m_skipped_tag;

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
