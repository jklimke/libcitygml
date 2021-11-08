//
//  rectifiedgridcoverage.hpp
//  citygml
//
//  Created by 松本青空 on 2021/10/29.
//

#pragma once

#include <parser/gmlfeaturecollectionparser.h>

#include <vector>
#include <memory>

#include <citygml/featureobject.h>
#include <citygml/citygml_api.h>

namespace citygml {

    class LIBCITYGML_EXPORT RectifiedGridCoverage : public FeatureObject
    {
        friend class CityGMLFactory;
        
    protected:
        RectifiedGridCoverage(std::string const& id = "RectifiedGridCoverage");
        
        class Parser : public GMLFeatureCollectionElementParser
        {
        public:
            Parser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(RectifiedGridCoverage*)> callback);
            
            // ElementParser interface
            virtual std::string elementParserName() const override;
            virtual bool handlesElement(const NodeType::XMLNode &node) const override;
        protected:

            // CityGMLElementParser interface
            virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
            virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
            virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
            virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

            // GMLFeatureCollectionElementParser interface
            virtual FeatureObject* getFeatureObject() override;
            
        private:
            std::function<void(RectifiedGridCoverage*)> m_callback;
            RectifiedGridCoverage* m_model;
        };
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream& os, const RectifiedGridCoverage& o );

}

