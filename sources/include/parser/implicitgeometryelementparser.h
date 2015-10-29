#pragma once

#include <parser/gmlobjectparser.h>
#include <citygml/cityobject.h>

#include <string>
#include <functional>

namespace citygml {

    class ImplicitGeometry;

    class ImplicitGeometryElementParser : public GMLObjectElementParser {
    public:
        ImplicitGeometryElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger,
                              int lodLevel, CityObject::CityObjectsType parentType, std::function<void(ImplicitGeometry*)> callback);

        // ElementParser interface
        virtual std::string elementParserName() const override;
        virtual bool handlesElement(const NodeType::XMLNode &node) const override;
    protected:
        // CityGMLElementParser interface
        virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

        // GMLObjectElementParser interface
        virtual Object* getObject() override;

    private:
        ImplicitGeometry* m_model;
        std::function<void(ImplicitGeometry*)> m_callback;
        int m_lodLevel;
        CityObject::CityObjectsType m_parentType;
        std::string m_orientation;
    };

}
