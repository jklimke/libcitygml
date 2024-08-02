#pragma once

#include <parser/gmlobjectparser.h>

#include <functional>

namespace citygml {

    class LinearRing;

    class LinearRingElementParser : public GMLObjectElementParser {
    public:
        LinearRingElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, bool interior, std::function<void(LinearRing*)> callback);

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
        LinearRing* m_model;
        std::function<void(LinearRing*)> m_callback;
        bool m_interior;
    };

}
