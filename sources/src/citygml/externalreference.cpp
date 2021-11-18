//
//  externalreference.cpp
//  libcitygml
//
//  Created by 松本青空 on 2021/11/15.
//

#include "citygml/externalreference.h"
#include "citygml/citygmllogger.h"
#include "parser/documentlocation.h"
#include "parser/nodetypes.h"

namespace citygml {
ExternalReference::Parser::Parser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(ExternalReference *)> callback)
    : GMLObjectElementParser(documentParser, factory, logger) {
        this->callback = callback;
}

std::string ExternalReference::Parser::elementParserName() const {
    return "ExternalReference::Parser";
}

bool ExternalReference::Parser::handlesElement(NodeType::XMLNode const& node) const {
    return node.typeID() == NodeType::CORE_ExternalReferenceNode.typeID();
}

bool ExternalReference::Parser::parseElementStartTag(NodeType::XMLNode const& node, Attributes & attributes) {
    if (!handlesElement(node)) {
        CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::CORE_ExternalReferenceNode << "> but got <" << node.name() << "> at " << getDocumentLocation());
    }
    
    model = new ExternalReference();
}
}

