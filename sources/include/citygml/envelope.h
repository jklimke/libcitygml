#pragma once

#include <iostream>

#include <citygml/citygml_api.h>
#include <citygml/vecs.hpp>

namespace citygml {
    class LIBCITYGML_EXPORT Envelope
    {
    public:
        Envelope() {}

        Envelope( const TVec3d& lowerBound, const TVec3d& upperBound );

        const TVec3d& getLowerBound() const;
        const TVec3d& getUpperBound() const;

    protected:
        TVec3d m_lowerBound;
        TVec3d m_upperBound;
    };

    std::ostream& operator<<( std::ostream&, const citygml::Envelope& );
}
