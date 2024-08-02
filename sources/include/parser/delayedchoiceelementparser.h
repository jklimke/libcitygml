#pragma once

#include <vector>
#include <memory>

#include "parser/elementparser.h"

namespace citygml {

    /**
     * @brief The DelayedChoiceElementParser allows to parse xml elements of which the concrete type is not known in advance
     *
     * The DelayedChoiceElementParser is initialized with a list of possible parses. When the start element of the next node is parsed it chooses the
     * first parser that can handle the element.
     */
    class DelayedChoiceElementParser : public ElementParser {
    public:

        /**
         * @brief creates a DelayedChoiceElementParser
         * @param documentParser
         * @param logger
         * @param choices the parsers to choose from. The DelayedChoiceElementParser takes ownership of the parsers.
         */
        DelayedChoiceElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger, std::vector<ElementParser*> choices);

        // ElementParser interface
        bool startElement(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool endElement(const NodeType::XMLNode& node, const std::string& characters) override;
        bool handlesElement(const NodeType::XMLNode& node) const override;
        std::string elementParserName() const override;

    private:
        std::vector<ElementParser*> m_choices;
    };

}
