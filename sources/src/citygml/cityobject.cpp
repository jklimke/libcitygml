#include "citygml/cityobject.h"
#include "citygml/geometry.h"
#include "citygml/implictgeometry.h"
#include "citygml/appearancemanager.h"
#include "citygml/citygml.h"
#include "citygml/citygmllogger.h"

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
        switch (m_type) {
        case COT_GenericCityObject:
            return "GenericCityObject";
        case COT_Building:
            return "Building";
        case COT_Room:
            return "Room";
        case COT_BuildingInstallation:
            return "BuildingInstallation";
        case COT_BuildingFurniture:
            return "BuildingFurniture";
        case COT_Door:
            return "Door";
        case COT_Window:
            return "Window";
        case COT_CityFurniture:
            return "CityFurniture";
        case COT_Track:
            return "Track";
        case COT_Road:
            return "Road";
        case COT_Railway:
            return "Railway";
        case COT_Square:
            return "Square";
        case COT_PlantCover:
            return "PlantCover";
        case COT_SolitaryVegetationObject:
            return "SolitaryVegetationObject";
        case COT_WaterBody:
            return "WaterBody";
        case COT_TINRelief:
            return "TINRelief";
        case COT_LandUse:
            return "LandUse";
        case COT_Tunnel:
            return "Tunnel";
        case COT_Bridge:
            return "Bridge";
        case COT_BridgeConstructionElement:
            return "BridgeConstructionElement";
        case COT_BridgeInstallation:
            return "BridgeInstallation";
        case COT_BridgePart:
            return "BridgePart";
        case COT_BuildingPart:
            return "BuildingPart";
        case COT_WallSurface:
            return "WallSurface";
        case COT_RoofSurface:
            return "RoofSurface";
        case COT_GroundSurface:
            return "GroundSurface";
        case COT_ClosureSurface:
            return "ClosureSurface";
        case COT_FloorSurface:
            return "FloorSurface";
        case COT_InteriorWallSurface:
            return "InteriorWallSurface";
        case COT_CeilingSurface:
            return "CeilingSurface";
        default:
            return "Unknown";
        }
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
            it++;
        }

        for (unsigned int i = 0; i < o.getGeometriesCount(); i++) {
            os << o.getGeometry(i);
        }

        os << "  * " << o.getGeometriesCount() << " geometries." << std::endl;

        return os;
    }

}
