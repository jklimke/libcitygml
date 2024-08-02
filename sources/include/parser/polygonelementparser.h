#pragma once

#include <parser/gmlobjectparser.h>

#include <functional>
#include <memory>

namespace citygml {

    class Polygon;

    class PolygonElementParser : public GMLObjectElementParser {
    public:
        PolygonElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<Polygon>)> callback);

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
        std::shared_ptr<Polygon> m_model;
        std::function<void(std::shared_ptr<Polygon>)> m_callback;

        void parseRingElement(bool interior);
    };

}
