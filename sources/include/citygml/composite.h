#pragma once

#include <citygml/object.h>

namespace citygml {
    class Composite : public Object
    {
    public:
        Composite( const std::string& id, unsigned int lod = 0 );

        inline unsigned int getLOD() const;

    protected:
        unsigned int m_lod;
    };
}
