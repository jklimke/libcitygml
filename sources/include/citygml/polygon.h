#pragma once

#include <vector>
#include <unordered_map>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/vecs.hpp>
#include <citygml/linearring.h>
#include <citygml/geometry.h>
#include <citygml/appearancemanager.h>

class Tesselator;

namespace citygml {

    class TextureCoordinates;

    class LIBCITYGML_EXPORT Polygon : public Object
    {
    public:
        enum class AppearanceSide {
            FRONT,
            BACK
        };

        Polygon( const std::string& id );

        // Get the vertices
        const std::vector<TVec3d>& getVertices() const;

        // Get the indices
        const std::vector<unsigned int>& getIndices() const;

        // Get the normals
        const std::vector<TVec3f>& getNormals() const;

        // Get texture coordinates
        const std::vector<TVec2f>& getTexCoordsForTheme(const std::string& theme) const;

        const std::vector<std::shared_ptr<Appearance>> getAppearancesForTheme(const std::string& theme) const;

        // NOTE: appearances for m_geometry also target this polygon in the old implementation (only if the polygon has no appearance itself)
        void addAppearance(std::shared_ptr<Appearance> appearance);

        virtual ~Polygon();

    protected:
        void finish(bool doTesselate, Tesselator& tesselator );

        void addRing( LinearRing* );

        void computeIndices(const TVec3d& normal, bool tesselate, Tesselator& tesselator);
        void mergeRings();

        TVec3d computeNormal();

        bool merge( Polygon* );

        std::vector<TVec3d> m_vertices;
        std::vector<TVec3f> m_normals;
        std::vector<unsigned int> m_indices;
        std::unordered_map<std::string,std::vector<TVec2f>> m_themeTexCoordsMap;

        LinearRing* m_exteriorRing;
        std::vector<LinearRing*> m_interiorRings;

        std::vector<std::shared_ptr<Appearance>> m_appearances;

        bool m_negNormal;

        Geometry *m_geometry;
    };
}
