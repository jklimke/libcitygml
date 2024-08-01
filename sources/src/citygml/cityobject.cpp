#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>
#include <citygml/appearancemanager.h>
#include <citygml/citygml.h>
#include <citygml/citygmllogger.h>
#include <citygml/address.h>

#include <unordered_map>
#include <algorithm>

ENUM_CLASS_BITWISE_OPERATORS(citygml::CityObject::CityObjectsType);

namespace citygml {

    CityObject::CityObject(const std::string& id, CityObject::CityObjectsType type)  : FeatureObject( id ), m_type( type )
    {

    }

    CityObject::CityObjectsType CityObject::getType() const
    {
        return m_type;
    }

    std::string CityObject::getTypeAsString() const
    {
        return cityObjectsTypeToString(m_type);
    }

    unsigned int CityObject::getGeometriesCount() const
    {
        return static_cast<unsigned int>(m_geometries.size());
    }

    const Geometry& CityObject::getGeometry(unsigned int i) const
    {
        return *m_geometries[i];
    }

    Geometry& CityObject::getGeometry(unsigned int i)
    {
        return *m_geometries[i];
    }

    void CityObject::addGeometry(Geometry* geom)
    {
        m_geometries.push_back(std::unique_ptr<Geometry>(geom));
    }

    unsigned int CityObject::getImplicitGeometryCount() const
    {
        return static_cast<unsigned int>(m_implicitGeometries.size());
    }

    const ImplicitGeometry& CityObject::getImplicitGeometry(unsigned int i) const
    {
        return *m_implicitGeometries[i];
    }

    ImplicitGeometry& CityObject::getImplicitGeometry(unsigned int i)
    {
        return *m_implicitGeometries[i];
    }

    void CityObject::addImplictGeometry(ImplicitGeometry* implictGeom)
    {
		m_implicitGeometries.push_back(std::unique_ptr<ImplicitGeometry>(implictGeom));
    }

    unsigned int CityObject::getChildCityObjectsCount() const
    {
        return static_cast<unsigned int>(m_children.size());
    }

    const CityObject& CityObject::getChildCityObject(unsigned int i) const
    {
        return *m_children[i];
    }

    CityObject& CityObject::getChildCityObject(unsigned int i)
    {
        return *m_children[i];
    }

    void CityObject::addChildCityObject(CityObject* cityObj)
    {
		// don't add empty nodes as children
		if(cityObj!=NULL)
		{
            m_children.push_back(std::unique_ptr<CityObject>(cityObj));
		}
    }

    const Address* CityObject::address() const
    {
        return m_address.get();
    }

    void CityObject::setAddress(std::unique_ptr<Address>&& address)
    {
        m_address = std::move(address);
    }

    RectifiedGridCoverage const* CityObject::rectifiedGridCoverage() const {
        return m_rectifiedGridCoverage.get();
    }

    void CityObject::setRectifiedGridCoverage(RectifiedGridCoverage * rectifiedGridCoverage) {
        m_rectifiedGridCoverage = std::unique_ptr<RectifiedGridCoverage>(rectifiedGridCoverage);
    }

    ExternalReference const* CityObject::externalReference() const {
        return m_externalReference.get();
    }

    void CityObject::setExternalReference(ExternalReference * externalReference) {
        m_externalReference = std::unique_ptr<ExternalReference>(externalReference);
    }

    void CityObject::finish(TesselatorBase* tesselator, bool optimize, std::shared_ptr<CityGMLLogger> logger)
    {
        for (std::unique_ptr<Geometry>& geom : m_geometries) {
            geom->finish(tesselator, optimize, logger);
        }

        for (std::unique_ptr<ImplicitGeometry>& implictGeom : m_implicitGeometries) {
            for (int i = 0; i < implictGeom->getGeometriesCount(); i++) {
                implictGeom->getGeometry(i).finish(tesselator, optimize, logger);
            }
        }

        for (std::unique_ptr<CityObject>& child : m_children) {
            child->finish(tesselator, optimize, logger);
        }
    }

    CityObject::~CityObject()
    {
    }

