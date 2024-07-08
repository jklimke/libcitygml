#pragma once

#include <parser/gmlobjectparser.h>
#include <citygml/vecs.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace citygml {

    class Texture;
    class TextureTargetDefinition;
    class TextureCoordinates;

    // Parser for CityGML 3.0 TextureAssociation elements
    class TextureAssociationElementParser : public GMLObjectElementParser {
    public:
        TextureAssociationElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::shared_ptr<Texture> texture);

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
        std::shared_ptr<Texture> m_model;
        std::shared_ptr<TextureTargetDefinition> m_currentTexTargetDef;
        std::string m_lastTargetDefinitionID;
        std::string m_ringId;
        std::string m_texCoordGmlId;
        std::vector<TVec2f> m_texCoordData;
    };

}
