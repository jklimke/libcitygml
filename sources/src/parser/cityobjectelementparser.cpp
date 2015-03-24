#include "parser/cityobjectelementparser.h"
#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/appearanceelementparser.h"
#include "parser/geometryelementparser.h"
#include "parser/implicitgeometryelementparser.h"
#include "parser/skipelementparser.h"

#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"

#include <stdexcept>

namespace citygml {

    bool CityObjectElementParser::typeIDTypeMapInitialized = false;
    bool CityObjectElementParser::attributesSetInitialized = false;

    std::unordered_map<int, CityObject::CityObjectsType> CityObjectElementParser::typeIDTypeMap = std::unordered_map<int, CityObject::CityObjectsType>();
    std::unordered_set<int> CityObjectElementParser::attributesSet = std::unordered_set<int>();

    std::mutex CityObjectElementParser::initializedTypeIDMutex;
    std::mutex CityObjectElementParser::initializedAttributeSetMutex;

    #define HANDLE_TYPE( prefix, elementName ) std::pair<int, CityObject::CityObjectsType>(NodeType::prefix ## _ ## elementName ## Node.typeID(), CityObject::COT_ ## elementName)
    #define HANDLE_GROUP_TYPE( prefix, elementName, enumtype ) std::pair<int, CityObject::CityObjectsType>(NodeType::prefix ## _ ## elementName ## Node.typeID(), enumtype)
    #define HANDLE_ATTR( prefix, elementName ) NodeType::prefix ## _ ## elementName ## Node.typeID()

