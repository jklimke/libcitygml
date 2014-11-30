#include "citygml/linearring.h"
#include "citygml/texturecoordinates.h"
#include "citygml/texture.h"
#include "citygml/texturetargetdefinition.h"
#include <float.h>
#include <assert.h>

namespace citygml {

    LinearRing::LinearRing(const std::string& id, bool isExterior) : Object( id ), m_exterior( isExterior )
    {

    }

    bool LinearRing::isExterior() const
    {
        return m_exterior;
    }

    unsigned int LinearRing::size() const
    {
        return m_vertices.size();
    }

    void LinearRing::addVertex(const TVec3d& v)
    {
        m_vertices.push_back( v );
    }

    TVec3d LinearRing::computeNormal() const
    {
        unsigned int len = size();
        if ( len < 3 ) return TVec3d();

        // Tampieri, F. 1992. Newell's method for computing the plane equation of a polygon. In Graphics Gems III, pp.231-232.
        TVec3d n( 0., 0., 0. );
        for ( unsigned int i = 0; i < len; i++ )
        {
            const TVec3d& current = m_vertices[i];
            const TVec3d& next = m_vertices[ ( i + 1 ) % len];

            n.x += ( current.y - next.y ) * ( current.z + next.z );
            n.y += ( current.z - next.z ) * ( current.x + next.x );
            n.z += ( current.x - next.x ) * ( current.y + next.y );
        }
        return n.normal();
    }

    std::vector<TVec3d>& LinearRing::getVertices()
    {
        return m_vertices;
    }

    void LinearRing::setVertices(std::vector<TVec3d> vertices)
    {
        m_vertices = vertices;
    }

    const std::vector<TVec3d>& LinearRing::getVertices() const
    {
        return m_vertices;
    }

    void LinearRing::removeDuplicateVertices(const std::vector<TextureTargetDefinition*>& targets )
    {
        std::vector<TextureCoordinates*> coordinatesList;

        for (auto& texTarget : targets) {

            for (unsigned int i = 0; i < texTarget->getTextureCoordinatesCount(); i++) {
                TextureCoordinates* texCoords = texTarget->getTextureCoordinates(i);
                if (!texCoords->targets(*this)) continue;

                coordinatesList.push_back(texCoords);
            }

        }

        // Remove duplicated vertex
        unsigned int len = m_vertices.size();
        if ( len < 2 ) return;

        unsigned int i = 0;
        while ( i < m_vertices.size() )
        {
            if ( ( m_vertices[i] - m_vertices[ ( i + 1 ) % m_vertices.size() ] ).sqrLength() <= DBL_EPSILON )
            {
                m_vertices.erase( m_vertices.begin() + i );
                for (TextureCoordinates* coordinates : coordinatesList) {
                    coordinates->eraseCoordinate(i);
                }
            } else {
                i++;
            }
        }
    }

    void LinearRing::forgetVertices()
    {
        m_vertices.clear();
    }

}
