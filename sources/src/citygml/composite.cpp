#include "citygml/composite.h"

namespace citygml {

    Composite::Composite(const std::string& id, unsigned int lod) : Object( id ), m_lod( lod)
    {

    }

    unsigned int Composite::getLOD() const
    {
        return m_lod;
    }

}
