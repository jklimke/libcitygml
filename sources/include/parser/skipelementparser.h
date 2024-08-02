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
        /**
         * @brief initializes the SkipElementParser
         * @param skipNode if a valid node is passed the skip parser is bound to that node and skips all its children.
         *                 In that case the start tag of the node must already been parsed.
         *                 If the node is not valid (Default) the skip parser will be bound to the first element it encounters.
         */
        SkipElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger, const NodeType::XMLNode& skipNode = NodeType::XMLNode());

        // ElementParser interface
        std::string elementParserName() const override;
        bool handlesElement(const NodeType::XMLNode &node) const override;
        bool startElement(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool endElement(const NodeType::XMLNode& node, const std::string& characters)  override;

    private:
        NodeType::XMLNode m_skipNode;
        int m_depth;

    };

}
