#include "parser/elementparser.h"

#include "parser/citygmldocumentparser.h"

namespace citygml {

    void ElementParser::setParserForNextElement(ElementParser* parser)
    {
        m_documentParser.setCurrentElementParser(parser);
    }

    const DocumentLocation& ElementParser::getDocumentLocation() const
    {
        return m_documentParser.getDocumentLocation();
    }

    ElementParser::~ElementParser()
    {

    }
}
