#include "parser/cityobjectelementparser.h"
#include "parser/citygmldocumentparser.h"
#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/appearanceelementparser.h"
#include "parser/geometryelementparser.h"
#include "parser/implicitgeometryelementparser.h"
#include "parser/polygonelementparser.h"
#include "parser/skipelementparser.h"
#include "parser/delayedchoiceelementparser.h"
#include "parser/linestringelementparser.h"
#include "parser/addressparser.h"
#include "parser/rectifiedgridcoverageparser.h"
#include "parser/externalreferenceparser.h"

#include <citygml/citygmlfactory.h>
#include <citygml/citygmllogger.h>
#include <citygml/address.h>

#include <stdexcept>
#include <iostream>

namespace citygml {

    bool CityObjectElementParser::typeIDTypeMapInitialized = false;
    bool CityObjectElementParser::attributesSetInitialized = false;

    std::unordered_map<int, CityObject::CityObjectsType> CityObjectElementParser::typeIDTypeMap = std::unordered_map<int, CityObject::CityObjectsType>();
    std::unordered_set<int> CityObjectElementParser::attributesSet = std::unordered_set<int>();
    std::unordered_map<int, AttributeType> CityObjectElementParser::attributeTypeMap;

    std::mutex CityObjectElementParser::initializedTypeIDMutex;
    std::mutex CityObjectElementParser::initializedAttributeSetMutex;

    #define HANDLE_TYPE( prefix, elementName ) std::pair<int, CityObject::CityObjectsType>(NodeType::prefix ## _ ## elementName ## Node.typeID(), CityObject::CityObjectsType::COT_## elementName)
    #define HANDLE_GROUP_TYPE( prefix, elementName, enumtype ) std::pair<int, CityObject::CityObjectsType>(NodeType::prefix ## _ ## elementName ## Node.typeID(), enumtype)
    #define HANDLE_ATTR( prefix, elementName ) NodeType::prefix ## _ ## elementName ## Node.typeID()