    std::ostream& operator<<( std::ostream& os, const CityObject& o )
    {
        os << static_cast<uint64_t>(o.getType()) << ": " << o.getId() << std::endl;
        os << "  Envelope: " << o.getEnvelope() << std::endl;

        AttributesMap::const_iterator it = o.getAttributes().begin();
        while ( it != o.getAttributes().end() )
        {
            os << "  + " << it->first << ": " << it->second << std::endl;
            ++it;
        }

        for (unsigned int i = 0; i < o.getGeometriesCount(); i++) {
            os << o.getGeometry(i);
        }

        os << "  * " << o.getGeometriesCount() << " geometries." << std::endl;

        return os;
    }

    std::string cityObjectsTypeToString(const CityObject::CityObjectsType& t)
    {
        switch (t) {
        case CityObject::CityObjectsType::COT_GenericCityObject:
            return "GenericCityObject";
        case CityObject::CityObjectsType::COT_Building:
            return "Building";
        case CityObject::CityObjectsType::COT_Room:
            return "Room";
        case CityObject::CityObjectsType::COT_BuildingInstallation:
            return "BuildingInstallation";
        case CityObject::CityObjectsType::COT_BuildingFurniture:
            return "BuildingFurniture";
        case CityObject::CityObjectsType::COT_BuildingConstructiveElement:
            return "BuildingConstructiveElement";
        case CityObject::CityObjectsType::COT_BuildingRoom:
            return "BuildingRoom";
        case CityObject::CityObjectsType::COT_Door:
            return "Door";
        case CityObject::CityObjectsType::COT_Window:
            return "Window";
        case CityObject::CityObjectsType::COT_CityFurniture:
            return "CityFurniture";
        case CityObject::CityObjectsType::COT_Track:
            return "Track";
        case CityObject::CityObjectsType::COT_Road:
            return "Road";
        case CityObject::CityObjectsType::COT_Railway:
            return "Railway";
        case CityObject::CityObjectsType::COT_Square:
            return "Square";
        case CityObject::CityObjectsType::COT_Intersection:
            return "Intersection";
        case CityObject::CityObjectsType::COT_Section:
            return "Section";
        case CityObject::CityObjectsType::COT_Waterway:
            return "Waterway";
        case CityObject::CityObjectsType::COT_PlantCover:
            return "PlantCover";
        case CityObject::CityObjectsType::COT_SolitaryVegetationObject:
            return "SolitaryVegetationObject";
        case CityObject::CityObjectsType::COT_WaterBody:
            return "WaterBody";
        case CityObject::CityObjectsType::COT_ReliefFeature:
            return "ReliefFeature";
        case CityObject::CityObjectsType::COT_LandUse:
            return "LandUse";
        case CityObject::CityObjectsType::COT_Tunnel:
            return "Tunnel";
        case CityObject::CityObjectsType::COT_Bridge:
            return "Bridge";
        case CityObject::CityObjectsType::COT_BridgeConstructionElement:
            return "BridgeConstructionElement";
        case CityObject::CityObjectsType::COT_BridgeInstallation:
            return "BridgeInstallation";
        case CityObject::CityObjectsType::COT_BridgePart:
            return "BridgePart";
        case CityObject::CityObjectsType::COT_BuildingPart:
            return "BuildingPart";
        case CityObject::CityObjectsType::COT_WallSurface:
            return "WallSurface";
        case CityObject::CityObjectsType::COT_RoofSurface:
            return "RoofSurface";
        case CityObject::CityObjectsType::COT_GroundSurface:
            return "GroundSurface";
        case CityObject::CityObjectsType::COT_ClosureSurface:
            return "ClosureSurface";
        case CityObject::CityObjectsType::COT_FloorSurface:
            return "FloorSurface";
        case CityObject::CityObjectsType::COT_InteriorWallSurface:
            return "InteriorWallSurface";
        case CityObject::CityObjectsType::COT_CeilingSurface:
            return "CeilingSurface";
        case CityObject::CityObjectsType::COT_OuterCeilingSurface:
            return "OuterCeilingSurface";
        case CityObject::CityObjectsType::COT_OuterFloorSurface:
            return "OuterFloorSurface";
        case CityObject::CityObjectsType::COT_TransportationObject:
            return "TransportationObject";
        case CityObject::CityObjectsType::COT_IntBuildingInstallation:
	        return "IntBuildingInstallation";
        case CityObject::CityObjectsType::COT_GenericOccupiedSpace:
	        return "GenericOccupiedSpace";
        case CityObject::CityObjectsType::COT_GenericUnoccupiedSpace:
	        return "GenericUnoccupiedSpace";
        case CityObject::CityObjectsType::COT_GenericLogicalSpace:
	        return "GenericLogicalSpace";
        default:
            return "Unknown";
        }
    }

