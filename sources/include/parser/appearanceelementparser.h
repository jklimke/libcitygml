#pragma once

#include <parser/citygmlelementparser.h>

#include <string>
#include <vector>
#include <memory>

namespace citygml {

    class CityObject;
    class Appearance;

    class AppearanceElementParser : public CityGMLElementParser {
    public:
        AppearanceElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);

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
        std::vector<std::shared_ptr<Appearance>> m_surfaceDataList;
        std::string m_theme;
    };

}
