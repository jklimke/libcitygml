#include "parser/sequenceparser.h"

#include "citygml/citygmllogger.h"
#include "parser/documentlocation.h"
#include "parser/citygmldocumentparser.h"

namespace citygml {

    SequenceParser::SequenceParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger, std::function<ElementParser*()> childParserFactory, const NodeType::XMLNode& parentElement)
     : ElementParser(documentParser, logger)
    {
        m_childParserFactory = childParserFactory;
        m_containerType = parentElement;
        m_childParserInstance = nullptr;
    }

    bool SequenceParser::startElement(const NodeType::XMLNode& node, Attributes& attributes)
    {
        // Note that at this point the start tag of the parent element has already been parsed... hence node must be a child element
        ElementParser* childParser = m_childParserFactory();
        setParserForNextElement(childParser);
        return childParser->startElement(node, attributes);
    }

    bool SequenceParser::endElement(const NodeType::XMLNode& node, const std::string&)
    {
        if (node != m_containerType) {
            CITYGML_LOG_ERROR(m_logger, "Sequence parser was bound to container element <" << m_containerType << "> but found unexpected"
                                       " end tag <" << node << "> at " << getDocumentLocation() << ". Ignoring tag...");
            return false;

        } else {
            m_documentParser.removeCurrentElementParser(this);
        }

        return true;
    }

    bool SequenceParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return getChildParserDummyInstance().handlesElement(node);
    }

    std::string SequenceParser::elementParserName() const
    {
        return "SequenceParser (" + getChildParserDummyInstance().elementParserName() + ")";
    }

    ElementParser& SequenceParser::getChildParserDummyInstance() const
    {
        // Lazy initialize the dummy object (its only used for logging and error reporting)
        if (m_childParserInstance == nullptr) {
            m_childParserInstance = std::unique_ptr<ElementParser>(m_childParserFactory());
        }
        return *m_childParserInstance;
    }

}
