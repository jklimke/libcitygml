#pragma once

#include <memory>
#include <vector>
#include <unordered_set>

#include <citygml/citygml_api.h>
#include <citygml/appearancetarget.h>

class Tesselator;

namespace citygml {

    class LineString;
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
            GT_Unknown          = 1 << 0,
            GT_Roof             = 1 << 1,
            GT_Wall             = 1 << 2,
            GT_Ground           = 1 << 3,
            GT_Closure          = 1 << 4,
            GT_Floor            = 1 << 5,
            GT_InteriorWall     = 1 << 6,
            GT_Ceiling          = 1 << 7,
            GT_OuterCeiling     = 1 << 8,
            GT_OuterFloor       = 1 << 9,
            GT_Tin              = 1 << 10,
        };

        unsigned int getLOD() const;

        unsigned int getPolygonsCount() const;
        std::shared_ptr<Polygon> getPolygon( unsigned int i );
        std::shared_ptr<const Polygon> getPolygon( unsigned int i ) const;

        unsigned int getLineStringCount() const;
        std::shared_ptr<LineString> getLineString( unsigned int i );
        std::shared_ptr<const LineString> getLineString( unsigned int i ) const;

        unsigned int getGeometriesCount() const;
        const Geometry& getGeometry( unsigned int i ) const;
        Geometry& getGeometry( unsigned int i );
        void addGeometry(Geometry* geom);

        GeometryType getType() const;

        std::string getTypeAsString() const;

        unsigned int lod() const;
        void setLod(unsigned int lod);

        // Access the srs of the implicit geometry
        std::string getSRSName() const;
        void setSRSName(const std::string& srsName);

        void addPolygon(std::shared_ptr<Polygon> );
        void addLineString(std::shared_ptr<LineString>);

        /**
         * @brief finishes the geometry by finishing its child polygons after broadcasting its appearances to all child polygons
         * @param tesselate determines wether the polygons are tesselated
         * @param tesselator the tesselator to be used for tesselation
         * @param mergePolygons determines wether all polygons are merged into one
         */
        void finish(Tesselator& tesselator, bool optimize, std::shared_ptr<CityGMLLogger> logger);

        ~Geometry();


    protected:
        Geometry( const std::string& id, GeometryType type = GeometryType::GT_Unknown, unsigned int lod = 0, std::string srsName = "" );

        bool m_finished;

        GeometryType m_type;

        unsigned int m_lod;

        std::string m_srsName;

        std::vector<std::shared_ptr<Geometry> > m_childGeometries;

        std::vector<std::shared_ptr<Polygon> > m_polygons;
        std::vector<std::shared_ptr<LineString> > m_lineStrings;
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s );

}
