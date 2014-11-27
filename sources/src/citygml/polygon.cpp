#include "citygml/polygon.h"
#include "citygml/appearance.h"
#include "citygml/texture.h"
#include "citygml/texturecoordinates.h"
#include "citygml/tesselator.h"
#include "citygml/citygmllogger.h"
#include "citygml/texturetargetdefinition.h"
#include "citygml/materialtargetdefinition.h"

#include <algorithm>

#include <assert.h>

namespace citygml {

    Polygon::Polygon(const std::string& id, std::shared_ptr<CityGMLLogger> logger)  : AppearanceTarget( id ), m_negNormal( false )
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

        const TextureTargetDefinition* targetDef = getTextureTargetDefinitionForTheme(theme);
        if (targetDef == nullptr) {
            CITYGML_LOG_WARN(m_logger, "Polygon with id '" << this->getId() << "'' contains no texture for theme '" << theme << "'");
            return texCoords;
        }

        std::vector<std::string> orderedRingIDs;

        for ( const auto& ring : m_exteriorRings )
        {
            orderedRingIDs.push_back(ring->getId());
        }

        for ( const auto& ring : m_interiorRings )
        {
            orderedRingIDs.push_back(ring->getId());
        }

        if (targetDef->getTextureCoordinatesCount() != orderedRingIDs.size()) {
            CITYGML_LOG_WARN(m_logger, "Texture with id '" << targetDef->getAppearance()->getId() << "'' targets Polygon with id '" << this->getId() << "'"
                             << " but the number of TextureCoordinates objects (gml::textureCoordinates) is not equal with the number of LinearRing objects (gml:LinearRing)."
                             << " (Ring objects: " << orderedRingIDs.size() << " TextureCoordinates objects: " << targetDef->getTextureCoordinatesCount());
        }


        for (const std::string& ringId : orderedRingIDs) {

            const TextureCoordinates* coordinates = targetDef->getTextureCoordinatesForID(ringId);

            if (!coordinates) {
                CITYGML_LOG_WARN(m_logger, "Texture with id '" << targetDef->getAppearance()->getId() << "'' targets Polygon with id '" << this->getId() << "'"
                                 << " but does not contain a TextureCoordinates object (gml::textureCoordinates) for LinearRing object (gml:LinearRing) with id '" << ringId << "'");
            } else {
                texCoords.insert(texCoords.end(), coordinates->getCoords().begin(), coordinates->getCoords().end());
            }
        }

        return texCoords;
    }

    std::shared_ptr<const Texture> Polygon::getTextureForTheme(const std::string& theme) const
    {
        const TextureTargetDefinition* targetDef = getTextureTargetDefinitionForTheme(theme);

        if (targetDef == nullptr) {
            return nullptr;
        }

        return targetDef->getAppearance();
    }


    TVec3d Polygon::computeNormal()
    {
        if ( m_exteriorRings.empty() ) return TVec3d();

        TVec3d normal = m_exteriorRings.front()->computeNormal();

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

        std::vector<TextureTargetDefinition*> texTargetDefinitions = this->getTextureTargetDefinitions();

        // mergeRings should be done before merging polygons... hence m_exteriorRings should only contain one object
        for ( std::unique_ptr<LinearRing>& ring : m_exteriorRings )
        {
            ring->removeDuplicateVertices( texTargetDefinitions );
            m_vertices.insert(m_vertices.end(), ring->getVertices().begin(), ring->getVertices().end());
            ring->forgetVertices();
        }

        for ( std::unique_ptr<LinearRing>& ring : m_interiorRings )
        {
            ring->removeDuplicateVertices( texTargetDefinitions );
            m_vertices.insert(m_vertices.end(), ring->getVertices().begin(), ring->getVertices().end());
            ring->forgetVertices();
        }
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
