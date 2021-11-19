//
//  externalreference.cpp
//  libcitygml
//
//  Created by 松本青空 on 2021/11/15.
//

#include "citygml/externalreference.h"
#include "citygml/citygmllogger.h"
#include "citygml/citygmlfactory.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/nodetypes.h"

namespace citygml {
ExternalObjectReference::ExternalObjectReference() {
}

ExternalObjectReference::~ExternalObjectReference() {
}

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
    
    model = m_factory.createExternalReference(attributes.getCityGMLIDAttribute());
    
    return true;
}

bool ExternalReference::Parser::parseElementEndTag(NodeType::XMLNode const& node, std::string const&) {
    callback(model.get());
    return true;
}

bool ExternalReference::Parser::parseChildElementStartTag(NodeType::XMLNode const& node, Attributes & attributes) {
    if (model == nullptr) {
        throw std::runtime_error("ExternalReference::Parser::parseChildElementStartTag called before ExternalReference::Parser::parseElementStartTag");
    }
    
    if (node == NodeType::CORE_ExternalObjectNode
        || node == NodeType::CORE_InformationSystemNode
        || node == NodeType::CORE_NameNode
        || node == NodeType::CORE_UriNode) {
        return true;
    }
    
    return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
}

bool ExternalReference::Parser::parseChildElementEndTag(NodeType::XMLNode const& node, std::string const& characters) {
    if (model == nullptr) {
        throw std::runtime_error("ExternalReference::Parser::parseChildElementEndTag called before ExternalReference::Parser::parseElementStartTag");
    }
    
    if (node == NodeType::CORE_ExternalObjectNode) {
        return true;
    } else if (node == NodeType::CORE_InformationSystemNode) {
        model->informationSystem = characters;
    } else if (node == NodeType::CORE_NameNode) {
        model->externalObject.name = characters;
    } else if (node == NodeType::CORE_UriNode) {
        model->externalObject.uri = characters;
    }
    
    return GMLObjectElementParser::parseChildElementEndTag(node, characters);
}

Object * ExternalReference::Parser::getObject() {
    return model.get();
}

ExternalReference::ExternalReference(std::string const& id) : Object(id) {
}
}
