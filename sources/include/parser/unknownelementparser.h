#pragma once

#include <vector>
#include <memory>

#include "parser/elementparser.h"

namespace citygml {

    /**
     * @brief The UnknownElementParser allows to parse xml elements of which the concrete type is not known in advance
     *
     * The UnknownElementParser is initialized with a list of possible parses. When the start element of the next node is parsed it chooses the
     * first parser that can handle the element.
     */
    class UnknownElementParser : public ElementParser {
    public:

        /**
         * @brief creates a UnknownElementParser
         * @param documentParser
         * @param logger
         * @param choices the parsers to choose from. The UnknownElementParser takes ownership of the parsers.
         */
        UnknownElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger, std::vector<ElementParser*> choices);

        // ElementParser interface
        virtual bool startElement(const NodeType::XMLNode& node, Attributes& attributes);
        virtual bool endElement(const NodeType::XMLNode& node, const std::string& characters);
        virtual bool handlesElement(const NodeType::XMLNode& node) const;
        virtual std::string elementParserName() const;

        void setStockNode(const NodeType::XMLNode& node);

    private:
        std::vector<ElementParser*> m_choices;
        NodeType::XMLNode m_stockNode;
    };

}
