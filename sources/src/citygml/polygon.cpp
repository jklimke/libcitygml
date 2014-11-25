#include "citygml/polygon.h"
#include "citygml/appearance.h"
#include "citygml/texture.h"
#include "citygml/texturecoordinates.h"
#include "citygml/tesselator.h"
#include "citygml/citygmllogger.h"

#include <assert.h>

namespace citygml {

    Polygon::Polygon(const std::string& id, std::shared_ptr<CityGMLLogger> logger)  : Object( id ), m_exteriorRing( nullptr ), m_negNormal( false )
    {
        m_logger = logger;
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

    const std::vector<TVec2f> Polygon::getTexCoordsForTheme(const std::string& theme) const
    {
        // Lazy generation of texCoords
        std::vector<TVec2f> texCoords;

        std::shared_ptr<Texture> texture = getTextureForTheme(theme);
        if (texture == nullptr) {
            CITYGML_LOG_WARN(m_logger, "Polygon with id '" << this->getId() << "'' contains no texture for theme '" << theme << "'");
            return texCoords;
        }

        std::vector<std::string> orderedRingIDs;
        if (m_exteriorRing) {
            orderedRingIDs.push_back(m_exteriorRing->getId());
        }

        for ( const auto& ring : m_interiorRings )
        {
            orderedRingIDs.push_back(ring->getId());
        }

        const TextureTarget& target = texture->getTextureTargetFor(*this);

        assert(target.valid());

        if (target.getTexCoordinatesList().size() != orderedRingIDs.size()) {
            CITYGML_LOG_WARN(m_logger, "Texture with id '" << texture->getId() << "'' targets Polygon with id '" << this->getId() << "'"
                             << " but the number of TextureCoordinates objects (gml::textureCoordinates) is not equal with the number of LinearRing objects (gml:LinearRing)."
                             << " (Ring objects: " << orderedRingIDs.size() << " TextureCoordinates objects: " << target.getTexCoordinatesList().size());
        }

        for (const std::string& ringId : orderedRingIDs) {
            bool found = false;
            for (const TextureCoordinates& coordinates : target.getTexCoordinatesList()) {
                if (coordinates.targets(ringId)) {
                    texCoords.insert(texCoords.end(), coordinates.getCoords().begin(), coordinates.getCoords().end());
                    found = true;
                    break;
                }
            }

            if (!found) {
                CITYGML_LOG_WARN(m_logger, "Texture with id '" << texture->getId() << "'' targets Polygon with id '" << this->getId() << "'"
                                 << " but does not contain a TextureCoordinates object (gml::textureCoordinates) for LinearRing object (gml:LinearRing) with id '" << ringId << "'");
            }
        }
    }

    const std::shared_ptr<Texture> Polygon::getTextureForTheme(const std::string& theme) const
    {
        for (std::shared_ptr<Appearance> appearance : m_appearances) {

            std::shared_ptr<Texture> tex = appearance->asTexture();

            if (tex == nullptr) continue;

            if (!tex->inTheme(theme)) continue;

            return tex;
        }

        return nullptr;
    }

    void Polygon::addAppearance(std::shared_ptr<Appearance> appearance)
    {
        m_appearances.insert(appearance);
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
        std::vector<TextureTarget&> targets;
        for (std::shared_ptr<Appearance> appearance : m_appearances) {
            if (appearance->asTexture() == nullptr) continue;

            TextureTarget& current = appearance->asTexture()->getTextureTargetFor(*this);
            assert(current.valid());
            targets.push_back(current);
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

    void Polygon::merge( Polygon* p )
    {
        if (!p || m_finished || p->m_finished) {
            throw std::runtime_error("Invalid call to Polygon::merge. Source Polygon is nullptr or Source/Target Polygon is already finished.");
        }

        // merge vertices
        m_exteriorRings.insert(m_exteriorRings.end(),
                               std::make_move_iterator(p->m_exteriorRings.begin()),
                               std::make_move_iterator(p->m_exteriorRings.end()));
        p->m_exteriorRings.clear();

        m_interiorRings.insert(m_interiorRings.end(),
                               std::make_move_iterator(p->m_interiorRings.begin()),
                               std::make_move_iterator(p->m_interiorRings.end()));
        p->m_interiorRings.clear();

        m_appearances.insert(p->m_appearances.begin(), p->m_appearances.end());
        p->m_appearances.clear();

        // merge ids
        m_id += "+" + p->m_id;
    }

    void Polygon::finish(bool doTesselate , Tesselator& tesselator)
    {
        if (m_finished) {
            throw std::runtime_error("Called Polygon::finish on already finished polygon");
        }

        m_finished = true;
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
        if (m_finished) {
            throw std::runtime_error("Can't add LinearRing to finished Polygon.");
        }

        if (ring->isExterior() && !m_exteriorRings.empty()) {
            CITYGML_LOG_WARN(m_logger, "Duplicate definition of exterior LinearRing for Polygon with id '" << this->getId() << "'."
                             << " Using exterior LinearRing with id '" << m_exteriorRings.front()->getId() << "' and ignore LinearRing with id '" << ring->getId() << "'");
            delete ring;
            return;
        }

        if ( ring->isExterior() ) {
            m_exteriorRings.push_back(std::unique_ptr<LinearRing>(ring));
        }
        else {
            m_interiorRings.push_back( std::unique_ptr<LinearRing>(ring) );
        }
    }

    Polygon::~Polygon()
    {
    }

}