    std::string cityObjectsTypeToLowerString(const CityObject::CityObjectsType& t) {
        std::string str = cityObjectsTypeToString(t);
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    std::unordered_map<std::string, CityObject::CityObjectsType> stringTypeMap = {
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_GenericCityObject), CityObject::CityObjectsType::COT_GenericCityObject},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Building), CityObject::CityObjectsType::COT_Building},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Room), CityObject::CityObjectsType::COT_Room},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BuildingInstallation), CityObject::CityObjectsType::COT_BuildingInstallation},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BuildingFurniture), CityObject::CityObjectsType::COT_BuildingFurniture},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BuildingConstructiveElement), CityObject::CityObjectsType::COT_BuildingConstructiveElement},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BuildingRoom), CityObject::CityObjectsType::COT_BuildingRoom},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Door), CityObject::CityObjectsType::COT_Door},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Window), CityObject::CityObjectsType::COT_Window},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_CityFurniture), CityObject::CityObjectsType::COT_CityFurniture},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Track), CityObject::CityObjectsType::COT_Track},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Road), CityObject::CityObjectsType::COT_Road},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Railway), CityObject::CityObjectsType::COT_Railway},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Square), CityObject::CityObjectsType::COT_Square},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Intersection), CityObject::CityObjectsType::COT_Intersection},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Section), CityObject::CityObjectsType::COT_Section},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Waterway), CityObject::CityObjectsType::COT_Waterway},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_PlantCover), CityObject::CityObjectsType::COT_PlantCover},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_SolitaryVegetationObject), CityObject::CityObjectsType::COT_SolitaryVegetationObject},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_WaterBody), CityObject::CityObjectsType::COT_WaterBody},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_ReliefFeature), CityObject::CityObjectsType::COT_ReliefFeature},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_LandUse), CityObject::CityObjectsType::COT_LandUse},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Tunnel), CityObject::CityObjectsType::COT_Tunnel},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_Bridge), CityObject::CityObjectsType::COT_Bridge},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BridgeConstructionElement), CityObject::CityObjectsType::COT_BridgeConstructionElement},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BridgeInstallation), CityObject::CityObjectsType::COT_BridgeInstallation},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BridgePart), CityObject::CityObjectsType::COT_BridgePart},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_BuildingPart), CityObject::CityObjectsType::COT_BuildingPart},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_WallSurface), CityObject::CityObjectsType::COT_WallSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_RoofSurface), CityObject::CityObjectsType::COT_RoofSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_GroundSurface), CityObject::CityObjectsType::COT_GroundSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_ClosureSurface), CityObject::CityObjectsType::COT_ClosureSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_FloorSurface), CityObject::CityObjectsType::COT_FloorSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_InteriorWallSurface), CityObject::CityObjectsType::COT_InteriorWallSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_CeilingSurface), CityObject::CityObjectsType::COT_CeilingSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_OuterCeilingSurface), CityObject::CityObjectsType::COT_OuterCeilingSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_OuterFloorSurface), CityObject::CityObjectsType::COT_OuterFloorSurface},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_TransportationObject), CityObject::CityObjectsType::COT_TransportationObject},
	     {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_IntBuildingInstallation), CityObject::CityObjectsType::COT_IntBuildingInstallation},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_GenericOccupiedSpace), CityObject::CityObjectsType::COT_GenericOccupiedSpace},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_GenericUnoccupiedSpace), CityObject::CityObjectsType::COT_GenericUnoccupiedSpace},
         {cityObjectsTypeToLowerString(CityObject::CityObjectsType::COT_GenericLogicalSpace), CityObject::CityObjectsType::COT_GenericLogicalSpace}
    };

    CityObject::CityObjectsType cityObjectsTypeFromString(const std::string& s, bool& valid)
    {
        std::string lower_s = s;
        std::transform(lower_s.begin(), lower_s.end(), lower_s.begin(), ::tolower);
        auto it = stringTypeMap.find(lower_s);

        valid = it != stringTypeMap.end();

        if (valid) {
            return it->second;
        }

        return CityObject::CityObjectsType::COT_All;
    }

}
