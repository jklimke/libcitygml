#include <parser/skipelementparser.h>
#include <parser/citygmldocumentparser.h>
#include <parser/documentlocation.h>

#include <citygml/citygmllogger.h>

namespace citygml {

    SkipElementParser::SkipElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger)
        : ElementParser(documentParser, logger)
    {

    }

    std::string SkipElementParser::elementParserName() const
    {
        return "SkipElementParser";
    }

    bool SkipElementParser::handlesElement(const NodeType::XMLNode&) const
    {
        return true;
    }

    bool SkipElementParser::startElement(const NodeType::XMLNode& node, Attributes&)
    {
        if (!m_skipNode.valid()) {
            CITYGML_LOG_TRACE(m_logger, "Skipping element <" << node << "> at " << getDocumentLocation());
            m_skipNode = node;
            m_depth = 1;
        } else if (node == m_skipNode) {
            m_depth++;
        }

        return true;
    }

    bool SkipElementParser::endElement(const NodeType::XMLNode& node, const std::string&)
    {
        if (!m_skipNode.valid()) {
            m_documentParser.removeCurrentElementParser(this);
        }

        if (node == m_skipNode) {
            m_depth--;

            if (m_depth == 0) {
                m_documentParser.removeCurrentElementParser(this);
            }
        }

        return true;
    }

}
