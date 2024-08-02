#pragma once

#include <parser/gmlobjectparser.h>
#include <citygml/cityobject.h>

#include <string>
#include <functional>

namespace citygml {

    class Geometry;

    class GeometryElementParser : public GMLObjectElementParser {
    public:
        GeometryElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger,
                              int lodLevel, CityObject::CityObjectsType parentType, std::function<void(Geometry*)> callback);

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
        Geometry* m_model;
        std::function<void(Geometry*)> m_callback;
        int m_lodLevel;
        CityObject::CityObjectsType m_parentType;
        std::string m_orientation;
    };

}
