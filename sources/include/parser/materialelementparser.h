#pragma once

#include <parser/citygmlelementparser.h>

#include <functional>
#include <memory>
#include <string>

namespace citygml {

    class Material;

    class MaterialElementParser : public CityGMLElementParser {
    public:
        MaterialElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<Material>)> callback);

        bool handlesElement(const NodeType::XMLNode &node) const override;

    protected:
        // CityGMLElementParser interface
        virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

    private:
        std::shared_ptr<Material> m_model;
        std::function<void(std::shared_ptr<Material>)> m_callback;
        std::string m_lastTargetDefinitionID;
    };

}
