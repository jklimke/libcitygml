#include <citygml/polygon.h>
#include <citygml/appearance.h>
#include <citygml/texture.h>
#include <citygml/texturecoordinates.h>
#include <citygml/tesselator.h>
#include <citygml/citygmllogger.h>
#include <citygml/texturetargetdefinition.h>
#include <citygml/materialtargetdefinition.h>

#include <algorithm>
#include <stdexcept>
#include <assert.h>
#include <stdexcept>

#include <iostream>

namespace citygml {

    Polygon::Polygon(const std::string& id, std::shared_ptr<CityGMLLogger> logger)  : AppearanceTarget( id ), m_negNormal( false )
    {
        m_logger = logger;
        m_finished = false;
    }

    const std::vector<TVec3d>& Polygon::getVertices() const
    {
        return m_vertices;
    }

    std::vector<TVec3d>& Polygon::getVertices()
    {
        return m_vertices;
    }

    const std::vector<unsigned int>& Polygon::getIndices() const
    {
        return m_indices;
    }


    std::shared_ptr<const Material> Polygon::getMaterialFor(const std::string& theme, bool front) const
    {
        const auto targetDef = getMaterialTargetDefinitionForTheme(theme, front);
        if (targetDef == nullptr) {
            return nullptr;
        }
        return targetDef->getAppearance();
    }

    std::shared_ptr<const Material> Polygon::getMaterialFor(const std::string& theme) const
    {
        const auto result = getMaterialFor(theme, true);
        if (result != nullptr) {
            return result;
        }
        return getMaterialFor(theme, false);
    }

    std::shared_ptr<const Texture> Polygon::getTextureFor(const std::string& theme, bool front) const
    {
        const auto targetDef = getTextureTargetDefinitionForTheme(theme, front);
        if (targetDef == nullptr) {
            return nullptr;
        }
        return targetDef->getAppearance();
    }

    std::shared_ptr<const Texture> Polygon::getTextureFor(const std::string& theme) const
    {
        const auto result = getTextureFor(theme, true);
        if (result != nullptr) {
            return result;
        }
        return getTextureFor(theme, false);
    }

    const std::vector<TVec2f> Polygon::getTexCoordsForTheme(const std::string& theme, bool front) const
    {
        auto& map = front ? m_themeToFrontTexCoordsMap : m_themeToBackTexCoordsMap;
        auto it = map.find(theme);

        if (it == map.end()) {
            return std::vector<TVec2f>();
        }

        if (it->second.size() != m_vertices.size()) {
            CITYGML_LOG_ERROR(m_logger, "Number of texture coordinates (" << it->second.size() << ") for theme "
                             << theme << " in polygon with id " << this->getId() << " does not match number of vertices (" << m_vertices.size()
                              << ").");
        }

        assert(it->second.size() == m_vertices.size());

        return it->second;
    }

    std::shared_ptr<const Texture> Polygon::getTextureForTheme(const std::string& theme, bool front) const
    {
        const auto targetDef = getTextureTargetDefinitionForTheme(theme, front);

        if (targetDef == nullptr) {
            return nullptr;
        }

        return targetDef->getAppearance();
    }


    TVec3d Polygon::computeNormal()
    {
        if ( m_exteriorRing == nullptr ) return TVec3d();

        TVec3d normal = m_exteriorRing->computeNormal();

        return m_negNormal ? -normal : normal;
    }

    bool Polygon::negNormal() const
    {
        return m_negNormal;
    }

    void Polygon::setNegNormal(bool negNormal)
    {
        m_negNormal = negNormal;
    }

    void Polygon::removeDuplicateVerticesInRings(std::shared_ptr<CityGMLLogger> logger) {
        std::vector<TextureTargetDefinition*> texTargetDefinitions = this->getTextureTargetDefinitions();

        // mergeRings should be done before merging polygons... hence m_exteriorRings should only contain one object
        if ( m_exteriorRing != nullptr )
        {
            m_exteriorRing->removeDuplicateVertices( texTargetDefinitions, logger );
        }

        for ( std::unique_ptr<LinearRing>& ring : m_interiorRings )
        {
            ring->removeDuplicateVertices( texTargetDefinitions, logger );
        }
    }