    CityObjectElementParser::CityObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void (CityObject*)> callback)
        : GMLFeatureCollectionElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
    }

    std::string CityObjectElementParser::elementParserName() const
    {
        return "CityObjectElementParser";
    }

    void CityObjectElementParser::initializeTypeIDTypeMap()
    {
        // double-checked lock
        if (!typeIDTypeMapInitialized) {
            std::lock_guard<std::mutex> lock(CityObjectElementParser::initializedTypeIDMutex);

            if (!typeIDTypeMapInitialized) {
                typeIDTypeMap.insert(HANDLE_TYPE(GEN, GenericCityObject));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Building));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingPart));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Room));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingInstallation));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingFurniture));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Door));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Window));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, CityFurniture));
                typeIDTypeMap.insert(HANDLE_TYPE(FRN, CityFurniture));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Track));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Road));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Railway));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Square));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(TRANS, TransportationComplex, CityObject::CityObjectsType::COT_TransportationObject));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(TRANS, TrafficArea, CityObject::CityObjectsType::COT_TransportationObject));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(TRANS, AuxiliaryTrafficArea, CityObject::CityObjectsType::COT_TransportationObject));
                typeIDTypeMap.insert(HANDLE_TYPE(VEG, PlantCover));
                typeIDTypeMap.insert(HANDLE_TYPE(VEG, SolitaryVegetationObject));
                typeIDTypeMap.insert(HANDLE_TYPE(WTR, WaterBody));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(WTR, WaterSurface, CityObject::COT_WaterBody));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(WTR, WaterGroundSurface, CityObject::COT_WaterBody));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(WTR, WaterClosureSurface, CityObject::COT_WaterBody));
                typeIDTypeMap.insert(HANDLE_TYPE(LUSE, LandUse));
                typeIDTypeMap.insert(HANDLE_TYPE(SUB, Tunnel));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, Bridge));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, BridgeConstructionElement));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, BridgeInstallation));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, BridgePart));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, WallSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, RoofSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, GroundSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, ClosureSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, FloorSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, InteriorWallSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, CeilingSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(GRP, CityObjectGroup));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, ReliefFeature));

                typeIDTypeMapInitialized = true;
            }
        }
    }

    void CityObjectElementParser::initializeAttributesSet()
    {
        // double-checked lock
        if (!attributesSetInitialized) {
            std::lock_guard<std::mutex> lock(CityObjectElementParser::initializedAttributeSetMutex);

            if (!attributesSetInitialized) {
                attributesSet.insert(HANDLE_ATTR(CORE, CreationDate));
                attributesSet.insert(HANDLE_ATTR(CORE, TerminationDate));
                attributesSet.insert(HANDLE_ATTR(BLDG, Type));
                attributesSet.insert(HANDLE_ATTR(BLDG, Class));
                attributesSet.insert(HANDLE_ATTR(BLDG, Function));
                attributesSet.insert(HANDLE_ATTR(BLDG, Usage));
                attributesSet.insert(HANDLE_ATTR(BLDG, YearOfConstruction));
                attributesSet.insert(HANDLE_ATTR(BLDG, YearOfDemolition));
                attributesSet.insert(HANDLE_ATTR(BLDG, StoreyHeightsAboveGround));
                attributesSet.insert(HANDLE_ATTR(BLDG, StoreyHeightsBelowGround));
                attributesSet.insert(HANDLE_ATTR(BLDG, StoreysBelowGround));
                attributesSet.insert(HANDLE_ATTR(BLDG, StoreysAboveGround));
                attributesSet.insert(HANDLE_ATTR(BLDG, MeasuredHeight));
                attributesSet.insert(HANDLE_ATTR(BLDG, RoofType));
                attributesSet.insert(HANDLE_ATTR(CORE, Address));
                attributesSet.insert(HANDLE_ATTR(BLDG, Address));
                attributesSet.insert(HANDLE_ATTR(XAL, XalAddress));
                attributesSet.insert(HANDLE_ATTR(XAL, Administrativearea));
                attributesSet.insert(HANDLE_ATTR(XAL, Country));
                attributesSet.insert(HANDLE_ATTR(XAL, CountryName));
                attributesSet.insert(HANDLE_ATTR(XAL, Code));
                attributesSet.insert(HANDLE_ATTR(XAL, Street));
                attributesSet.insert(HANDLE_ATTR(XAL, PostalCode));
                attributesSet.insert(HANDLE_ATTR(XAL, City));
                attributesSet.insert(HANDLE_ATTR(XAL, LocalityName));
                attributesSet.insert(HANDLE_ATTR(XAL, Thoroughfare));
                attributesSet.insert(HANDLE_ATTR(XAL, ThoroughfareNumber));
                attributesSet.insert(HANDLE_ATTR(XAL, ThoroughfareName));
                attributesSet.insert(HANDLE_ATTR(XAL, Locality));
                attributesSet.insert(HANDLE_ATTR(XAL, AddressDetails));
                attributesSet.insert(HANDLE_ATTR(XAL, DependentLocalityName));
                attributesSet.insert(HANDLE_ATTR(VEG, Class ));
                attributesSet.insert(HANDLE_ATTR(VEG, Function ));
                attributesSet.insert(HANDLE_ATTR(VEG, AverageHeight ));
                attributesSet.insert(HANDLE_ATTR(VEG, Species ));
                attributesSet.insert(HANDLE_ATTR(VEG, Height ));
                attributesSet.insert(HANDLE_ATTR(VEG, TrunkDiameter ));
                attributesSet.insert(HANDLE_ATTR(VEG, CrownDiameter ));
                attributesSet.insert(HANDLE_ATTR(FRN, Class));
                attributesSet.insert(HANDLE_ATTR(FRN, Function));
                attributesSet.insert(HANDLE_ATTR(GRP, Class));
                attributesSet.insert(HANDLE_ATTR(GRP, Function));
                attributesSet.insert(HANDLE_ATTR(GRP, Usage));
                attributesSet.insert(HANDLE_ATTR(GEN, Class));
                attributesSet.insert(HANDLE_ATTR(GEN, Function));
                attributesSet.insert(HANDLE_ATTR(GEN, Usage));
                attributesSet.insert(HANDLE_ATTR(LUSE, Class));
                attributesSet.insert(HANDLE_ATTR(LUSE, Function));
                attributesSet.insert(HANDLE_ATTR(LUSE, Usage));
                attributesSet.insert(HANDLE_ATTR(DEM, Lod));
                attributesSet.insert(HANDLE_ATTR(TRANS, Usage));
                attributesSet.insert(HANDLE_ATTR(TRANS, Function));
                attributesSet.insert(HANDLE_ATTR(TRANS, SurfaceMaterial));
                attributesSet.insert(HANDLE_ATTR(WTR, Class));
                attributesSet.insert(HANDLE_ATTR(WTR, Function));
                attributesSet.insert(HANDLE_ATTR(WTR, Usage));
                attributesSet.insert(HANDLE_ATTR(WTR, WaterLevel));

                attributesSetInitialized = true;
            }
        }
    }

    bool CityObjectElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        initializeTypeIDTypeMap();

        return typeIDTypeMap.count(node.typeID()) > 0;
    }

    bool CityObjectElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        initializeTypeIDTypeMap();

        auto it = typeIDTypeMap.find(node.typeID());

        if (it == typeIDTypeMap.end()) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag of CityObject but got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createCityObject(attributes.getCityGMLIDAttribute(), static_cast<CityObject::CityObjectsType>(it->second));
        return true;

    }

    bool CityObjectElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        m_callback(m_model);
        m_model = nullptr;
        return true;
    }

    bool CityObjectElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        initializeAttributesSet();

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
                   || node == NodeType::BLDG_ConsistsOfBuildingPartNode
                   || node == NodeType::GRP_GroupMemberNode
                   || node == NodeType::GRP_ParentNode
                   || node == NodeType::TRANS_TrafficAreaNode
                   || node == NodeType::TRANS_AuxiliaryTrafficAreaNode
                   || node == NodeType::WTR_BoundedByNode) {
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
                   || node == NodeType::FRN_Lod1TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod1MultiSurfaceNode
                   || node == NodeType::TRANS_Lod1MultiSurfaceNode
                   || node == NodeType::WTR_Lod1MultiCurveNode
                   || node == NodeType::WTR_Lod1MultiSurfaceNode
                   || node == NodeType::WTR_Lod1SolidNode) {

            parseGeometryForLODLevel(1);
        } else if (node == NodeType::BLDG_Lod2GeometryNode
                   || node == NodeType::BLDG_Lod2MultiCurveNode
                   || node == NodeType::BLDG_Lod2MultiSurfaceNode
                   || node == NodeType::BLDG_Lod2SolidNode
                   || node == NodeType::BLDG_Lod2TerrainIntersectionNode
                   || node == NodeType::GEN_Lod2GeometryNode
                   || node == NodeType::GEN_Lod2TerrainIntersectionNode
                   || node == NodeType::FRN_Lod2GeometryNode
                   || node == NodeType::FRN_Lod2TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod2MultiSurfaceNode
                   || node == NodeType::TRANS_Lod2MultiSurfaceNode
                   || node == NodeType::WTR_Lod2SolidNode
                   || node == NodeType::WTR_Lod2SurfaceNode) {

            parseGeometryForLODLevel(2);
        } else if (node == NodeType::BLDG_Lod3GeometryNode
                   || node == NodeType::BLDG_Lod3MultiCurveNode
                   || node == NodeType::BLDG_Lod3MultiSurfaceNode
                   || node == NodeType::BLDG_Lod3SolidNode
                   || node == NodeType::BLDG_Lod3TerrainIntersectionNode
                   || node == NodeType::GEN_Lod3GeometryNode
                   || node == NodeType::GEN_Lod3TerrainIntersectionNode
                   || node == NodeType::FRN_Lod3GeometryNode
                   || node == NodeType::FRN_Lod3TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod3MultiSurfaceNode
                   || node == NodeType::TRANS_Lod3MultiSurfaceNode
                   || node == NodeType::WTR_Lod3SolidNode
                   || node == NodeType::WTR_Lod3SurfaceNode) {

            parseGeometryForLODLevel(3);
        } else if (node == NodeType::BLDG_Lod4GeometryNode
                   || node == NodeType::BLDG_Lod4MultiCurveNode
                   || node == NodeType::BLDG_Lod4MultiSurfaceNode
                   || node == NodeType::BLDG_Lod4SolidNode
                   || node == NodeType::BLDG_Lod4TerrainIntersectionNode
                   || node == NodeType::GEN_Lod4GeometryNode
                   || node == NodeType::GEN_Lod4TerrainIntersectionNode
                   || node == NodeType::FRN_Lod4GeometryNode
                   || node == NodeType::FRN_Lod4TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod4MultiSurfaceNode
                   || node == NodeType::TRANS_Lod4MultiSurfaceNode
                   || node == NodeType::WTR_Lod4SolidNode
                   || node == NodeType::WTR_Lod4SurfaceNode) {

            parseGeometryForLODLevel(4);
        } else if (node == NodeType::VEG_Lod1ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod1ImplicitRepresentationNode
                   || node == NodeType::GEN_Lod1ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(1);
        } else if (node == NodeType::VEG_Lod2ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod2ImplicitRepresentationNode
                   || node == NodeType::GEN_Lod2ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(2);
        } else if (node == NodeType::VEG_Lod3ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod3ImplicitRepresentationNode
                   || node == NodeType::GEN_Lod3ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(3);
        } else if (node == NodeType::VEG_Lod4ImplicitRepresentationNode
                   || node == NodeType::FRN_Lod4ImplicitRepresentationNode
                   || node == NodeType::GEN_Lod4ImplicitRepresentationNode) {

            parseImplicitGeometryForLODLevel(4);
        } else if (node == NodeType::CORE_GeneralizesToNode
                   || node == NodeType::CORE_ExternalReferenceNode
                   || node == NodeType::GML_MultiPointNode
                   || node == NodeType::GRP_GeometryNode
                   || node == NodeType::DEM_ReliefComponentNode
                   || node == NodeType::GEN_Lod0GeometryNode
                   || node == NodeType::GEN_Lod0ImplicitRepresentationNode
                   || node == NodeType::GEN_Lod0TerrainIntersectionNode
                   || node == NodeType::TRANS_Lod0NetworkNode
                   || node == NodeType::WTR_Lod0MultiCurveNode
                   || node == NodeType::WTR_Lod0MultiSurfaceNode) {
            CITYGML_LOG_INFO(m_logger, "Skipping CityObject child element <" << node  << ">  at " << getDocumentLocation() << " (Currently not supported!)");
            setParserForNextElement(new SkipElementParser(m_documentParser, m_logger));
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

        initializeAttributesSet();

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
                    || node == NodeType::GEN_Lod1ImplicitRepresentationNode
                    || node == NodeType::GEN_Lod2ImplicitRepresentationNode
                    || node == NodeType::GEN_Lod3ImplicitRepresentationNode
                    || node == NodeType::GEN_Lod4ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod1ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod2ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod3ImplicitRepresentationNode
                    || node == NodeType::VEG_Lod4ImplicitRepresentationNode
                    || node == NodeType::CORE_ExternalReferenceNode
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
                    || node == NodeType::FRN_Lod4ImplicitRepresentationNode
                    || node == NodeType::CORE_GeneralizesToNode
                    || node == NodeType::GML_MultiPointNode
                    || node == NodeType::GRP_GroupMemberNode
                    || node == NodeType::GRP_ParentNode
                    || node == NodeType::LUSE_Lod1MultiSurfaceNode
                    || node == NodeType::LUSE_Lod2MultiSurfaceNode
                    || node == NodeType::LUSE_Lod3MultiSurfaceNode
                    || node == NodeType::LUSE_Lod4MultiSurfaceNode
                    || node == NodeType::DEM_ReliefComponentNode
                    || node == NodeType::GEN_Lod0GeometryNode
                    || node == NodeType::GEN_Lod0ImplicitRepresentationNode
                    || node == NodeType::GEN_Lod0TerrainIntersectionNode
                    || node == NodeType::TRANS_Lod0NetworkNode
                    || node == NodeType::TRANS_TrafficAreaNode
                    || node == NodeType::TRANS_AuxiliaryTrafficAreaNode
                    || node == NodeType::TRANS_Lod1MultiSurfaceNode
                    || node == NodeType::TRANS_Lod2MultiSurfaceNode
                    || node == NodeType::TRANS_Lod3MultiSurfaceNode
                    || node == NodeType::TRANS_Lod4MultiSurfaceNode
                    || node == NodeType::WTR_Lod0MultiCurveNode
                    || node == NodeType::WTR_Lod0MultiSurfaceNode
                    || node == NodeType::WTR_Lod1MultiCurveNode
                    || node == NodeType::WTR_Lod1MultiSurfaceNode
                    || node == NodeType::WTR_Lod1SolidNode
                    || node == NodeType::WTR_Lod2SolidNode
                    || node == NodeType::WTR_Lod3SolidNode
                    || node == NodeType::WTR_Lod4SolidNode
                    || node == NodeType::WTR_Lod2SurfaceNode
                    || node == NodeType::WTR_Lod3SurfaceNode
                    || node == NodeType::WTR_Lod4SurfaceNode
                    || node == NodeType::WTR_BoundedByNode) {

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
