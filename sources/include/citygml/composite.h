#pragma once

#include <citygml/appearancetarget.h>

namespace citygml {
    class CityGMLFactory;

    class Composite : public AppearanceTarget
    {
        friend class CityGMLFactory;
    public:
        inline unsigned int getLOD() const;

    protected:
        Composite( const std::string& id, unsigned int lod = 0 );

        unsigned int m_lod;
    };
}
