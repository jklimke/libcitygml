#include "citygml/landuse.h"

namespace citygml {

    LandUse::LandUse(const std::string& id) : CityObject( id, COT_LandUse )
    {

    }

    TVec4f LandUse::getDefaultColor() const
    {
        std::string c = getAttribute( "class" );
        if ( c != "" )
        {
            int cl = atoi( c.c_str() );
            switch ( cl )
            {
            case 1000: return MAKE_RGB( 150, 143, 134 );	// Settlement Area
            case 1100: return MAKE_RGB( 133, 83, 101 );		// Undeveloped Area
            case 2000: return MAKE_RGB( 159, 159, 159 );	// Traffic
            case 3000: return MAKE_RGB( 79, 212, 53 );		// Vegetation
            case 4000: return MAKE_RGB( 67, 109, 247 );		// Water
            }
        }
        return MAKE_RGB( 10, 230, 1 );
    }

}
