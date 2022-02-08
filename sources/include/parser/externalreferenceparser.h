#pragma once

#include <functional>
#include "parser/gmlobjectparser.h"
#include <citygml/externalreference.h>


namespace citygml {
    class ExternalReferenceParser: public GMLObjectElementParser {
    public:
        ExternalReferenceParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(ExternalReference *)> callback);
        
        // ElementParser interface
        virtual std::string elementParserName() const override;
        virtual bool handlesElement(NodeType::XMLNode const& node) const override;
        
    protected:
        // CityGMLElementParser interaface
        virtual bool parseElementStartTag(NodeType::XMLNode const& node, Attributes & attribute) override;
        virtual bool parseElementEndTag(NodeType::XMLNode const& node, std::string const& characters) override;
        virtual bool parseChildElementStartTag(NodeType::XMLNode const& node, Attributes & attributes) override;
        virtual bool parseChildElementEndTag(NodeType::XMLNode const& node, std::string const& characters) override;
        
        // GMLObjectElementParser interface
        virtual Object* getObject() override;
        
    private:
        std::shared_ptr<ExternalReference> model;
        std::function<void(ExternalReference *)> callback;
    };
}
