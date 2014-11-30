#include "citygml/envelope.h"

#include <numeric>

namespace citygml {


    Envelope::Envelope()
    {

    }

    Envelope::Envelope(const std::string& srsName)
    {
        m_srsName = srsName;
    }

    const TVec3d&Envelope::getLowerBound() const
    {
        return m_lowerBound;
    }

    void Envelope::setLowerBound(const TVec3d& coordinate)
    {
        m_lowerBound = coordinate;
    }

    const TVec3d&Envelope::getUpperBound() const
    {
        return m_upperBound;
    }

    void Envelope::setUpperBound(const TVec3d& coordinate)
    {
        m_upperBound = coordinate;
    }

    const std::string&Envelope::srsName() const
    {
        return m_srsName;
    }

    std::ostream& operator<<( std::ostream& os, const Envelope& e )
    {
        return os << e.getLowerBound() << " " << e.getUpperBound();
    }

}
