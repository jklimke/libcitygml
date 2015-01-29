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
    class CityGMLFactory;
    class CityGMLLogger;

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
        unsigned int getPolygonsCount() const;
        Polygon& getPolygon( unsigned int i );
        const Polygon& getPolygon( unsigned int i ) const;

        unsigned int getGeometriesCount() const;
        const Geometry& getGeometry( unsigned int i ) const;
        Geometry& getGeometry( unsigned int i );
        void addGeometry(Geometry* geom);

        GeometryType getType() const;

        unsigned int lod() const;
        void setLod(unsigned int lod);

        void addPolygon(std::shared_ptr<Polygon> );

        /**
         * @brief finishes the geometry by finishing its child polygons after broadcasting its appearances to all child polygons
         * @param tesselate determines wether the polygons are tesselated
         * @param tesselator the tesselator to be used for tesselation
         * @param mergePolygons determines wether all polygons are merged into one
         */
        void finish(Tesselator& tesselator, bool optimize, std::shared_ptr<CityGMLLogger> logger);

        ~Geometry();


    protected:
        Geometry( const std::string& id, GeometryType type = GeometryType::GT_Unknown, unsigned int lod = 0 );

        bool m_finished;

        GeometryType m_type;

        unsigned int m_lod;

        std::vector<std::shared_ptr<Geometry>> m_childGeometries;

        std::vector<std::shared_ptr<Polygon>> m_polygons;
    };

    std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s );

}
