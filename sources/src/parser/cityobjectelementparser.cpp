#include "parser/cityobjectelementparser.h"

#include <unordered_map>
#include <unordered_set>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/appearanceelementparser.h"
#include "parser/geometryelementparser.h"
#include "parser/implicitgeometryelementparser.h"

#include "citygml/cityobject.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"

#include <stdexcept>

namespace citygml {

#define HANDLE_TYPE( prefix, elementName ) { NodeType::prefix ## _ ## elementName ## Node.typeID(), CityObject::COT_ ## elementName }

    // The nodes that are valid CityObjects
    const std::unordered_map<int, CityObject::CityObjectsType> typeIDTypeMap = {
        HANDLE_TYPE( GEN, GenericCityObject ),
        HANDLE_TYPE( BLDG, Building ),
        HANDLE_TYPE( BLDG, BuildingPart ),
        HANDLE_TYPE( BLDG, Room ),
        HANDLE_TYPE( BLDG, BuildingInstallation ),
        HANDLE_TYPE( BLDG, BuildingFurniture ),
        HANDLE_TYPE( BLDG, Door ),
        HANDLE_TYPE( BLDG, Window ),
        HANDLE_TYPE( BLDG, CityFurniture ),
        HANDLE_TYPE( FRN, CityFurniture ),
        HANDLE_TYPE( TRANS, Track ),
        HANDLE_TYPE( TRANS, Road ),
        HANDLE_TYPE( TRANS, Railway ),
        HANDLE_TYPE( TRANS, Square ),
        HANDLE_TYPE( VEG, PlantCover ),
        HANDLE_TYPE( VEG, SolitaryVegetationObject ),
        HANDLE_TYPE( WTR, WaterBody ),
        HANDLE_TYPE( LUSE, TINRelief ),
        HANDLE_TYPE( LUSE, LandUse ),
        HANDLE_TYPE( SUB, Tunnel ),
        HANDLE_TYPE( BRID, Bridge ),
        HANDLE_TYPE( BRID, BridgeConstructionElement ),
        HANDLE_TYPE( BRID, BridgeInstallation ),
        HANDLE_TYPE( BRID, BridgePart ),
        HANDLE_TYPE( BLDG, WallSurface ),
        HANDLE_TYPE( BLDG, RoofSurface ),
        HANDLE_TYPE( BLDG, GroundSurface ),
        HANDLE_TYPE( BLDG, ClosureSurface ),
        HANDLE_TYPE( BLDG, FloorSurface ),
        HANDLE_TYPE( BLDG, InteriorWallSurface ),
        HANDLE_TYPE( BLDG, CeilingSurface )
    };

    CityObjectElementParser::CityObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void (CityObject*)> callback)
        : GMLFeatureCollectionElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
    }

    std::string CityObjectElementParser::elementParserName() const
    {
        return "CityObjectElementParser";
    }

    bool CityObjectElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return typeIDTypeMap.count(node.typeID()) > 0;
    }

    bool CityObjectElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        auto it = typeIDTypeMap.find(node.typeID());

        if (it == typeIDTypeMap.end()) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag of CityObject but got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createCityObject(attributes.getCityGMLIDAttribute(), it->second);
        return true;

    }

    bool CityObjectElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        m_callback(m_model);
        m_model = nullptr;
        return true;
    }

