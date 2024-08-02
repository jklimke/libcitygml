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
        std::string elementParserName() const override;
        bool handlesElement(const NodeType::XMLNode &node) const override;
    protected:
        // CityGMLElementParser interface
        bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

        // GMLObjectElementParser interface
        Object* getObject() override;

    private:
        ImplicitGeometry* m_model;
        std::function<void(ImplicitGeometry*)> m_callback;
        int m_lodLevel;
        CityObject::CityObjectsType m_parentType;
        std::string m_orientation;
    };

}
