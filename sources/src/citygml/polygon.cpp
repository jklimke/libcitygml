#include "citygml/polygon.h"
#include "citygml/appearance.h"
#include "citygml/texture.h"
#include "citygml/tesselator.h"

#include <assert.h>

namespace citygml {

    Polygon::Polygon(const std::string& id)  : Object( id ), m_exteriorRing( nullptr ), m_negNormal( false ), m_geometry( nullptr )
    {

    }

    const std::vector<TVec3d>&Polygon::getVertices() const
    {
        return m_vertices;
    }

    const std::vector<unsigned int>&Polygon::getIndices() const
    {
        return m_indices;
    }

    const std::vector<TVec3f>&Polygon::getNormals() const
    {
        return m_normals;
    }

    const std::vector<TVec2f>& Polygon::getTexCoordsForTheme(const std::string& theme) const
    {

    }

    const std::vector<std::shared_ptr<Appearance> > Polygon::getAppearancesForTheme(const std::string& theme) const
    {

    }

    void Polygon::addAppearance(std::shared_ptr<Appearance> appearance)
    {
        m_appearances = appearance;
    }

    TVec3d Polygon::computeNormal()
    {
        if ( !m_exteriorRing ) return TVec3d();

        TVec3d normal = m_exteriorRing->computeNormal();

        return m_negNormal ? -normal : normal;
    }

    void Polygon::computeIndices(const TVec3d& normal, bool tesselate, Tesselator& tesselator )
    {
        m_indices.clear();

        if ( m_vertices.size() < 3 ) return;


        if (!tesselate) {
            // Create triangles' indices
            int indicesSize = 3 * ( m_vertices.size() - 2 );
            if ( indicesSize < 3 ) return;
            m_indices.resize( indicesSize );
            for ( int i = 0, p = 0; p < indicesSize - 2; i++, p += 3 )
                for ( unsigned int j = 0; j < 3; j++ )
                    m_indices[ p + j ] = i + j;
            return;
        }

        tesselator.init(m_vertices.size(), normal);

        tesselator.addContour( m_vertices );
        tesselator.compute();
        m_vertices = tesselator.getVertices();
        m_indices = tesselator.getIndices();
    }

    void Polygon::mergeRings()
    {
        // We will modify the vertices (delete duplicates) therefore we have to modify the corresponding texture coordinates
        std::vector<TextureTarget*> targets;
        for (std::shared_ptr<Appearance> appearance : m_appearances) {
            if (appearance->asTexture() == nullptr) continue;

            std::vector<TextureTarget*> current = appearance->asTexture()->getTextureTargetsFor(*this);

            targets.insert(targets.end(), current.begin(), current.end());
        }

        m_exteriorRing->removeDuplicateVertices( targets );

        m_vertices.insert(m_vertices.end(), m_exteriorRing->getVertices().begin(), m_exteriorRing->getVertices().end());
        m_exteriorRing->forgetVertices();

        for ( LinearRing* ring : m_interiorRings )
        {
            ring->removeDuplicateVertices( targets );
            m_vertices.insert(m_vertices.end(), ring->getVertices().begin(), ring->getVertices().end());
            ring->forgetVertices();
        }
    }

    // Merge polygon p into the current polygon
    bool Polygon::merge( Polygon* p )
    {
        if ( !p ) return false;
        if ( p->getVertices().size() == 0 ) return true;

        // merge vertices
        m_indices.insert(m_vertices.end(), p->m_vertices.begin(), p->m_vertices.end());
        m_indices.insert(m_indices.end(), p->m_indices.begin(), p->m_indices.end());
        m_normals.insert(m_normals.end(), p->m_normals.begin(), p->m_normals.end());
        m_appearances.insert(m_appearances.end(), p->m_appearances.begin(), p->m_appearances.end());
        // merge ids
        m_id += "+" + p->m_id;

        return true;
    }

    void Polygon::finish(bool doTesselate , Tesselator& tesselator)
    {
        mergeRings();
        TVec3d normal = computeNormal();
        computeIndices(normal, doTesselate, tesselator);

        // Create the normal per point field
        m_normals.resize( m_vertices.size() );
        for ( unsigned int i = 0; i < m_vertices.size(); i++ )
            m_normals[i] = TVec3f( (float)normal.x, (float)normal.y, (float)normal.z );
    }

    void Polygon::addRing( LinearRing* ring )
    {
        if ( ring->isExterior() ) m_exteriorRing = ring;
        else m_interiorRings.push_back( ring );
    }

    Polygon::~Polygon()
    {
        delete m_exteriorRing;
        std::vector< LinearRing* >::const_iterator it = m_interiorRings.begin();
        for ( ; it != m_interiorRings.end(); ++it ) delete *it;
    }

}
