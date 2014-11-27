#include "citygml/cityobject.h"
#include "citygml/geometry.h"
#include "citygml/implictgeometry.h"
#include "citygml/composite.h"
#include "citygml/appearancemanager.h"
#include "citygml/citygml.h"

namespace citygml {

    CityObject::CityObject(const std::string& id, CityObject::CityObjectsType type)  : Object( id ), m_type( type )
    {

    }

    CityObject::CityObjectsType CityObject::getType() const
    {
        return m_type;
    }

    const Envelope& CityObject::getEnvelope() const
    {
        return m_envelope;
    }

    void CityObject::setEnvelope(Envelope envelope)
    {
        m_envelope = envelope;
    }

    unsigned int CityObject::getGeometriesCount() const
    {
        return m_geometries.size();
    }

    const Geometry& CityObject::getGeometry(unsigned int i) const
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

    void CityObject::addImplictGeometry(ImplicitGeometry* implictGeom)
    {
        m_implicitGeometries.push_back(std::unique_ptr<ImplicitGeometry>(implictGeom));
    }

    unsigned int CityObject::getChildCityObjecsCount() const
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

    void CityObject::finish(bool tesselate, Tesselator& tesselator)
    {
        for (std::unique_ptr<Geometry>& geom : m_geometries) {
            geom->finish(tesselate, tesselator);
        }

        for (std::unique_ptr<ImplicitGeometry>& implictGeom : m_implicitGeometries) {
            for (int i = 0; i < implictGeom->getGeometriesCount(); i++) {
                implictGeom->getGeometry(i).finish(tesselate, tesselator);
            }
        }

        for (std::unique_ptr<CityObject>& child : m_children) {
            child->finish(tesselate, tesselator);
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
