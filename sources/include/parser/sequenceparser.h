#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "parser/citygmlelementparser.h"

namespace citygml {

    /**
     * @brief The SequenceParser parses a sequence of elements
     *
     * The sequence parser can be used if an xml element can contain more than one child elements. If a
     * parser finds the start tag of such an element (e.g. <gml:patches>) it can invoke the sequence parser, which
     * in turn calls a predefined parser for each child until the end tag of the element is found.
     */
    class SequenceParser : public ElementParser {
    public:
        /**
         * @brief initializes the parser for an element that can contain multiple children
         * @param documentParser the CityGMLDocumentParser instance
         * @param logger a CityGMLLogger logger instance
         * @param childParserFactory a factory for the parser for the child elements
         * @param parentElement the element that contains the child objects
         */
        SequenceParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger, std::function<ElementParser*()> childParserFactory, const NodeType::XMLNode& parentElement);

        // ElementParser interface
        bool startElement(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool endElement(const NodeType::XMLNode& node, const std::string& characters) override;
        bool handlesElement(const NodeType::XMLNode& node) const override;
        std::string elementParserName() const override;

    private:
        std::function<ElementParser*()> m_childParserFactory;
        mutable std::unique_ptr<ElementParser> m_childParserInstance;
        NodeType::XMLNode m_containerType;

        ElementParser& getChildParserDummyInstance() const;
    };

}
