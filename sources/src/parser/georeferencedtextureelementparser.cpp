#include "parser/georeferencedtextureelementparser.h"

#include <unordered_map>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"

#include "citygml/georeferencedtexture.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"

#include <stdexcept>

namespace citygml {

    GeoReferencedTextureElementParser::GeoReferencedTextureElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger,
                                                                         std::function<void (std::shared_ptr<GeoreferencedTexture>)> callback)
        : CityGMLElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
    }

    std::string GeoReferencedTextureElementParser::elementParserName() const
    {
        return "GeoReferencedTextureElementParser";
    }

    bool GeoReferencedTextureElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::APP_GeoreferencedTextureNode;
    }

    bool GeoReferencedTextureElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (node != NodeType::APP_GeoreferencedTextureNode) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::APP_GeoreferencedTextureNode.name() << "> got " << node << " at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }
        return true;
    }

    bool GeoReferencedTextureElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        // Not Implemented
        return false;
    }

    bool GeoReferencedTextureElementParser::parseChildElementStartTag(const NodeType::XMLNode&, Attributes&)
    {
        // Not Implemented
        return false;
    }

    bool GeoReferencedTextureElementParser::parseChildElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        // Not Implemented
        return false;
    }




}
