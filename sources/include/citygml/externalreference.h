//
//  ExternalReference.h
//  libcitygml
//
//  Created by 松本青空 on 2021/11/12.
//

#pragma once

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include "parser/gmlobjectparser.h"

namespace citygml {
    union LIBCITYGML_EXPORT ExternalObjectReference {
        std::string name;
        std::string uri;
    };

    class LIBCITYGML_EXPORT ExternalReference: public Object {
    public:
        class Parser: public GMLObjectElementParser {
        public:
            Parser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(ExternalReference *)> callback);
            
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
            std::unique_ptr<ExternalReference> model;
            std::function<void(ExternalReference *)> callback;
        };
        
        ExternalReference() = default;
        ~ExternalReference() noexcept override;                    // Destructor
        
        std::string informationSystem;
        ExternalObjectReference externalObject;
    };
}
