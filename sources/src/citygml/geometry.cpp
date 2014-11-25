#include "citygml/geometry.h"

#include <citygml/composite.h>
#include <citygml/polygon.h>
#include <citygml/appearancemanager.h>
#include <citygml/appearance.h>

namespace citygml {

    Geometry::Geometry(const std::string& id, Geometry::GeometryType type, unsigned int lod) : AppearanceTarget( id ), m_finished(false), m_type( type ), m_lod( lod ), m_composite( 0 )
    {

    }

    unsigned int Geometry::getLOD() const
    {
        return m_lod;
    }

    unsigned int Geometry::size() const
    {
        return m_polygons.size();
    }

    Polygon& Geometry::operator[](unsigned int i)
    {
        return *m_polygons[i];
    }

    const Polygon& Geometry::operator[](unsigned int i) const
    {
        return *m_polygons[i];
    }

    Geometry::GeometryType Geometry::getType() const
    {
        return m_type;
    }

    const Composite* Geometry::getComposite() const
    {
        return m_composite.get();
    }

    void Geometry::setComposite(Composite* composite)
    {
        m_composite = std::unique_ptr<Composite>(composite);
    }

    void Geometry::addAppearance(std::shared_ptr<Appearance> appearance)
    {
        m_appearances.insert(appearance);
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


    void Geometry::addPolygon( Polygon* p )
    {
        m_polygons.push_back( std::unique_ptr<Polygon>(p) );
    }

    void Geometry::finish(bool tesselate, Tesselator& tesselator, bool mergePolygons)
    {
        // only need to finish geometry once
        if (m_finished) {
            throw std::runtime_error("Called Geometry::finish on already finished Geometry.");
        }

        m_finished = true;

        if (mergePolygons && m_polygons.size() > 1) {

            // Try merge Polygons from left to right
            for (int i = 1; i < m_polygons.size(); i++) {

                m_polygons[0]->merge(m_polygons[i]);

            }

            m_polygons.resize(1);
        }

        for (std::unique_ptr<Polygon>& polygon : m_polygons) {
            // Add own appearances and that of the composite to the polygon
            // Note that the order is important. If there are multiple appearance objects that target the same
            // LinearRing then an appearance objects that target the polygon
            // have a higher priority then thoose that target the geometry which in turn have a higher pripority than thoose that
            // target the composite. The priority is defined by the order in which they are added to the polygon... consequently
            // the appearances that directly target the polygon must be added first
            polygon->addAppearancesOf(*this);

            if (m_composite != nullptr) {
                polygon->addAppearancesOf(*m_composite);
            }

            polygon->finish(tesselate, tesselator);
        }

    }

    bool Geometry::merge( Geometry* g )
    {
        if (g == nullptr || g->m_finished || this->m_finished) {
            throw std::runtime_error("Invalid call to Geometry::merge. Source Geometry is nullptr or Source/Target Geometry is already finished.");
        }

        if ( g->getLOD() != m_lod || g->getType() != m_type ) return false;

        m_polygons.insert(m_polygons.end(),
                               std::make_move_iterator(g->m_polygons.begin()),
                               std::make_move_iterator(g->m_polygons.end()));
        g->m_polygons.clear();

        m_id += "+" + g->m_id;

        return true;
    }

    std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s )
    {
        unsigned int count = 0;
        for ( unsigned int i = 0; i < s.size(); i++ )
        {
            os << *s[i];
            count += s[i]->getVertices().size();
        }

        os << "  @ " << s.m_polygons.size() << " polys [" << count << " vertices]" << std::endl;

        return os;
    }

}
