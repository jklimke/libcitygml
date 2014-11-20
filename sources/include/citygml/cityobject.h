#pragma once

#include <vector>

#include <citygml/object.h>
#include <citygml/vecs.hpp>
#include <citygml/envelope.h>

#define MAKE_RGBA( _r_, _g_, _b_, _a_ ) TVec4f( _r_/255.f, _g_/255.f, _b_/255.f, _a_/255.f )
#define MAKE_RGB( _r_, _g_, _b_ ) MAKE_RGBA( _r_, _g_, _b_, 255 )

namespace citygml {

    class ParserParams;
    class Geometry;
    class ImplicitGeometry;
    class Composite;
    class AppearanceManager;

    class CityObject : public Object
    {
    public:

        enum CityObjectsType {
            COT_GenericCityObject           = 1 << 0,
            COT_Building                    = 1 << 1,
            COT_Room                        = 1 << 2,
            COT_BuildingInstallation        = 1 << 3,
            COT_BuildingFurniture           = 1 << 4,
            COT_Door                        = 1 << 5,
            COT_Window                      = 1 << 6,
            COT_CityFurniture               = 1 << 7,
            COT_Track                       = 1 << 8,
            COT_Road                        = 1 << 9,
            COT_Railway                     = 1 << 10,
            COT_Square                      = 1 << 11,
            COT_PlantCover                  = 1 << 12,
            COT_SolitaryVegetationObject    = 1 << 13,
            COT_WaterBody                   = 1 << 14,
            COT_TINRelief                   = 1 << 15,
            COT_LandUse                     = 1 << 16,
            COT_Tunnel						= 1 << 17,
            COT_Bridge						= 1 << 18,
            COT_BridgeConstructionElement	= 1 << 19,
            COT_BridgeInstallation			= 1 << 20,
            COT_BridgePart                  = 1 << 21,
            COT_BuildingPart                = 1 << 22,

            COT_WallSurface                 = 1 << 23,
            COT_RoofSurface                 = 1 << 24,
            COT_GroundSurface               = 1 << 25,
            COT_ClosureSurface              = 1 << 26,
            COT_FloorSurface                = 1 << 27,
            COT_InteriorWallSurface         = 1 << 28,
            COT_CeilingSurface              = 1 << 29,

            COT_All                         = 0xFFFFFFFF
        };

        CityObject( const std::string& id, CityObjectsType type );

        // Get the object type
        CityObjectsType getType() const;

        std::string getTypeAsString() const;

        // Return the envelope (ie. the bounding box) of the object
        const Envelope& getEnvelope() const;

        // Get the default diffuse color of this object class
        virtual TVec4f getDefaultColor() const = 0;

        // Get the number of geometries contains in the object
        unsigned int size() const;

        // Access the geometries
        const Geometry* getGeometry( unsigned int i ) const;

        // Access the composites
        const Composite* getComposite( unsigned int i ) const;

        // Get the number of implicit geometries contains in the object
        unsigned int getImplicitGeometryCount() const;

        // Access the implicit geometries
        const ImplicitGeometry* getImplicitGeometry( unsigned int i ) const;

        // Access the children
        unsigned int getChildCount() const;

        const CityObject* getChild( unsigned int i ) const;

        CityObject* getChild( unsigned int i );

        const std::vector< CityObject* >& getChildren() const;

        std::vector< CityObject* >& getChildren();

        virtual ~CityObject();

    protected:
       void finish( AppearanceManager&, const ParserParams& );

    protected:
        CityObjectsType m_type;

        Envelope m_envelope;

        std::vector<Geometry*> m_geometries;
        std::vector<Composite*> m_composites;
        std::vector<ImplicitGeometry*> m_implicitGeometries;
        std::vector<CityObject*> m_children;
    };

    typedef unsigned int CityObjectsTypeMask;
    CityObjectsTypeMask getCityObjectsTypeMaskFromString( const std::string& stringMask );

    std::ostream& operator<<( std::ostream& os, const CityObject& o );
}
