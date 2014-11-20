#pragma once

#include <vector>

#include <citygml/citygml_api.h>
#include <citygml/object.h>

namespace citygml {

    class Polygon;
    class AppearanceManager;
    class Appearance;
    class ParserParams;
    class Composite;

    class LIBCITYGML_EXPORT Geometry : public Object
    {
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

        Geometry( const std::string& id, GeometryType type = GeometryType::GT_Unknown, unsigned int lod = 0 );

        // Get the geometry LOD
        unsigned int getLOD() const;

        // Get the polygons
        unsigned int size() const;
        Polygon* operator[]( unsigned int i );
        const Polygon* operator[]( unsigned int i ) const;

        GeometryType getType() const;

        Composite* getComposite() const;

        ~Geometry();

    protected:
        void addPolygon( Polygon* );

        void finish(AppearanceManager&, std::shared_ptr<Appearance>, const ParserParams& );

        bool merge( Geometry* );

    protected:
        bool m_finished;

        GeometryType m_type;

        unsigned int m_lod;

        std::vector< Polygon* > m_polygons;

        Composite* m_composite;
    };

    std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s );

}
