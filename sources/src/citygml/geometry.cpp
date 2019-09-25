#include <citygml/geometry.h>

#include <citygml/polygon.h>
#include <citygml/appearancemanager.h>
#include <citygml/appearance.h>
#include <citygml/citygmllogger.h>

namespace citygml {

    Geometry::Geometry(const std::string& id, Geometry::GeometryType type, unsigned int lod, std::string srsName)
        : AppearanceTarget( id ), m_finished(false), m_type( type ), m_lod( lod ), m_srsName( srsName )
    {

    }

    unsigned int Geometry::getLOD() const
    {
        return m_lod;
    }

    unsigned int Geometry::getPolygonsCount() const
    {
        return m_polygons.size();
    }

    std::shared_ptr<Polygon> Geometry::getPolygon(unsigned int i)
    {
        return m_polygons.at(i);
    }

    std::shared_ptr<const Polygon> Geometry::getPolygon(unsigned int i) const
    {
        return m_polygons.at(i);
    }

    unsigned int Geometry::getLineStringCount() const
    {
        return m_lineStrings.size();
    }

    std::shared_ptr<LineString> Geometry::getLineString(unsigned int i)
    {
        return m_lineStrings.at(i);
    }

    std::shared_ptr<const LineString> Geometry::getLineString(unsigned int i) const
    {
        return m_lineStrings.at(i);
    }

    unsigned int Geometry::getGeometriesCount() const
    {
        return m_childGeometries.size();
    }

    const Geometry& Geometry::getGeometry(unsigned int i) const
    {
        return *m_childGeometries.at(i);
    }

    Geometry& Geometry::getGeometry(unsigned int i)
    {
        return *m_childGeometries.at(i);
    }

    void Geometry::addGeometry(Geometry* geom)
    {
        m_childGeometries.push_back(std::unique_ptr<Geometry>(geom));
    }

    Geometry::GeometryType Geometry::getType() const
    {
        return m_type;
    }

    std::string Geometry::getTypeAsString() const
    {
        switch (m_type) {
        case GeometryType::GT_Unknown:
            return "Unknown";
        case GeometryType::GT_Roof:
            return "Roof";
        case GeometryType::GT_Wall:
            return "Wall";
        case GeometryType::GT_Ground:
            return "Ground";
        case GeometryType::GT_Closure:
            return "Closure";
        case GeometryType::GT_Floor:
            return "Floor";
        case GeometryType::GT_InteriorWall:
            return "InteriorWall";
        case GeometryType::GT_Ceiling:
            return "Ceiling";
        case GeometryType::GT_OuterCeiling:
            return "OuterCeiling";
        case GeometryType::GT_OuterFloor:
            return "OuterFloor";
        default:
            return "Unknown";
        }
    }

    Geometry::~Geometry()
    {
    }

    unsigned int Geometry::lod() const
    {
        return m_lod;
    }

    void Geometry::setLod(unsigned int lod)
    {
        m_lod = lod;
    }

    std::string Geometry::getSRSName() const
    {
        return m_srsName;
    }

    void Geometry::setSRSName(const std::string& srsName)
    {
        m_srsName = srsName;
    }

    void Geometry::addPolygon( std::shared_ptr<Polygon> p )
    {
        m_polygons.push_back(p);
    }

    void Geometry::addLineString(std::shared_ptr<LineString> l)
    {
        m_lineStrings.push_back(l);
    }

    void Geometry::finish(Tesselator& tesselator, bool optimize, std::shared_ptr<CityGMLLogger> logger)
    {
        // only need to finish geometry once
        if (m_finished) {
            return;
        }

        m_finished = true;

        for (std::shared_ptr<Geometry>&  child : m_childGeometries) {
            child->addTargetDefinitionsOf(*this);
            child->finish(tesselator, optimize, logger);
        }

        for (std::shared_ptr<Polygon>& polygon : m_polygons) {
            polygon->addTargetDefinitionsOf(*this);
            polygon->finish(tesselator, optimize, logger);
        }

    }

    std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s )
    {
        unsigned int count = 0;
        for ( unsigned int i = 0; i < s.getPolygonsCount(); i++ )
        {
            os << s.getPolygon(i);
            count += s.getPolygon(i)->getVertices().size();
        }

        os << "  @ " << s.getPolygonsCount() << " polys [" << count << " vertices]" << std::endl;

        return os;
    }

}
