#include "parser/appearanceelementparser.h"

#include <unordered_map>
#include <functional>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/delayedchoiceelementparser.h"
#include "parser/textureelementparser.h"
#include "parser/materialelementparser.h"
#include "parser/georeferencedtextureelementparser.h"

#include "citygml/appearance.h"
#include "citygml/texture.h"
#include "citygml/material.h"
#include "citygml/georeferencedtexture.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"

#include <stdexcept>

namespace citygml {

    AppearanceElementParser::AppearanceElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger)
        : CityGMLElementParser(documentParser, factory, logger)
    {

    }

    std::string AppearanceElementParser::elementParserName() const
    {
        return "AppearanceElementParser";
    }

    bool AppearanceElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::APP_AppearanceNode;
    }

    bool AppearanceElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (node != NodeType::APP_AppearanceNode) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::APP_AppearanceNode.name() << "> got <" << node << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }
        return true;
    }

    bool AppearanceElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        if (m_theme.empty()) {
            CITYGML_LOG_ERROR(m_logger, "Appearance node that ends at " << getDocumentLocation() << " has not theme defined. Using empty theme.");
        }

        if (m_surfaceDataList.empty()) {
            CITYGML_LOG_ERROR(m_logger, "Appearance node that ends at " << getDocumentLocation() << " has no surfaceData elements.");
        }

        // assign the theme to all members
        for (std::shared_ptr<Appearance>& surfaceData : m_surfaceDataList) {
            surfaceData->addToTheme(m_theme);
        }

        return true;
    }

    bool AppearanceElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (node == NodeType::APP_ThemeNode) {
            return true;
        } else if (node == NodeType::APP_SurfaceDataMemberNode) {

            if (attributes.hasXLinkAttribute()) {
                // surfaceDataMemberNode links to an existing surfaceData member

                std::shared_ptr<Appearance> sharedAppearance = m_factory.getAppearanceWithID(attributes.getXLinkValue());
                if (sharedAppearance != nullptr) {
                    m_surfaceDataList.push_back(sharedAppearance);
                } else {
                    CITYGML_LOG_WARN(m_logger, "SurfaceDataMember node with invalid xlink attribute. SurfaceData object with id " << attributes.getXLinkValue() << " does not exist.");
                }
            } else {
                // surfaceDataMemberNode contains a surfaceData object (material, texture or georeferencedtexture)

                std::function<void(std::shared_ptr<Appearance>)> callback = [this] (std::shared_ptr<Appearance> surfacedata) {
                    this->m_surfaceDataList.push_back(surfacedata);
                };

                std::vector<ElementParser*> choices;
                choices.push_back(new MaterialElementParser(m_documentParser, m_factory, m_logger, callback));
                choices.push_back(new TextureElementParser(m_documentParser, m_factory, m_logger, callback));
                choices.push_back(new GeoReferencedTextureElementParser(m_documentParser, m_factory, m_logger, callback));

                setParserForNextElement(new DelayedChoiceElementParser(m_documentParser, m_logger, choices));
            }
            return true;

        }
        return false;
    }

    bool AppearanceElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (node == NodeType::APP_ThemeNode) {
            if (!m_theme.empty()) {
                CITYGML_LOG_WARN(m_logger, "Duplicate definition of theme in appearance node at " << getDocumentLocation() << ". Overwriting last theme '" << m_theme << "' with '" << characters << "'");
            }
            m_theme = characters;
            return true;
        } else if (node == NodeType::APP_SurfaceDataMemberNode) {
            return true;
        }
        return false;
    }




}
