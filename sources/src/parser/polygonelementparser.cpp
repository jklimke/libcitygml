#include "parser/polygonelementparser.h"

#include <unordered_map>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/linearringelementparser.h"

#include <citygml/polygon.h>
#include <citygml/citygmlfactory.h>
#include <citygml/citygmllogger.h>

#include <mutex>
#include <stdexcept>

namespace citygml {


    // The nodes that are valid Polygon Objects
    std::unordered_set<int> typeIDSet;
    bool typeIDSetInitialized = false;
    std::mutex polygonElementParser_initializedTypeIDMutex;

    PolygonElementParser::PolygonElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<Polygon>)> callback)
        : GMLObjectElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
    }

    std::string PolygonElementParser::elementParserName() const
    {
        return "PolygonElementParser";
    }

    bool PolygonElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        if (!typeIDSetInitialized) {
            std::lock_guard<std::mutex> lock(polygonElementParser_initializedTypeIDMutex);

            if(!typeIDSetInitialized) {
                typeIDSet.insert(NodeType::GML_TriangleNode.typeID());
                typeIDSet.insert(NodeType::GML_RectangleNode.typeID());
                typeIDSet.insert(NodeType::GML_PolygonNode.typeID());
                typeIDSet.insert(NodeType::GML_PolygonPatchNode.typeID());
                typeIDSetInitialized = true;
            }
        }

        return typeIDSet.count(node.typeID()) > 0;
    }

    bool PolygonElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag of PolygonObject but got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createPolygon(attributes.getCityGMLIDAttribute());
        return true;

    }

    bool PolygonElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        m_callback(m_model);
        return true;
    }



    bool PolygonElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("PolygonElementParser::parseChildElementStartTag called before PolygonElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_InteriorNode) {

            parseRingElement(true);
            return true;
        } else if (node == NodeType::GML_ExteriorNode) {

            parseRingElement(false);
            return true;
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool PolygonElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("PolygonElementParser::parseChildElementEndTag called before PolygonElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_InteriorNode || node == NodeType::GML_ExteriorNode) {

            return true;
        }

        return GMLObjectElementParser::parseChildElementEndTag(node, characters);

    }

    Object* PolygonElementParser::getObject()
    {
        return m_model.get();
    }

    void PolygonElementParser::parseRingElement(bool interior)
    {
        setParserForNextElement(new LinearRingElementParser(m_documentParser, m_factory, m_logger, interior, [this](LinearRing* ring){
            m_model->addRing(ring);
        }));
    }

}
