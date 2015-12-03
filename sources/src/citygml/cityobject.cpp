#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>
#include <citygml/appearancemanager.h>
#include <citygml/citygml.h>
#include <citygml/citygmllogger.h>
#include <citygml/address.h>

#include <unordered_map>
#include <algorithm>

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
        return m_geometries.size();
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
        return m_implicitGeometries.size();
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
        return m_children.size();
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
        m_children.push_back(std::unique_ptr<CityObject>(cityObj));
    }

    const Address* CityObject::address() const
    {
        return m_address.get();
    }

    void CityObject::setAddress(std::unique_ptr<Address>&& address)
    {
        m_address = std::move(address);
    }

    void CityObject::finish(Tesselator& tesselator, bool optimize, std::shared_ptr<CityGMLLogger> logger)
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
        os << o.getType() << ": " << o.getId() << std::endl;
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
        case CityObject::COT_GenericCityObject:
            return "GenericCityObject";
        case CityObject::COT_Building:
            return "Building";
        case CityObject::COT_Room:
            return "Room";
        case CityObject::COT_BuildingInstallation:
            return "BuildingInstallation";
        case CityObject::COT_BuildingFurniture:
            return "BuildingFurniture";
        case CityObject::COT_Door:
            return "Door";
        case CityObject::COT_Window:
            return "Window";
        case CityObject::COT_CityFurniture:
            return "CityFurniture";
        case CityObject::COT_Track:
            return "Track";
        case CityObject::COT_Road:
            return "Road";
        case CityObject::COT_Railway:
            return "Railway";
        case CityObject::COT_Square:
            return "Square";
        case CityObject::COT_PlantCover:
            return "PlantCover";
        case CityObject::COT_SolitaryVegetationObject:
            return "SolitaryVegetationObject";
        case CityObject::COT_WaterBody:
            return "WaterBody";
        case CityObject::COT_ReliefFeature:
            return "ReliefFeature";
        case CityObject::COT_LandUse:
            return "LandUse";
        case CityObject::COT_Tunnel:
            return "Tunnel";
        case CityObject::COT_Bridge:
            return "Bridge";
        case CityObject::COT_BridgeConstructionElement:
            return "BridgeConstructionElement";
        case CityObject::COT_BridgeInstallation:
            return "BridgeInstallation";
        case CityObject::COT_BridgePart:
            return "BridgePart";
        case CityObject::COT_BuildingPart:
            return "BuildingPart";
        case CityObject::COT_WallSurface:
            return "WallSurface";
        case CityObject::COT_RoofSurface:
            return "RoofSurface";
        case CityObject::COT_GroundSurface:
            return "GroundSurface";
        case CityObject::COT_ClosureSurface:
            return "ClosureSurface";
        case CityObject::COT_FloorSurface:
            return "FloorSurface";
        case CityObject::COT_InteriorWallSurface:
            return "InteriorWallSurface";
        case CityObject::COT_CeilingSurface:
            return "CeilingSurface";
        case CityObject::COT_TransportationObject:
            return "TransportationObject";
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
         {cityObjectsTypeToLowerString(CityObject::COT_GenericCityObject), CityObject::COT_GenericCityObject},
         {cityObjectsTypeToLowerString(CityObject::COT_Building), CityObject::COT_Building},
         {cityObjectsTypeToLowerString(CityObject::COT_Room), CityObject::COT_Room},
         {cityObjectsTypeToLowerString(CityObject::COT_BuildingInstallation), CityObject::COT_BuildingInstallation},
         {cityObjectsTypeToLowerString(CityObject::COT_BuildingFurniture), CityObject::COT_BuildingFurniture},
         {cityObjectsTypeToLowerString(CityObject::COT_Door), CityObject::COT_Door},
         {cityObjectsTypeToLowerString(CityObject::COT_Window), CityObject::COT_Window},
         {cityObjectsTypeToLowerString(CityObject::COT_CityFurniture), CityObject::COT_CityFurniture},
         {cityObjectsTypeToLowerString(CityObject::COT_Track), CityObject::COT_Track},
         {cityObjectsTypeToLowerString(CityObject::COT_Road), CityObject::COT_Road},
         {cityObjectsTypeToLowerString(CityObject::COT_Railway), CityObject::COT_Railway},
         {cityObjectsTypeToLowerString(CityObject::COT_Square), CityObject::COT_Square},
         {cityObjectsTypeToLowerString(CityObject::COT_PlantCover), CityObject::COT_PlantCover},
         {cityObjectsTypeToLowerString(CityObject::COT_SolitaryVegetationObject), CityObject::COT_SolitaryVegetationObject},
         {cityObjectsTypeToLowerString(CityObject::COT_WaterBody), CityObject::COT_WaterBody},
         {cityObjectsTypeToLowerString(CityObject::COT_ReliefFeature), CityObject::COT_ReliefFeature},
         {cityObjectsTypeToLowerString(CityObject::COT_LandUse), CityObject::COT_LandUse},
         {cityObjectsTypeToLowerString(CityObject::COT_Tunnel), CityObject::COT_Tunnel},
         {cityObjectsTypeToLowerString(CityObject::COT_Bridge), CityObject::COT_Bridge},
         {cityObjectsTypeToLowerString(CityObject::COT_BridgeConstructionElement), CityObject::COT_BridgeConstructionElement},
         {cityObjectsTypeToLowerString(CityObject::COT_BridgeInstallation), CityObject::COT_BridgeInstallation},
         {cityObjectsTypeToLowerString(CityObject::COT_BridgePart), CityObject::COT_BridgePart},
         {cityObjectsTypeToLowerString(CityObject::COT_BuildingPart), CityObject::COT_BuildingPart},
         {cityObjectsTypeToLowerString(CityObject::COT_WallSurface), CityObject::COT_WallSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_RoofSurface), CityObject::COT_RoofSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_GroundSurface), CityObject::COT_GroundSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_ClosureSurface), CityObject::COT_ClosureSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_FloorSurface), CityObject::COT_FloorSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_InteriorWallSurface), CityObject::COT_InteriorWallSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_CeilingSurface), CityObject::COT_CeilingSurface},
         {cityObjectsTypeToLowerString(CityObject::COT_TransportationObject), CityObject::COT_TransportationObject}
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

        return CityObject::COT_All;
    }

}
