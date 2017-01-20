#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <citygml/citygml_api.h>
#include <citygml/appearancetarget.h>
#include <citygml/vecs.hpp>
#include <citygml/linearring.h>
#include <citygml/geometry.h>

class Tesselator;

namespace citygml {

    class TextureCoordinates;
    class CityGMLFactory;
    class Texture;
    class Material;

    /**
     * @brief The Polygon class implements the functionality of gml::Polygon and gml::SurfacePatch (gml::Rectangle, gml::Triangle) objects
     */
    class LIBCITYGML_EXPORT Polygon : public AppearanceTarget
    {
        friend class CityGMLFactory;
    public:
        enum class AppearanceSide {
            FRONT,
            BACK
        };

        // Get the vertices
        const std::vector<TVec3d>& getVertices() const;
        std::vector<TVec3d>& getVertices();

        // Get the indices
        const std::vector<unsigned int>& getIndices() const;

        /**
         * @brief returns the material of this polygon for the given theme and side
         * @param theme a name of an appearance theme
         * @param front determines for which side the material should be returned (true = front side, false = backside)
         * @return a Material object or nullptr if there is no material for the theme and side
         */
        std::shared_ptr<const Material> getMaterialFor(const std::string& theme, bool front) const;

        /**
         * @brief returns the material of this polygon for the given theme. Prefers front side materials over back side materials
         * @param theme a name of an appearance theme
         * @return a Material object or nullptr if there is no material for the theme
         */
        std::shared_ptr<const Material> getMaterialFor(const std::string& theme) const;

        /**
         * @brief returns the texture of this polygon for the given theme and side
         * @param theme a name of an appearance theme
         * @param front determines for which side the texture should be returned (true = front side, false = backside)
         * @return a Texture object or nullptr if there is no texture for the theme and side
         */
        std::shared_ptr<const Texture> getTextureFor(const std::string& theme, bool front) const;

        /**
         * @brief returns the texture of this polygon for the given theme. Prefers front side textures over back side textures
         * @param theme a name of an appearance theme
         * @return a Texture object or nullptr if there is no texture for the theme
         */
        std::shared_ptr<const Texture> getTextureFor(const std::string& theme) const;

        /**
         * @brief returns the texture coordinates for the given theme and side
         * @param theme a name of an appearance theme
         * @param front determines for which side the texture coordinates should be returned (true = front side, false = backside)
         * @return the texture coordinates or an empty list if there are no texture coordinates for this theme and side
         */
        const std::vector<TVec2f> getTexCoordsForTheme(const std::string& theme, bool front) const;

        bool negNormal() const;
        void setNegNormal(bool negNormal);

        void addRing( LinearRing* );

        void finish(Tesselator& tesselator , bool optimize, std::shared_ptr<CityGMLLogger> logger);

        std::shared_ptr<LinearRing> exteriorRing(){
            return m_exteriorRing;
        }

        const std::shared_ptr<LinearRing> exteriorRing() const{
            return m_exteriorRing;
        }

        std::vector<std::shared_ptr<LinearRing> >& interiorRings() {
            return m_interiorRings;
        }

        const std::vector<std::shared_ptr<LinearRing> >& interiorRings() const{
            return m_interiorRings;
        }

        virtual ~Polygon();

    protected:
        Polygon( const std::string& id, std::shared_ptr<CityGMLLogger> logger );

        std::shared_ptr<const Texture> getTextureForTheme(const std::string& theme, bool front) const;

        /**
         * @brief fill the vertex array and creates a corresponding index array
         * @param tesselate if true the tesselator will be used to tesselate the linear rings
         * @param tesselator the Tesselator object
         */
        void computeIndices(Tesselator& tesselator, std::shared_ptr<CityGMLLogger> logger);
        void createSimpleIndices(std::shared_ptr<CityGMLLogger> logger);
        void createIndicesWithTesselation(Tesselator& tesselator, std::shared_ptr<CityGMLLogger> logger);
        void removeDuplicateVerticesInRings(std::shared_ptr<CityGMLLogger> logger);
        std::vector<TVec2f> getTexCoordsForRingAndTheme(const LinearRing& ring, const std::string& theme, bool front);
        std::vector<std::vector<TVec2f> > getTexCoordListsForRing(const LinearRing& ring, const std::vector<std::string>& themesFront, const std::vector<std::string>& themesBack);

        TVec3d computeNormal();

        std::vector<TVec3d> m_vertices;
        std::unordered_map<std::string, std::vector<TVec2f> > m_themeToFrontTexCoordsMap;
        std::unordered_map<std::string, std::vector<TVec2f> > m_themeToBackTexCoordsMap;
        std::vector<unsigned int> m_indices;

        std::shared_ptr<LinearRing> m_exteriorRing;
        std::vector<std::shared_ptr<LinearRing> > m_interiorRings;

        bool m_negNormal;
        bool m_finished;

        std::shared_ptr<CityGMLLogger> m_logger;
    };
}
