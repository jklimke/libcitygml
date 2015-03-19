#pragma once

#include <parser/elementparser.h>

namespace citygml {

    /**
     * @brief A parser that just skips over the element (and all its child elements) for which it was called
     * @note The SkipElementParser can be called for the content of an element that might be empty. In
     *       that case the SkipElementParser will return control to the calling parser after the end element of that element was parsed.
     *       Hence the the calling parser should not expect to parse the end element.
     */
    class SkipElementParser : public ElementParser {
    public:
        SkipElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger);

        // ElementParser interface
        virtual std::string elementParserName() const override;
        virtual bool handlesElement(const NodeType::XMLNode &node) const override;
        virtual bool startElement(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool endElement(const NodeType::XMLNode& node, const std::string& characters)  override;

    private:
        NodeType::XMLNode m_skipNode;
        int m_depth;

    };

}