#define HANDLE_ATTR(prefix, elementName) NodeType::prefix ## _ ## elementName ## Node.typeID()
    // The nodes that contain string content which is set as attribute of the CityObject
    const std::unordered_set<int> attributesSet = {
        HANDLE_ATTR(CORE, CreationDate),
        HANDLE_ATTR(CORE, TerminationDate),
        HANDLE_ATTR(BLDG, Type),
        HANDLE_ATTR(BLDG, Class),
        HANDLE_ATTR(BLDG, Function),
        HANDLE_ATTR(BLDG, Usage),
        HANDLE_ATTR(BLDG, YearOfConstruction),
        HANDLE_ATTR(BLDG, YearOfDemolition),
        HANDLE_ATTR(BLDG, StoreyHeightsAboveGround),
        HANDLE_ATTR(BLDG, StoreysBelowGround),
        HANDLE_ATTR(BLDG, MeasuredHeight),
        HANDLE_ATTR(BLDG, Address),
        HANDLE_ATTR(BLDG, RoofType),
        HANDLE_ATTR(XAL, XalAddress),
        HANDLE_ATTR(XAL, Administrativearea),
        HANDLE_ATTR(XAL, Country),
        HANDLE_ATTR(XAL, CountryName),
        HANDLE_ATTR(XAL, Code),
        HANDLE_ATTR(XAL, Street),
        HANDLE_ATTR(XAL, PostalCode),
        HANDLE_ATTR(XAL, City),
        HANDLE_ATTR(XAL, LocalityName),
        HANDLE_ATTR(XAL, Thoroughfare),
        HANDLE_ATTR(XAL, ThoroughfareNumber),
        HANDLE_ATTR(XAL, ThoroughfareName),
        HANDLE_ATTR(XAL, Locality),
        HANDLE_ATTR(XAL, AddressDetails),
        HANDLE_ATTR(XAL, DependentLocalityName)
    };

    bool CityObjectElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("CityObjectElementParser::parseChildElementStartTag called before CityObjectElementParser::parseElementStartTag");
        }

        if (    node == NodeType::GEN_StringAttributeNode
             || node == NodeType::GEN_DoubleAttributeNode
             || node == NodeType::GEN_IntAttributeNode
             || node == NodeType::GEN_DateAttributeNode
             || node == NodeType::GEN_UriAttributeNode) {

            m_lastAttributeName = attributes.getAttribute("name");

        } else if (attributesSet.count(node.typeID()) > 0 || node == NodeType::GEN_ValueNode) {

            return true;
        } else if (node == NodeType::BLDG_BoundedByNode
                   || node == NodeType::BLDG_OuterBuildingInstallationNode
                   || node == NodeType::BLDG_InteriorBuildingInstallationNode
                   || node == NodeType::BLDG_InteriorFurnitureNode
                   || node == NodeType::BLDG_RoomInstallationNode
                   || node == NodeType::BLDG_InteriorRoomNode
                   || node == NodeType::BLDG_OpeningNode
                   || node == NodeType::BLDG_ConsistsOfBuildingPartNode) {
            setParserForNextElement(new CityObjectElementParser(m_documentParser, m_factory, m_logger, [this](CityObject* obj) {
                                        m_model->addChildCityObject(obj);
                                    }));
        } else if (node == NodeType::APP_AppearanceNode // Compatibility with CityGML 1.0 (in CityGML 2 CityObjects can only contain appearanceMember elements)
                   || node == NodeType::APP_AppearanceMemberNode) {

            setParserForNextElement(new AppearanceElementParser(m_documentParser, m_factory, m_logger));
        } else if (node == NodeType::BLDG_Lod1GeometryNode
                   || node == NodeType::BLDG_Lod1MultiCurveNode
                   || node == NodeType::BLDG_Lod1MultiSurfaceNode
                   || node == NodeType::BLDG_Lod1SolidNode
                   || node == NodeType::BLDG_Lod1TerrainIntersectionNode
                   || node == NodeType::GEN_Lod1GeometryNode
                   || node == NodeType::GEN_Lod1TerrainIntersectionNode
                   || node == NodeType::FRN_Lod1GeometryNode
                   || node == NodeType::FRN_Lod1TerrainIntersectionNode) {

            parseGeometryForLODLevel(1);
        } else if (node == NodeType::BLDG_Lod2GeometryNode
                   || node == NodeType::BLDG_Lod2MultiCurveNode
                   || node == NodeType::BLDG_Lod2MultiSurfaceNode
                   || node == NodeType::BLDG_Lod2SolidNode
                   || node == NodeType::BLDG_Lod2TerrainIntersectionNode
                   || node == NodeType::GEN_Lod2GeometryNode
                   || node == NodeType::GEN_Lod2TerrainIntersectionNode
                   || node == NodeType::FRN_Lod2GeometryNode
                   || node == NodeType::FRN_Lod2TerrainIntersectionNode) {

            parseGeometryForLODLevel(2);
        } else if (node == NodeType::BLDG_Lod3GeometryNode
                   || node == NodeType::BLDG_Lod3MultiCurveNode
                   || node == NodeType::BLDG_Lod3MultiSurfaceNode
                   || node == NodeType::BLDG_Lod3SolidNode
                   || node == NodeType::BLDG_Lod3TerrainIntersectionNode
                   || node == NodeType::GEN_Lod3GeometryNode
                   || node == NodeType::GEN_Lod3TerrainIntersectionNode
                   || node == NodeType::FRN_Lod3GeometryNode
                   || node == NodeType::FRN_Lod3TerrainIntersectionNode) {

            parseGeometryForLODLevel(3);
        } else if (node == NodeType::BLDG_Lod4GeometryNode
                   || node == NodeType::BLDG_Lod4MultiCurveNode
                   || node == NodeType::BLDG_Lod4MultiSurfaceNode
                   || node == NodeType::BLDG_Lod4SolidNode
                   || node == NodeType::BLDG_Lod4TerrainIntersectionNode
                   || node == NodeType::GEN_Lod4GeometryNode
                   || node == NodeType::GEN_Lod4TerrainIntersectionNode
                   || node == NodeType::FRN_Lod4GeometryNode
                   || node == NodeType::FRN_Lod4TerrainIntersectionNode) {

            parseGeometryForLODLevel(4);
        } else if (node == NodeType::VEG_Lod1ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod1ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(1);
        } else if (node == NodeType::VEG_Lod2ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod2ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(2);
        } else if (node == NodeType::VEG_Lod3ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod3ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(3);
        } else if (node == NodeType::VEG_Lod4ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod4ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(4);
        } else if (node == NodeType::BLDG_ExternalReferenceNode
                   || node == NodeType::BLDG_InformationSystemNode
                   || node == NodeType::BLDG_ExternalObjectNode
                   || node == NodeType::BLDG_UriNode) {
            return true;
        } else {
            return GMLFeatureCollectionElementParser::parseChildElementStartTag(node, attributes);
        }

        return true;

    }

    bool CityObjectElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("CityObjectElementParser::parseChildElementEndTag called before CityObjectElementParser::parseElementStartTag");
        }

        if (    node == NodeType::GEN_StringAttributeNode
             || node == NodeType::GEN_DoubleAttributeNode
             || node == NodeType::GEN_IntAttributeNode
             || node == NodeType::GEN_DateAttributeNode
             || node == NodeType::GEN_UriAttributeNode) {

            m_lastAttributeName = "";
            return true;

        } else if (node == NodeType::GEN_ValueNode) {

            if (!m_lastAttributeName.empty()) {
                m_model->setAttribute(m_lastAttributeName, characters);
            } else {
                CITYGML_LOG_WARN(m_logger, "Found value node (" << NodeType::GEN_ValueNode << ") outside attribute node... ignore.");
            }

            return true;
        } else if (attributesSet.count(node.typeID()) > 0) {
            if (!characters.empty()) {
                m_model->setAttribute(node.name(), characters);
            }
            return true;
        } else if (    node == NodeType::BLDG_BoundedByNode
                    || node == NodeType::BLDG_OuterBuildingInstallationNode
                    || node == NodeType::BLDG_InteriorBuildingInstallationNode
                    || node == NodeType::BLDG_InteriorFurnitureNode
                    || node == NodeType::BLDG_RoomInstallationNode
                    || node == NodeType::BLDG_InteriorRoomNode
                    || node == NodeType::BLDG_OpeningNode
                    || node == NodeType::APP_AppearanceNode
                    || node == NodeType::APP_AppearanceMemberNode
                    || node == NodeType::BLDG_Lod1GeometryNode
                    || node == NodeType::BLDG_Lod1MultiCurveNode
                    || node == NodeType::BLDG_Lod1MultiSurfaceNode
                    || node == NodeType::BLDG_Lod1SolidNode
                    || node == NodeType::BLDG_Lod1TerrainIntersectionNode
                    || node == NodeType::BLDG_Lod2GeometryNode
                    || node == NodeType::BLDG_Lod2MultiCurveNode
                    || node == NodeType::BLDG_Lod2MultiSurfaceNode
                    || node == NodeType::BLDG_Lod2SolidNode
                    || node == NodeType::BLDG_Lod2TerrainIntersectionNode
                    || node == NodeType::BLDG_Lod3GeometryNode
                    || node == NodeType::BLDG_Lod3MultiCurveNode
                    || node == NodeType::BLDG_Lod3MultiSurfaceNode
                    || node == NodeType::BLDG_Lod3SolidNode
                    || node == NodeType::BLDG_Lod3TerrainIntersectionNode
                    || node == NodeType::BLDG_Lod4GeometryNode
                    || node == NodeType::BLDG_Lod4MultiCurveNode
                    || node == NodeType::BLDG_Lod4MultiSurfaceNode
                    || node == NodeType::BLDG_Lod4SolidNode
                    || node == NodeType::BLDG_Lod4TerrainIntersectionNode
                    || node == NodeType::GEN_Lod1GeometryNode
                    || node == NodeType::GEN_Lod2GeometryNode
                    || node == NodeType::GEN_Lod3GeometryNode
                    || node == NodeType::GEN_Lod4GeometryNode
                    || node == NodeType::GEN_Lod1TerrainIntersectionNode
                    || node == NodeType::GEN_Lod2TerrainIntersectionNode
                    || node == NodeType::GEN_Lod3TerrainIntersectionNode
                    || node == NodeType::GEN_Lod4TerrainIntersectionNode
                    || node == NodeType::VEG_Lod1ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod2ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod3ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod4ImplicitRepresentationNode
                    || node == NodeType::BLDG_ExternalReferenceNode
                    || node == NodeType::BLDG_InformationSystemNode
                    || node == NodeType::BLDG_ExternalObjectNode
                    || node == NodeType::BLDG_UriNode
                    || node == NodeType::BLDG_ConsistsOfBuildingPartNode
                    || node == NodeType::FRN_Lod1GeometryNode
                    || node == NodeType::FRN_Lod1TerrainIntersectionNode
                    || node == NodeType::FRN_Lod1ImplicitRepresentationNode
                    || node == NodeType::FRN_Lod2GeometryNode
                    || node == NodeType::FRN_Lod2TerrainIntersectionNode
                    || node == NodeType::FRN_Lod2ImplicitRepresentationNode
                    || node == NodeType::FRN_Lod3GeometryNode
                    || node == NodeType::FRN_Lod3TerrainIntersectionNode
                    || node == NodeType::FRN_Lod3ImplicitRepresentationNode
                    || node == NodeType::FRN_Lod4GeometryNode
                    || node == NodeType::FRN_Lod4TerrainIntersectionNode
                    || node == NodeType::FRN_Lod4ImplicitRepresentationNode) {

            return true;
        }

        return GMLFeatureCollectionElementParser::parseChildElementEndTag(node, characters);

    }

    FeatureObject* CityObjectElementParser::getFeatureObject()
    {
        return m_model;
    }

    void CityObjectElementParser::parseGeometryForLODLevel(int lod)
    {
        setParserForNextElement(new GeometryElementParser(m_documentParser, m_factory, m_logger, lod, m_model->getType(), [this](Geometry* geom) {
            m_model->addGeometry(geom);
                                }));
    }

    void CityObjectElementParser::parseImplicitGeometryForLODLevel(int lod)
    {
        setParserForNextElement(new ImplicitGeometryElementParser(m_documentParser, m_factory, m_logger, lod, m_model->getType(), [this](ImplicitGeometry* imp) {
            m_model->addImplictGeometry(imp);
        }));
    }

}
