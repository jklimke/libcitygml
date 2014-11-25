#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include <citygml/citygml_api.h>
#include <citygml/appearancetarget.h>

class Tesselator;

namespace citygml {

    class Polygon;
    class AppearanceManager;
    class Appearance;
    class ParserParams;
    class Composite;
    class CityGMLFactory;

    class LIBCITYGML_EXPORT Geometry : public AppearanceTarget
    {
        friend class CityGMLFactory;
    public:
        enum class GeometryType
        {
            GT_Unknown,
            GT_Roof,
            GT_Wall,
            GT_Ground,
            GT_Closure,
            GT_Floor,
            GT_InteriorWall,
            GT_Ceiling,
        };

        // Get the geometry LOD
        unsigned int getLOD() const;

        // Get the polygons
        unsigned int size() const;
        Polygon& operator[]( unsigned int i );
        const Polygon& operator[]( unsigned int i ) const;

        GeometryType getType() const;

        const Composite* getComposite() const;
        void setComposite(Composite* composite);

        unsigned int lod() const;
        void setLod(unsigned int lod);

        void addPolygon( Polygon* );

        bool merge( Geometry* );

        /**
         * @brief finishes the geometry by finishing its child polygons after broadcasting its appearances to all child polygons
         * @param tesselate determines wether the polygons are tesselated
         * @param tesselator the tesselator to be used for tesselation
         * @param mergePolygons determines wether all polygons are merged into one
         */
        void finish(bool tesselate, Tesselator& tesselator, bool mergePolygons);

        ~Geometry();


    protected:
        Geometry( const std::string& id, GeometryType type = GeometryType::GT_Unknown, unsigned int lod = 0 );

        bool m_finished;

        GeometryType m_type;

        unsigned int m_lod;

        std::vector<std::unique_ptr<Polygon>> m_polygons;

        std::unique_ptr<Composite> m_composite;
    };

    std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s );

}
