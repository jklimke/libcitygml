#pragma once

#include <citygml/citygml_api.h>
#include <citygml/cityobject.h>

namespace citygml {

    class CityGMLFactory;

#define DECLARE_SIMPLE_OBJECT_CLASS( _name_, _defcolor_ ) \
    class LIBCITYGML_EXPORT _name_ : public CityObject \
    {\
    friend class CityGMLFactory;\
    public:\
    inline TVec4f getDefaultColor(()) const { return _defcolor_; }\
    protected:\
    _name_( const std::string& id ) : CityObject( id, COT_ ## _name_ ) {}\
    }


    DECLARE_SIMPLE_OBJECT_CLASS( Building, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( BuildingPart, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Room, MAKE_RGB( 181, 180, 163 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Door, MAKE_RGB( 145, 53, 13 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Window, MAKE_RGBA( 147, 170, 209, 60 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( BuildingInstallation, MAKE_RGB( 186, 186, 177 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( BuildingFurniture, MAKE_RGB( 227, 225, 157 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( CityFurniture, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( WaterBody, MAKE_RGB( 48, 133, 187 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( PlantCover, MAKE_RGB( 0, 184, 0 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( SolitaryVegetationObject, MAKE_RGB( 10, 184, 10 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Track, MAKE_RGB( 171, 131, 46 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Road, MAKE_RGB( 159, 159, 159 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Railway, MAKE_RGB( 180, 180, 180 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Square, MAKE_RGB( 159, 159, 159 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( TINRelief, MAKE_RGB( 100, 230, 10 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Tunnel, MAKE_RGB( 180, 180, 150 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( Bridge, MAKE_RGB( 245, 30, 30 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( BridgeConstructionElement, MAKE_RGB( 245, 20, 20 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( BridgeInstallation, MAKE_RGB( 245, 80, 80 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( BridgePart, MAKE_RGB( 245, 50, 50 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( GenericCityObject, MAKE_RGB( 100, 130, 0 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( WallSurface, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( RoofSurface, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( GroundSurface, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( ClosureSurface, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( FloorSurface, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( InteriorWallSurface, MAKE_RGB( 186, 184, 135 ) );

    DECLARE_SIMPLE_OBJECT_CLASS( CeilingSurface, MAKE_RGB( 186, 184, 135 ) );

}
