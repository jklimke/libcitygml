#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include <citygml/citygml_api.h>
#include <citygml/appearancetarget.h>
#include <citygml/vecs.hpp>
#include <citygml/linearring.h>
#include <citygml/geometry.h>
#include <citygml/appearancemanager.h>

class Tesselator;

namespace citygml {

    class TextureCoordinates;
    class CityGMLFactory;

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

        // Get the indices
        const std::vector<unsigned int>& getIndices() const;

        // Get the normals
        const std::vector<TVec3f>& getNormals() const;

        // Get texture coordinates
        const std::vector<TVec2f> getTexCoordsForTheme(const std::string& theme) const;

        bool negNormal() const;
        void setNegNormal(bool negNormal);

        void addRing( LinearRing* );

        void finish(bool doTesselate, Tesselator& tesselator );

        virtual ~Polygon();

    protected:
        Polygon( const std::string& id, std::shared_ptr<CityGMLLogger> logger );

        std::shared_ptr<const Texture> getTextureForTheme(const std::string& theme) const;

        void computeIndices(const TVec3d& normal, bool tesselate, Tesselator& tesselator);
        void mergeRings();

        TVec3d computeNormal();

        std::vector<TVec3d> m_vertices;
        std::vector<TVec3f> m_normals;
        std::vector<unsigned int> m_indices;

        std::vector<std::unique_ptr<LinearRing>> m_exteriorRings;
        std::vector<std::unique_ptr<LinearRing>> m_interiorRings;

        bool m_negNormal;
        bool m_finished;

        std::shared_ptr<CityGMLLogger> m_logger;
    };
}
