#pragma once

#include <citygml/object.h>
#include <citygml/cityobject.h>

namespace citygml {

    class LandUse : public CityObject
    {
    public:
        LandUse( const std::string& id );

        TVec4f getDefaultColor() const;
    };

}
