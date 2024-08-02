#pragma once

#include <parser/gmlobjectparser.h>

#include <functional>
#include <memory>
#include <string>

namespace citygml {

    class Texture;
    class TextureTargetDefinition;
    class TextureCoordinates;

    class TextureElementParser : public GMLObjectElementParser {
    public:
        TextureElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<Texture>)> callback);

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
        std::shared_ptr<Texture> m_model;
        std::function<void(std::shared_ptr<Texture>)> m_callback;
        std::shared_ptr<TextureTargetDefinition> m_currentTexTargetDef;
        std::shared_ptr<TextureCoordinates> m_currentTexCoords;
    };

}
