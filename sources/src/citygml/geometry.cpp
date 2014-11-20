#include "citygml/geometry.h"

#include <citygml/composite.h>
#include <citygml/polygon.h>
#include <citygml/appearancemanager.h>
#include <citygml/appearance.h>

namespace citygml {

    Geometry::Geometry(const std::string& id, Geometry::GeometryType type, unsigned int lod) : Object( id ), m_finished(false), m_type( type ), m_lod( lod ), m_composite( 0 )
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

    Polygon* Geometry::operator[](unsigned int i)
    {
        return m_polygons[i];
    }

    const Polygon*Geometry::operator[](unsigned int i) const
    {
        return m_polygons[i];
    }

    Geometry::GeometryType Geometry::getType() const
    {
        return m_type;
    }

    Composite*Geometry::getComposite() const
    {
        return m_composite;
    }

    Geometry::~Geometry()
    {
        std::vector< Polygon* >::const_iterator it = m_polygons.begin();
        for ( ; it != m_polygons.end(); ++it ) delete *it;
    }

    void Geometry::addPolygon( Polygon* p )
    {
        p->m_geometry = this;
        m_polygons.push_back( p );
    }

    void Geometry::finish( AppearanceManager& appearanceManager, std::shared_ptr<Appearance> defAppearance,  const ParserParams& params )
    {
        // only need to finish geometry once
        if (m_finished)
            return;

        std::shared_ptr<Appearance> myappearance = appearanceManager.getAppearanceForTarget( getId() );
        std::vector< Polygon* >::const_iterator it = m_polygons.begin();
        for ( ; it != m_polygons.end(); ++it ) (*it)->finish( appearanceManager, myappearance ? myappearance : defAppearance, params.tesselate );

        bool finish = false;
        while ( !finish && params.optimize )
        {
            finish = true;
            int len = (int)m_polygons.size();
            for ( int i = 0; finish && i < len - 1; i++ )
            {
                for ( int j = i+1; finish && j < len - 1; j++ )
                {
                    if ( !m_polygons[i]->merge( m_polygons[j] ) ) continue;
                    delete m_polygons[j];
                    m_polygons.erase( m_polygons.begin() + j );
                    finish = false;
                }
            }
        }
        m_finished = true;
    }

    bool Geometry::merge( Geometry* g )
    {
        if ( !g || g->m_lod != m_lod || g->m_type != m_type ) return false;

        unsigned int pSize = g->m_polygons.size();
        for ( unsigned int i = 0; i < pSize; i++ )
            m_polygons.push_back( g->m_polygons[i] );

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
