#include "parser/delayedchoiceelementparser.h"

#include "parser/documentlocation.h"
#include "parser/citygmldocumentparser.h"

#include "citygml/citygmllogger.h"

#include <sstream>

namespace citygml {

    DelayedChoiceElementParser::DelayedChoiceElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger, std::vector<ElementParser*> choices)
        : ElementParser(documentParser, logger)
    {
        m_choices = choices;
    }

    bool DelayedChoiceElementParser::startElement(const NodeType::XMLNode& node, Attributes& attributes)
    {
        ElementParser* choosenParser = nullptr;
        for (ElementParser* parser : m_choices) {

            if (choosenParser == nullptr && parser->handlesElement(node)) {
                choosenParser = parser;
            } else {
                delete parser;
            }

        }

        if (choosenParser != nullptr) {
            m_documentParser.removeCurrentElementParser(this);
            m_documentParser.setCurrentElementParser(choosenParser);
            return choosenParser->startElement(node, attributes);
        } else {
            CITYGML_LOG_ERROR(m_logger, "DelayedChoiceElementParser could not find a parser to handle <" << node << "> at " << getDocumentLocation());
            throw std::runtime_error("No parser for XML element found.");
        }
    }

    bool DelayedChoiceElementParser::endElement(const NodeType::XMLNode&, const std::string&)
    {
        throw std::runtime_error("DelayedChoiceElementParser::endElement must never be called.");
    }

    bool DelayedChoiceElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        for (const ElementParser* parser : m_choices) {
            if (parser->handlesElement(node)) {
                return true;
            }
        }
        return false;
    }

    std::string DelayedChoiceElementParser::elementParserName() const
    {
        std::stringstream ss;
        ss << "DelayedChoiceElementParser (";
        for (size_t i = 0; i < m_choices.size(); i++) {

            if (i > 0) {
                ss << " | ";
            }

            ss << m_choices[i]->elementParserName();
        }

        ss << ")";
        return ss.str();
    }

}
