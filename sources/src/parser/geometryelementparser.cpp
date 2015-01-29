#include "parser/geometryelementparser.h"

#include <unordered_set>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/polygonelementparser.h"
#include "parser/delayedchoiceelementparser.h"

#include "citygml/geometry.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"
#include "citygml/polygon.h"

#include <stdexcept>

namespace citygml {


    // The nodes that are valid Geometry Objects
    std::unordered_set<int> geometryTypeIDSet;
    bool geometryTypeIDSetInitialized = false;

    GeometryElementParser::GeometryElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger,
                                                 int lodLevel, CityObject::CityObjectsType parentType,  std::function<void(Geometry*)> callback)
        : CityGMLElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
        m_lodLevel = lodLevel;
        m_parentType = parentType;
    }

    std::string GeometryElementParser::elementParserName() const
    {
        return "GeometryElementParser";
    }

    bool GeometryElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        if(!geometryTypeIDSetInitialized) {
            geometryTypeIDSet.insert(NodeType::GML_CompositeSolidNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_SolidNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_MultiSurfaceNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_CompositeSurfaceNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_TriangulatedSurfaceNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_OrientableSurfaceNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_MultiSolidNode.typeID());
            geometryTypeIDSet.insert(NodeType::GML_CompositeSolidNode.typeID());
            geometryTypeIDSetInitialized = true;
        }

        return geometryTypeIDSet.count(node.typeID()) > 0;
    }

    Geometry::GeometryType mapCityObjectsTypeToGeometryType(const CityObject::CityObjectsType& cityObjType) {

        switch (cityObjType) {
        case CityObject::CityObjectsType::COT_RoofSurface:
            return Geometry::GeometryType::GT_Roof;
        case CityObject::CityObjectsType::COT_WallSurface:
            return Geometry::GeometryType::GT_Wall;
        case CityObject::CityObjectsType::COT_GroundSurface:
            return Geometry::GeometryType::GT_Ground;
        case CityObject::CityObjectsType::COT_ClosureSurface:
            return Geometry::GeometryType::GT_Closure;
        case CityObject::CityObjectsType::COT_FloorSurface:
            return Geometry::GeometryType::GT_Floor;
        case CityObject::CityObjectsType::COT_InteriorWallSurface:
            return Geometry::GeometryType::GT_InteriorWall;
        case CityObject::CityObjectsType::COT_CeilingSurface:
            return Geometry::GeometryType::GT_Ceiling;
        default:
            return Geometry::GeometryType::GT_Unknown;
        }

    }

    bool GeometryElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag of GeometryObject but got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createGeometry(attributes.getCityGMLIDAttribute(), mapCityObjectsTypeToGeometryType(m_parentType), m_lodLevel);
        m_orientation = attributes.getAttribute("orientation", "+"); // A gml:OrientableSurface may define a negative orientation
        return true;

    }

    bool GeometryElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        if (m_orientation == "-") {
            for (int i = 0; i < m_model->getPolygonsCount(); i++) {
                m_model->getPolygon(i).setNegNormal(true);
            }
        }

        m_callback(m_model);
        return true;
    }



    bool GeometryElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("GeometryElementParser::parseChildElementStartTag called before GeometryElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_InteriorNode
         || node == NodeType::GML_ExteriorNode
         || node == NodeType::GML_SolidMemberNode) {

            setParserForNextElement(new GeometryElementParser(m_documentParser, m_factory, m_logger, m_lodLevel, m_parentType, [this](Geometry* child) {
                                        m_model->addGeometry(child);
                                    }));
            return true;

        } else if (node == NodeType::GML_SurfaceMemberNode
                   || node == NodeType::GML_BaseSurfaceNode
                   || node == NodeType::GML_PatchesNode
                   || node == NodeType::GML_TrianglePatchesNode) {

            if (attributes.hasXLinkAttribute()) {
                m_factory.requestSharedPolygonForGeometry(m_model, attributes.getXLinkValue());
            } else {
                std::vector<ElementParser*> parsers;

                std::function<void(std::shared_ptr<Polygon>)> callback1 = [this](std::shared_ptr<Polygon> poly) {m_model->addPolygon(poly);};
                std::function<void(Geometry*)>                callback2 = [this](Geometry* child) {m_model->addGeometry(child);};

                parsers.push_back(new PolygonElementParser(m_documentParser, m_factory, m_logger, callback1));
                parsers.push_back(new GeometryElementParser(m_documentParser, m_factory, m_logger, m_lodLevel, m_parentType, callback2));

                setParserForNextElement(new DelayedChoiceElementParser(m_documentParser, m_logger, parsers));
            }
            return true;
        }

        return false;
    }

    bool GeometryElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("GeometryElementParser::parseChildElementEndTag called before GeometryElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_InteriorNode
         || node == NodeType::GML_ExteriorNode
         || node == NodeType::GML_SolidMemberNode
         || node == NodeType::GML_SurfaceMemberNode
         || node == NodeType::GML_BaseSurfaceNode
         || node == NodeType::GML_PatchesNode
         || node == NodeType::GML_TrianglePatchesNode)  {
            return true;
        }

        return false;

    }

}
