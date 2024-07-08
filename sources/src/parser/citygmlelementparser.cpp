#include "parser/citygmlelementparser.h"

#include <citygml/utils.h>
#include <citygml/citygmllogger.h>

#include "parser/citygmldocumentparser.h"
#include "parser/documentlocation.h"
#include "parser/nodetypes.h"

#include <stdexcept>

namespace citygml {

    CityGMLElementParser::CityGMLElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger)
        : ElementParser(documentParser, logger), m_factory(factory), m_boundElement(NodeType::InvalidNode)
    {
    }

    bool CityGMLElementParser::startElement(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (!node.valid()) {
            CITYGML_LOG_ERROR(m_logger, "Invalid xml start tag (no name) found at " << getDocumentLocation());
            throw std::runtime_error("Error parsing xml document. Invalid start tag.");
        }

        if (!m_boundElement.valid()) {
            m_boundElement = node;
            return parseElementStartTag(node, attributes);
        } else {
            return parseChildElementStartTag(node, attributes);
        }
    }

    bool CityGMLElementParser::endElement(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (!m_boundElement.valid()) {
            // This might happen if an container element that usally contains a child element links to an exting object using XLink an thus
            // uses a combined start/end element: e.g.: <surfaceMember xlink:href="#..."/>
            // For such elements a child parser must only be created if there is no xlink attribute.
            // It can also happen in cases when a <cityObjectMember> only contains ignored elements.
            CITYGML_LOG_WARN(m_logger, "CityGMLElementParser::endElement called on unbound " << elementParserName() << " object for element <" << node << "> at " << getDocumentLocation());
            m_documentParser.removeCurrentElementParser(this);
            return true;
        }

        if (m_boundElement == node) {
            m_documentParser.removeCurrentElementParser(this);
            return parseElementEndTag(node, characters);
        } else {
            return parseChildElementEndTag(node, characters);
        }
    }

    CityGMLElementParser::~CityGMLElementParser()
    {

    }

}
