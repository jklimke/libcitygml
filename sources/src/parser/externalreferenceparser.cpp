#include "parser/externalreferenceparser.h"

#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/nodetypes.h"
#include <citygml/citygmllogger.h>
#include <citygml/citygmlfactory.h>

namespace citygml {
    ExternalReferenceParser::ExternalReferenceParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(ExternalReference *)> callback)
        : GMLObjectElementParser(documentParser, factory, logger) {
            this->callback = callback;
    }

    std::string ExternalReferenceParser::elementParserName() const {
        return "ExternalReferenceParser";
    }

    bool ExternalReferenceParser::handlesElement(NodeType::XMLNode const& node) const {
        return node.typeID() == NodeType::CORE_ExternalReferenceNode.typeID();
    }

    bool ExternalReferenceParser::parseElementStartTag(NodeType::XMLNode const& node, Attributes & attributes) {
        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::CORE_ExternalReferenceNode << "> but got <" << node.name() << "> at " << getDocumentLocation());
        }
        
        model = m_factory.createExternalReference(attributes.getCityGMLIDAttribute());
        
        return true;
    }

    bool ExternalReferenceParser::parseElementEndTag(NodeType::XMLNode const& node, std::string const&) {
        callback(model.get());
        return true;
    }

    bool ExternalReferenceParser::parseChildElementStartTag(NodeType::XMLNode const& node, Attributes & attributes) {
        if (model == nullptr) {
            throw std::runtime_error("ExternalReferenceParser::parseChildElementStartTag called before ExternalReferenceParser::parseElementStartTag");
        }
        
        if (node == NodeType::CORE_ExternalObjectNode
            || node == NodeType::CORE_InformationSystemNode
            || node == NodeType::CORE_NameNode
            || node == NodeType::CORE_UriNode) {
            return true;
        }
        
        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool ExternalReferenceParser::parseChildElementEndTag(NodeType::XMLNode const& node, std::string const& characters) {
        if (model == nullptr) {
            throw std::runtime_error("ExternalReferenceParser::parseChildElementEndTag called before ExternalReferenceParser::parseElementStartTag");
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

    Object * ExternalReferenceParser::getObject() {
        return model.get();
    }
}