    CityObjectElementParser::CityObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void (CityObject*)> callback)
        : GMLFeatureCollectionElementParser(documentParser, factory, logger)
        , m_lastGenericAttributeType(AttributeType::String), m_lastAttributeType(AttributeType::String)
        , m_typeMask(documentParser.getParserParams().objectsMask.get())
        , m_skipped(false)
    {
        m_callback = callback;
		m_model = nullptr;
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
                typeIDTypeMap.insert(HANDLE_TYPE(GEN, GenericOccupiedSpace));
                typeIDTypeMap.insert(HANDLE_TYPE(GEN, GenericUnoccupiedSpace));
                typeIDTypeMap.insert(HANDLE_TYPE(GEN, GenericLogicalSpace));
                typeIDTypeMap.insert(HANDLE_TYPE(GEN, GenericThematicSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Building));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingPart));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Room));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingInstallation));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingFurniture));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingConstructiveElement));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingRoom));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Door));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Window));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, CityFurniture));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, Storey));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingSubdivision));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, BuildingRoom));
                typeIDTypeMap.insert(HANDLE_TYPE(CORE, PointCloud));
                typeIDTypeMap.insert(HANDLE_TYPE(FRN, CityFurniture));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Track));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Road));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Railway));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Square));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Intersection));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Section));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, Waterway));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(TRANS, TransportationComplex, CityObject::CityObjectsType::COT_TransportationObject));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(TRANS, TrafficArea, CityObject::CityObjectsType::COT_TransportationObject));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(TRANS, AuxiliaryTrafficArea, CityObject::CityObjectsType::COT_TransportationObject));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, TrafficSpace));
                typeIDTypeMap.insert(HANDLE_TYPE(TRANS, AuxiliaryTrafficSpace));
                typeIDTypeMap.insert(HANDLE_TYPE(VEG, PlantCover));
                typeIDTypeMap.insert(HANDLE_TYPE(VEG, SolitaryVegetationObject));
                typeIDTypeMap.insert(HANDLE_TYPE(WTR, WaterBody));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(WTR, WaterSurface, CityObject::CityObjectsType::COT_WaterBody));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(WTR, WaterGroundSurface, CityObject::CityObjectsType::COT_WaterBody));
                typeIDTypeMap.insert(HANDLE_GROUP_TYPE(WTR, WaterClosureSurface, CityObject::CityObjectsType::COT_WaterBody));
                typeIDTypeMap.insert(HANDLE_TYPE(LUSE, LandUse));
                typeIDTypeMap.insert(HANDLE_TYPE(SUB, Tunnel));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, Bridge));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, BridgeConstructionElement));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, BridgeInstallation));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, BridgePart));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, OuterBridgeConstruction));
                typeIDTypeMap.insert(HANDLE_TYPE(BRID, OuterBridgeInstallation));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, FillingSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, WindowSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, DoorSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, WallSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, RoofSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, GroundSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, WallSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, RoofSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, GroundSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, ClosureSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, FloorSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, InteriorWallSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, CeilingSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, OuterCeilingSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, OuterFloorSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, FloorSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, InteriorWallSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, CeilingSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, OuterCeilingSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, OuterFloorSurface));
                typeIDTypeMap.insert(HANDLE_TYPE(CON, OtherConstruction));
                typeIDTypeMap.insert(HANDLE_TYPE(GRP, CityObjectGroup));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, ReliefFeature));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, ReliefComponent));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, TINRelief));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, MassPointRelief));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, BreaklineRelief));
                typeIDTypeMap.insert(HANDLE_TYPE(DEM, RasterRelief));
                typeIDTypeMap.insert(HANDLE_TYPE(BLDG, IntBuildingInstallation));

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
                attributesSet.insert(HANDLE_ATTR(GEN, Area));
                attributesSet.insert(HANDLE_ATTR(GEN, SpaceType));
                attributesSet.insert(HANDLE_ATTR(GEN, Volume));
                attributesSet.insert(HANDLE_ATTR(LUSE, Class));
                attributesSet.insert(HANDLE_ATTR(LUSE, Function));
                attributesSet.insert(HANDLE_ATTR(LUSE, Usage));
                attributesSet.insert(HANDLE_ATTR(DEM, Lod));
                attributesSet.insert(HANDLE_ATTR(TRANS, Usage));
                attributesSet.insert(HANDLE_ATTR(TRANS, Function));
                attributesSet.insert(HANDLE_ATTR(TRANS, SurfaceMaterial));
                attributesSet.insert(HANDLE_ATTR(TRANS, Granularity));
                attributesSet.insert(HANDLE_ATTR(WTR, Class));
                attributesSet.insert(HANDLE_ATTR(WTR, Function));
                attributesSet.insert(HANDLE_ATTR(WTR, Usage));
                attributesSet.insert(HANDLE_ATTR(WTR, WaterLevel));


                attributeTypeMap[HANDLE_ATTR(CORE, CreationDate)] = AttributeType::Date;
                attributeTypeMap[HANDLE_ATTR(CORE, TerminationDate)] = AttributeType::Date;
                attributeTypeMap[HANDLE_ATTR(BLDG, Type)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(BLDG, Class)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(BLDG, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(BLDG, Usage)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(BLDG, YearOfConstruction)] = AttributeType::Date;
                attributeTypeMap[HANDLE_ATTR(BLDG, YearOfDemolition)] = AttributeType::Date;
                attributeTypeMap[HANDLE_ATTR(BLDG, StoreyHeightsAboveGround)] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(BLDG, StoreyHeightsBelowGround)] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(BLDG, StoreysBelowGround)] = AttributeType::Integer;
                attributeTypeMap[HANDLE_ATTR(BLDG, StoreysAboveGround)] = AttributeType::Integer;
                attributeTypeMap[HANDLE_ATTR(BLDG, MeasuredHeight)] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(BLDG, RoofType)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(VEG, Class )] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(VEG, Function )] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(VEG, AverageHeight )] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(VEG, Species )] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(VEG, Height )] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(VEG, TrunkDiameter )] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(VEG, CrownDiameter )] = AttributeType::Double;
                attributeTypeMap[HANDLE_ATTR(FRN, Class)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(FRN, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GRP, Class)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GRP, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GRP, Usage)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GEN, Class)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GEN, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GEN, Usage)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GEN, Area)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GEN, SpaceType)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(GEN, Volume)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(LUSE, Class)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(LUSE, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(LUSE, Usage)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(DEM, Lod)] = AttributeType::Integer;
                attributeTypeMap[HANDLE_ATTR(TRANS, Usage)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(TRANS, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(TRANS, SurfaceMaterial)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(TRANS, Granularity)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(WTR, Class)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(WTR, Function)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(WTR, Usage)] = AttributeType::String;
                attributeTypeMap[HANDLE_ATTR(WTR, WaterLevel)] = AttributeType::Double;

                attributesSetInitialized = true;
            }
        }
    }

    AttributeType CityObjectElementParser::getAttributeType(const NodeType::XMLNode& node)
    {
        if (node == NodeType::GEN_StringAttributeNode) {
            return AttributeType::String;
        } else if (node == NodeType::GEN_DoubleAttributeNode) {
            return AttributeType::Double;
        } else if (node == NodeType::GEN_IntAttributeNode) {
            return AttributeType::Integer;
        } else if (node == NodeType::GEN_DateAttributeNode) {
            return AttributeType::Date;
        } else if (node == NodeType::GEN_UriAttributeNode) {
            return AttributeType::Uri;
        } else if (node == NodeType::CORE_GenericAttributeNode) {
            return AttributeType::Generic;
        } else {
            // fallback to string for other types
            return AttributeType::String;
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

        auto const type = static_cast<CityObject::CityObjectsType>(it->second);
        if (m_typeMask.test(static_cast<size_t>(type))) {
            m_model = m_factory.createCityObject(attributes.getCityGMLIDAttribute(), type);
            return true;
        } else {
            m_skipped = true;//skipUnknownOrUnexpectedElement(node.name());//m_skipped = true;
            CITYGML_LOG_TRACE(m_logger, "Ignoring start tag <" << node.name() << "> because it is ignored at " << getDocumentLocation());
            return false;
        }

    }

    bool CityObjectElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        if (getSourceSRSOverride()) {
            Envelope *envelope = new Envelope(getEnvelope().srsName());
            envelope->setLowerBound(m_model->getEnvelope().getLowerBound());
            envelope->setUpperBound(m_model->getEnvelope().getUpperBound());
            m_model->setEnvelope(envelope);
        }
        m_callback(m_model);
        m_model = nullptr;
        return true;
    }

    bool CityObjectElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_skipped) {
            return false;
        }
        initializeAttributesSet();

        if (m_model == nullptr) {
            throw std::runtime_error("CityObjectElementParser::parseChildElementStartTag called before CityObjectElementParser::parseElementStartTag");
        }

        if (    node == NodeType::GEN_StringAttributeNode
             || node == NodeType::GEN_DoubleAttributeNode
             || node == NodeType::GEN_IntAttributeNode
             || node == NodeType::GEN_DateAttributeNode
             || node == NodeType::GEN_UriAttributeNode) {

            if (m_lastAttributeType == AttributeType::Generic) {
                m_lastGenericAttributeType = getAttributeType(node);
            } else {
                m_lastAttributeName = attributes.getAttribute("name");
                m_lastAttributeType = getAttributeType(node);
            }
        } else if (node == NodeType::CORE_GenericAttributeNode) {
            m_lastAttributeType = getAttributeType(node);
        } else if (attributesSet.count(node.typeID()) > 0 
            || node == NodeType::GEN_NameNode
            || node == NodeType::GEN_ValueNode) {

            return true;
        } else if (node == NodeType::GML_RectifiedGridCoverageNode) {
            
            setParserForNextElement(new RectifiedGridCoverageParser(m_documentParser, m_factory, m_logger, [this](RectifiedGridCoverage * rectifiedGridCoverage) {
                m_model->setRectifiedGridCoverage(rectifiedGridCoverage);
            }));
        } else if (node == NodeType::BLDG_BoundedByNode
                   || node == NodeType::BLDG_OuterBuildingInstallationNode
                   || node == NodeType::BLDG_InteriorBuildingInstallationNode
                   || node == NodeType::BLDG_InteriorFurnitureNode
                   || node == NodeType::BLDG_RoomInstallationNode
                   || node == NodeType::BLDG_InteriorRoomNode
                   || node == NodeType::BLDG_OpeningNode
                   || node == NodeType::BLDG_ConsistsOfBuildingPartNode
                   || node == NodeType::BLDG_BuildingPartNode
                   || node == NodeType::BLDG_BuildingConstructiveElementNode
                   || node == NodeType::BLDG_BuildingRoomNode
                   || node == NodeType::BLDG_BuildingInstallationNode
                   || node == NodeType::BLDG_BuildingSubdivisionNode
                   || node == NodeType::BLDG_StoreyNode
                   || node == NodeType::GRP_GroupMemberNode
                   || node == NodeType::GRP_ParentNode
                   || node == NodeType::TRANS_TrafficAreaNode
                   || node == NodeType::TRANS_AuxiliaryTrafficAreaNode
                   || node == NodeType::TRANS_TrafficSpaceNode
                   || node == NodeType::TRANS_AuxiliaryTrafficSpaceNode
                   || node == NodeType::TRANS_IntersectionNode
                   || node == NodeType::TRANS_SectionNode
                   || node == NodeType::TRANS_WaterwayNode
                   || node == NodeType::WTR_BoundedByNode
                   || node == NodeType::DEM_ReliefComponentNode
                   || node == NodeType::DEM_TINReliefNode
                   || node == NodeType::DEM_MassPointReliefNode
                   || node == NodeType::DEM_BreaklineReliefNode
                   || node == NodeType::DEM_RasterReliefNode
                   || node == NodeType::DEM_GridNode
                   || node == NodeType::CORE_GeneralizesToNode
                   || node == NodeType::GEN_GenericOccupiedSpaceNode
                   || node == NodeType::GEN_GenericUnoccupiedSpaceNode
                   || node == NodeType::GEN_GenericLogicalSpaceNode
                   || node == NodeType::GEN_GenericThematicSurfaceNode
                   || node == NodeType::CORE_BoundaryNode
                   || node == NodeType::CON_FillingSurfaceNode
                   || node == NodeType::CON_WindowSurfaceNode
                   || node == NodeType::CON_DoorSurfaceNode
                   || node == NodeType::CORE_PointCloudNode
                   || node == NodeType::CON_OtherConstructionNode
                   || node == NodeType::BRID_OuterBridgeConstructionNode
                   || node == NodeType::BRID_OuterBridgeInstallationNode
                   || node == NodeType::BRID_BridgeConstructionElementNode
                   || node == NodeType::BRID_BridgeInstallationNode
                   || node == NodeType::BRID_BridgePartNode) {
            setParserForNextElement(new CityObjectElementParser(m_documentParser, m_factory, m_logger, [this](CityObject* obj) {
                                        m_model->addChildCityObject(obj);
                                    }));
        } else if (node == NodeType::APP_AppearanceNode // Compatibility with CityGML 1.0 (in CityGML 2 CityObjects can only contain appearanceMember elements)
                   || node == NodeType::APP_AppearanceMemberNode) {

            setParserForNextElement(new AppearanceElementParser(m_documentParser, m_factory, m_logger));
        } else if (node == NodeType::DEM_ExtentNode
                   || node == NodeType::DEM_TinNode
                   || node == NodeType::DEM_ReliefPointsNode
                   || node == NodeType::DEM_RidgeOrValleyLinesNode
                   || node == NodeType::DEM_BreaklinesNode) {
            
            std::string lod = m_model->getAttribute("dem:lod");
            if (!lod.empty()) {
                parseGeometryForLODLevel(std::stoi(lod));
            } else {
                parseGeometryForLODLevel(0);
            }
        } else if (node == NodeType::GEN_Lod0TerrainIntersectionNode
                   || node == NodeType::WTR_Lod0MultiCurveNode
                   || node == NodeType::WTR_Lod0MultiSurfaceNode
                   || node == NodeType::CORE_Lod0MultiSurfaceNode) {
            
            parseGeometryForLODLevel(0);
        } else if (node == NodeType::BLDG_Lod0FootPrintNode) {
            // for Lod0 footprint, we must explicitly set the City Object Type, because the parent type is "Building", and it doesn't allow to discriminate between the ground and roof surface
            parseGeometryForLODLevel(0, CityObject::CityObjectsType::COT_GroundSurface);
        } else if (node == NodeType::BLDG_Lod0RoofEdgeNode) {
            // for Lod0 roof edge, we must explicitly set the City Object Type, because the parent type is "Building", and it doesn't allow to discriminate between the ground and roof surface
            parseGeometryForLODLevel(0, CityObject::CityObjectsType::COT_RoofSurface);
        } else if (node == NodeType::BLDG_Lod1MultiCurveNode
                   || node == NodeType::BLDG_Lod1MultiSurfaceNode
                   || node == NodeType::BLDG_Lod1SolidNode
                   || node == NodeType::BLDG_Lod1TerrainIntersectionNode
                   || node == NodeType::GEN_Lod1TerrainIntersectionNode
                   || node == NodeType::FRN_Lod1TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod1MultiSurfaceNode
                   || node == NodeType::TRANS_Lod1MultiSurfaceNode
                   || node == NodeType::WTR_Lod1MultiCurveNode
                   || node == NodeType::WTR_Lod1MultiSurfaceNode
                   || node == NodeType::CORE_Lod0MultiSurfaceNode
                   || node == NodeType::CORE_Lod1MultiSurfaceNode
                   || node == NodeType::WTR_Lod1SolidNode) {

            parseGeometryForLODLevel(1);
        } else if (node == NodeType::BLDG_Lod2MultiCurveNode
                   || node == NodeType::BLDG_Lod2MultiSurfaceNode
                   || node == NodeType::BLDG_Lod2SolidNode
                   || node == NodeType::BLDG_Lod2TerrainIntersectionNode
                   || node == NodeType::GEN_Lod2TerrainIntersectionNode
                   || node == NodeType::FRN_Lod2TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod2MultiSurfaceNode
                   || node == NodeType::TRANS_Lod2MultiSurfaceNode
                   || node == NodeType::WTR_Lod2SolidNode
                   || node == NodeType::WTR_Lod2SurfaceNode
                   || node == NodeType::GEN_Lod0MultiCurveNode
                   || node == NodeType::GEN_Lod2MultiCurveNode
                   || node == NodeType::GEN_Lod3MultiCurveNode
                   || node == NodeType::GEN_Lod0MultiSurfaceNode
                   || node == NodeType::GEN_Lod2MultiSurfaceNode
                   || node == NodeType::GEN_Lod3MultiSurfaceNode
                   || node == NodeType::CORE_Lod0MultiSurfaceNode
                   || node == NodeType::CORE_Lod2MultiSurfaceNode
                   || node == NodeType::CORE_Lod3MultiSurfaceNode) {

            parseGeometryForLODLevel(2);
        } else if (node == NodeType::BLDG_Lod3MultiCurveNode
                   || node == NodeType::BLDG_Lod3MultiSurfaceNode
                   || node == NodeType::CORE_Lod3MultiSurfaceNode
                   || node == NodeType::BLDG_Lod3SolidNode
                   || node == NodeType::BLDG_Lod3TerrainIntersectionNode
                   || node == NodeType::GEN_Lod3TerrainIntersectionNode
                   || node == NodeType::FRN_Lod3TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod3MultiSurfaceNode
                   || node == NodeType::TRANS_Lod3MultiSurfaceNode
                   || node == NodeType::WTR_Lod3SolidNode
                   || node == NodeType::WTR_Lod3SurfaceNode) {

            parseGeometryForLODLevel(3);
        } else if (node == NodeType::BLDG_Lod4MultiCurveNode
                   || node == NodeType::BLDG_Lod4SolidNode
                   || node == NodeType::BLDG_Lod4MultiSurfaceNode
                   || node == NodeType::BLDG_Lod4SolidNode
                   || node == NodeType::BLDG_Lod4TerrainIntersectionNode
                   || node == NodeType::GEN_Lod4TerrainIntersectionNode
                   || node == NodeType::FRN_Lod4TerrainIntersectionNode
                   || node == NodeType::LUSE_Lod4MultiSurfaceNode
                   || node == NodeType::TRANS_Lod4MultiSurfaceNode
                   || node == NodeType::WTR_Lod4SolidNode
                   || node == NodeType::WTR_Lod4SurfaceNode) {

            parseGeometryForLODLevel(4);
        } else if (node == NodeType::GEN_Lod0GeometryNode) {
            parseGeometryPropertyElementForLODLevel(0, attributes.getCityGMLIDAttribute());
        }
        else if (node == NodeType::GEN_Lod1GeometryNode
                   || node == NodeType::FRN_Lod1GeometryNode
                   || node == NodeType::VEG_Lod1GeometryNode
                   || node == NodeType::BRID_Lod1GeometryNode) {
            parseGeometryPropertyElementForLODLevel(1, attributes.getCityGMLIDAttribute());
        } else if (node == NodeType::GEN_Lod2GeometryNode
                   || node == NodeType::FRN_Lod2GeometryNode
                   || node == NodeType::BLDG_Lod2GeometryNode
                   || node == NodeType::VEG_Lod2GeometryNode
                   || node == NodeType::BRID_Lod2GeometryNode ) {
            parseGeometryPropertyElementForLODLevel(2, attributes.getCityGMLIDAttribute());
        } else if (node == NodeType::GEN_Lod3GeometryNode
                   || node == NodeType::FRN_Lod3GeometryNode
                   || node == NodeType::BLDG_Lod3GeometryNode
                   || node == NodeType::VEG_Lod3GeometryNode
                   || node == NodeType::BRID_Lod3GeometryNode) {
            parseGeometryPropertyElementForLODLevel(3, attributes.getCityGMLIDAttribute());
        } else if (node == NodeType::GEN_Lod4GeometryNode
                   || node == NodeType::FRN_Lod4GeometryNode
                   || node == NodeType::BLDG_Lod4GeometryNode
                   || node == NodeType::VEG_Lod4GeometryNode
                   || node == NodeType::BRID_Lod4GeometryNode) {
            parseGeometryPropertyElementForLODLevel(4, attributes.getCityGMLIDAttribute());
        } else if (node == NodeType::GEN_Lod0ImplicitRepresentationNode) {
          
            parseImplicitGeometryForLODLevel(0);
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
        } else if (node == NodeType::CORE_ExternalReferenceNode){
            
            setParserForNextElement(new ExternalReferenceParser(m_documentParser, m_factory, m_logger, [this](ExternalReference * externalReference){
                m_model->setExternalReference(externalReference);
            }));
        } else if (node == NodeType::GML_MultiPointNode
                   || node == NodeType::GRP_GeometryNode
                   || node == NodeType::TRANS_Lod0NetworkNode) {
            CITYGML_LOG_INFO(m_logger, "Skipping CityObject child element <" << node  << ">  at " << getDocumentLocation() << " (Currently not supported!)");
            setParserForNextElement(new SkipElementParser(m_documentParser, m_logger, node));
            return true;
        } else if (node == NodeType::BLDG_AddressNode
                   || node == NodeType::CORE_AddressNode
                   || node == NodeType::CORE_XalAddressNode) {
            setParserForNextElement(new AddressParser(m_documentParser, m_factory, m_logger, [this](std::unique_ptr<Address>&& address) {
                m_model->setAddress(std::move(address));
            }));
            return true;
        } else {
            return GMLFeatureCollectionElementParser::parseChildElementStartTag(node, attributes);
        }

        return true;

    }

    bool CityObjectElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (m_skipped) {
            return false;
        } else if (m_model == nullptr) {
            throw std::runtime_error("CityObjectElementParser::parseChildElementEndTag called before CityObjectElementParser::parseElementStartTag");
        }

        initializeAttributesSet();

        if (    node == NodeType::GEN_StringAttributeNode
             || node == NodeType::GEN_DoubleAttributeNode
             || node == NodeType::GEN_IntAttributeNode
             || node == NodeType::GEN_DateAttributeNode
             || node == NodeType::GEN_UriAttributeNode) {

            if (m_lastAttributeType != AttributeType::Generic) {
                m_lastAttributeName = "";
                m_lastAttributeType = AttributeType::String;
            }

            return true;
        } else if (node == NodeType::CORE_GenericAttributeNode) {
            if(!m_lastAttributeName.empty())
                m_model->setAttribute(m_lastAttributeName, m_lastGenericAttributeValue, m_lastGenericAttributeType);
            m_lastAttributeName = "";
            m_lastGenericAttributeValue = "";
            m_lastAttributeType = AttributeType::String;
            m_lastGenericAttributeType = AttributeType::String;
            return true;
        } else if (node == NodeType::GEN_NameNode) {
            m_lastAttributeName = characters;
            return true;
        } else if (node == NodeType::GEN_ValueNode) {

            if (m_lastAttributeType == AttributeType::Generic) {
                m_lastGenericAttributeValue = characters;
            } else {
                if (!m_lastAttributeName.empty()) {
                    m_model->setAttribute(m_lastAttributeName, characters, m_lastAttributeType);
                } else {
                    CITYGML_LOG_WARN(m_logger, "Found value node (" << NodeType::GEN_ValueNode << ") outside attribute node... ignore.");
                }
            }

            return true;
        } else if (attributesSet.count(node.typeID()) > 0) {
            if (!characters.empty()) {
                m_model->setAttribute(node.name(), characters, attributeTypeMap.at(node.typeID()));
            }
            return true;
        } else if (node == NodeType::BLDG_BoundedByNode
                    || node == NodeType::BLDG_OuterBuildingInstallationNode
                    || node == NodeType::BLDG_InteriorBuildingInstallationNode
                    || node == NodeType::BLDG_InteriorFurnitureNode
                    || node == NodeType::BLDG_RoomInstallationNode
                    || node == NodeType::BLDG_InteriorRoomNode
                    || node == NodeType::BLDG_OpeningNode
                    || node == NodeType::APP_AppearanceNode
                    || node == NodeType::APP_AppearanceMemberNode
                    || node == NodeType::BLDG_Lod0FootPrintNode
                    || node == NodeType::BLDG_Lod0RoofEdgeNode
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
                    || node == NodeType::BLDG_BuildingPartNode
                    || node == NodeType::BLDG_BuildingConstructiveElementNode
                    || node == NodeType::BLDG_BuildingRoomNode
                    || node == NodeType::BLDG_BuildingInstallationNode
                    || node == NodeType::CON_FillingSurfaceNode
                    || node == NodeType::CON_WindowSurfaceNode
                    || node == NodeType::CON_DoorSurfaceNode
                    || node == NodeType::BLDG_BuildingSubdivisionNode
                    || node == NodeType::BLDG_StoreyNode
                    || node == NodeType::CON_OtherConstructionNode
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
                    || node == NodeType::VEG_Lod2GeometryNode
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
                    || node == NodeType::CORE_BoundaryNode
                    || node == NodeType::CORE_PointCloudNode
                    || node == NodeType::GML_MultiPointNode
                    || node == NodeType::GRP_GroupMemberNode
                    || node == NodeType::GRP_ParentNode
                    || node == NodeType::LUSE_Lod1MultiSurfaceNode
                    || node == NodeType::LUSE_Lod2MultiSurfaceNode
                    || node == NodeType::LUSE_Lod3MultiSurfaceNode
                    || node == NodeType::LUSE_Lod4MultiSurfaceNode
                    || node == NodeType::DEM_ReliefComponentNode
                    || node == NodeType::DEM_TINReliefNode
                    || node == NodeType::DEM_MassPointReliefNode
                    || node == NodeType::DEM_BreaklineReliefNode
                    || node == NodeType::DEM_RasterReliefNode
                    || node == NodeType::DEM_TinNode
                    || node == NodeType::DEM_ReliefPointsNode
                    || node == NodeType::DEM_RidgeOrValleyLinesNode
                    || node == NodeType::DEM_BreaklinesNode
                    || node == NodeType::DEM_GridNode
                    || node == NodeType::GEN_Lod0GeometryNode
                    || node == NodeType::GEN_Lod0ImplicitRepresentationNode
                    || node == NodeType::GEN_Lod0TerrainIntersectionNode
                    || node == NodeType::TRANS_Lod0NetworkNode
                    || node == NodeType::TRANS_TrafficAreaNode
                    || node == NodeType::TRANS_AuxiliaryTrafficAreaNode
                    || node == NodeType::TRANS_TrafficSpaceNode
                    || node == NodeType::TRANS_AuxiliaryTrafficSpaceNode
                    || node == NodeType::TRANS_IntersectionNode
                    || node == NodeType::TRANS_SectionNode
                    || node == NodeType::TRANS_WaterwayNode
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
                    || node == NodeType::WTR_BoundedByNode
                    || node == NodeType::BLDG_AddressNode
                    || node == NodeType::CORE_AddressNode
                    || node == NodeType::CORE_XalAddressNode
                    || node == NodeType::GEN_GenericOccupiedSpaceNode
                    || node == NodeType::GEN_GenericUnoccupiedSpaceNode
                    || node == NodeType::GEN_GenericLogicalSpaceNode
                    || node == NodeType::GEN_GenericThematicSurfaceNode
                    || node == NodeType::GEN_Lod0MultiCurveNode
                    || node == NodeType::GEN_Lod2MultiCurveNode
                    || node == NodeType::GEN_Lod3MultiCurveNode
                    || node == NodeType::GEN_Lod0MultiSurfaceNode
                    || node == NodeType::GEN_Lod2MultiSurfaceNode
                    || node == NodeType::GEN_Lod3MultiSurfaceNode
                    || node == NodeType::CORE_Lod0MultiSurfaceNode
                    || node == NodeType::CORE_Lod1MultiSurfaceNode
                    || node == NodeType::CORE_Lod2MultiSurfaceNode
                    || node == NodeType::CORE_Lod3MultiSurfaceNode
                    || node == NodeType::CORE_BoundaryNode
                    || node == NodeType::BRID_OuterBridgeConstructionNode
                    || node == NodeType::BRID_OuterBridgeInstallationNode
                    || node == NodeType::BRID_BridgeConstructionElementNode
                    || node == NodeType::BRID_BridgeInstallationNode
                    || node == NodeType::BRID_BridgePartNode) {

            return true;
        }

        return GMLFeatureCollectionElementParser::parseChildElementEndTag(node, characters);

    }

    FeatureObject* CityObjectElementParser::getFeatureObject()
    {
        return m_model;
    }
    
    void CityObjectElementParser::parseGeometryForLODLevel(int lod, CityObject::CityObjectsType parentType)
    {
        setParserForNextElement(new GeometryElementParser(m_documentParser, m_factory, m_logger, lod, parentType, [this](Geometry* geom) {
            m_model->addGeometry(geom);
        }));
    }
    
    void CityObjectElementParser::parseGeometryForLODLevel(int lod)
    {
        parseGeometryForLODLevel(lod, m_model->getType());
    }

    void CityObjectElementParser::parseImplicitGeometryForLODLevel(int lod)
    {
        setParserForNextElement(new ImplicitGeometryElementParser(m_documentParser, m_factory, m_logger, lod, m_model->getType(), [this](ImplicitGeometry* imp) {
            m_model->addImplictGeometry(imp);
        }));
    }

    void CityObjectElementParser::parseGeometryPropertyElementForLODLevel(int lod, const std::string& id)
    {
        setParserForNextElement(new DelayedChoiceElementParser(m_documentParser, m_logger, {
            new PolygonElementParser(m_documentParser, m_factory, m_logger, [id, lod, this](std::shared_ptr<Polygon> p) {
                                                                       Geometry* geom = m_factory.createGeometry(id, m_model->getType(), lod);
                                                                       geom->addPolygon(p);
                                                                       m_model->addGeometry(geom);
                                                                   }),
            new LineStringElementParser(m_documentParser, m_factory, m_logger, [id, lod, this](std::shared_ptr<LineString> l) {
                                                                       Geometry* geom = m_factory.createGeometry(id, m_model->getType(), lod);
                                                                       geom->addLineString(l);
                                                                       m_model->addGeometry(geom);
                                                                   }),
            new GeometryElementParser(m_documentParser, m_factory, m_logger, lod, m_model->getType(), [this](Geometry* geom) {
                                                                       m_model->addGeometry(geom);
                                                                   })
        }));

    }

}
