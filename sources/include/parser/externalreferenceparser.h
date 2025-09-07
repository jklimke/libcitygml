#pragma once

#include <functional>
#include "parser/gmlobjectparser.h"
#include <citygml/externalreference.h>


namespace citygml {
    class ExternalReferenceParser: public GMLObjectElementParser {
    public:
        ExternalReferenceParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(ExternalReference *)> callback);
        
        // ElementParser interface
        std::string elementParserName() const override;
        bool handlesElement(NodeType::XMLNode const& node) const override;
        
    protected:
        // CityGMLElementParser interaface
        bool parseElementStartTag(NodeType::XMLNode const& node, Attributes & attribute) override;
        bool parseElementEndTag(NodeType::XMLNode const& node, std::string const& characters) override;
        bool parseChildElementStartTag(NodeType::XMLNode const& node, Attributes & attributes) override;
        bool parseChildElementEndTag(NodeType::XMLNode const& node, std::string const& characters) override;
        
        // GMLObjectElementParser interface
        Object* getObject() override;
        
    private:
        std::unique_ptr<ExternalReference> model;
        std::function<void(ExternalReference *)> callback;
    };
}