    std::vector<TVec2f> Polygon::getTexCoordsForRingAndTheme(const LinearRing& ring, const std::string& theme, bool front) {

        const auto targetDef = getTextureTargetDefinitionForTheme(theme, front);

        if (targetDef == nullptr) {
            return std::vector<TVec2f>();
        }

        const auto coords = targetDef->getTextureCoordinatesForID(ring.getId());

        if (coords == nullptr || coords->getCoords().empty()) {
            return std::vector<TVec2f>();
        }

        return coords->getCoords();
    }

    std::vector<std::vector<TVec2f>> Polygon::getTexCoordListsForRing(const LinearRing& ring, const std::vector<std::string>& themesFront, const std::vector<std::string>& themesBack) {
        std::vector<std::vector<TVec2f>> texCoordsLists;

        for (const std::string& theme : themesFront) {
            texCoordsLists.push_back(getTexCoordsForRingAndTheme(ring, theme, true));
        }

        for (const std::string& theme : themesBack) {
            texCoordsLists.push_back(getTexCoordsForRingAndTheme(ring, theme, false));
        }

        return texCoordsLists;
    }

    void Polygon::createIndicesWithTesselation(Tesselator& tesselator, std::shared_ptr<CityGMLLogger> logger)
    {
        TVec3d normal = computeNormal();

        std::vector<std::string> themesFront = getAllTextureThemes(true);
        std::vector<std::string> themesBack = getAllTextureThemes(false);

        tesselator.init(normal);

        if (m_exteriorRing != nullptr) {

            tesselator.addContour( m_exteriorRing->getVertices(), getTexCoordListsForRing(*m_exteriorRing, themesFront, themesBack));
            m_exteriorRing->forgetVertices();
        }

        for ( std::unique_ptr<LinearRing>& ring : m_interiorRings )
        {
            tesselator.addContour( ring->getVertices(), getTexCoordListsForRing(*ring, themesFront, themesBack) );
            ring->forgetVertices();
        }

        tesselator.compute();
        m_vertices = tesselator.getVertices();
        m_indices = tesselator.getIndices();

        if (m_vertices.empty()) {
            return;
        }

        const std::vector<std::vector<TVec2f>>& texCoordLists = tesselator.getTexCoords();

        for (size_t i = 0; i < themesFront.size(); i++) {
            assert(texCoordLists.at(i).size() == m_vertices.size());
            m_themeToFrontTexCoordsMap[themesFront.at(i)] = texCoordLists.at(i);
        }

        for (size_t i = 0; i < themesBack.size(); i++) {
            assert(texCoordLists.at(i + themesFront.size()).size() == m_vertices.size());
            m_themeToBackTexCoordsMap[themesBack.at(i)] = texCoordLists.at(i + themesFront.size());
        }
    }

    void Polygon::computeIndices(Tesselator& tesselator, std::shared_ptr<CityGMLLogger> logger )
    {
        m_indices.clear();
        m_vertices.clear();

        createIndicesWithTesselation(tesselator, logger);

        if ( m_vertices.size() < 3 ) {
            CITYGML_LOG_WARN(logger, "Polygon with id " << this->getId() << " has less than 3 vertices.");
        }
    }

    void Polygon::finish(Tesselator& tesselator, bool optimize, std::shared_ptr<CityGMLLogger> logger)
    {
        if (m_finished) {
            // This may happen as Polygons can be shared between geometries
            return;
        }

        m_finished = true;

        if (optimize) {
            removeDuplicateVerticesInRings(logger);
        }

        computeIndices(tesselator, logger);

    }

    void Polygon::addRing( LinearRing* ring )
    {
        if (m_finished) {
            throw std::runtime_error("Can't add LinearRing to finished Polygon.");
        }

        if (ring->isExterior() && m_exteriorRing != nullptr) {
            CITYGML_LOG_WARN(m_logger, "Duplicate definition of exterior LinearRing for Polygon with id '" << this->getId() << "'."
                             << " Keeping exterior LinearRing with id '" << m_exteriorRing->getId() << "' and ignore LinearRing with id '" << ring->getId() << "'");
            delete ring;
            return;
        }

        if ( ring->isExterior() ) {
            m_exteriorRing = std::unique_ptr<LinearRing>(ring);
        }
        else {
            m_interiorRings.push_back( std::unique_ptr<LinearRing>(ring) );
        }
    }

    Polygon::~Polygon()
    {
    }

}
