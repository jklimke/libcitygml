#pragma once

#include <citygml/object.h>
#include <citygml/cityobject.h>

namespace citygml {

    class CityGMLFactory;

    class LandUse : public CityObject
    {
        friend class CityGMLFactory;
    public:
        TVec4f getDefaultColor() const;

    protected:
        LandUse( const std::string& id );
    };

}
