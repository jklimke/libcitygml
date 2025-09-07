#pragma once

#include <parser/gmlobjectparser.h>

#include <string>
#include <vector>
#include <memory>

namespace citygml {

    class CityObject;
    class Appearance;

    class AppearanceElementParser : public GMLObjectElementParser {
    public:
        AppearanceElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);

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
        std::vector<std::shared_ptr<Appearance> > m_surfaceDataList;
        std::shared_ptr<Object> m_appearanceObj;
        std::string m_theme;
    };

}
