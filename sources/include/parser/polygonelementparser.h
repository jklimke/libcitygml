#pragma once

#include <parser/citygmlelementparser.h>

#include <functional>
#include <memory>

namespace citygml {

    class Polygon;

    class PolygonElementParser : public CityGMLElementParser {
    public:
        PolygonElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<Polygon>)> callback);

        // ElementParser interface
        virtual std::string elementParserName() const;
        virtual bool handlesElement(const NodeType::XMLNode &node) const override;
    protected:
        // CityGMLElementParser interface
        virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

    private:
        std::shared_ptr<Polygon> m_model;
        std::function<void(std::shared_ptr<Polygon>)> m_callback;

        void parseRingElement(bool interior);
    };

}
