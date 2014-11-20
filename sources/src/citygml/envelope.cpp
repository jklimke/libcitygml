#include "citygml/envelope.h"

namespace citygml {

    Envelope::Envelope(const TVec3d& lowerBound, const TVec3d& upperBound)
    {
        m_lowerBound = lowerBound;
        m_upperBound = upperBound;
    }

    const TVec3d&Envelope::getLowerBound() const
    {
        return m_lowerBound;
    }

    const TVec3d&Envelope::getUpperBound() const
    {
        return m_upperBound;
    }

    std::ostream& operator<<( std::ostream& os, const Envelope& e )
    {
        return os << e.getLowerBound() << " " << e.getUpperBound();
    }

}
