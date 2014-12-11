#include "citygml/envelope.h"

#include <numeric>
#include <cmath>

namespace citygml {


    Envelope::Envelope()
    {
        m_lowerBound = TVec3d(std::nan(""),std::nan(""),std::nan(""));
        m_upperBound = TVec3d(std::nan(""),std::nan(""),std::nan(""));
    }

    Envelope::Envelope(const std::string& srsName)
    {
        m_srsName = srsName;
    }

    const TVec3d& Envelope::getLowerBound() const
    {
        return m_lowerBound;
    }

    void Envelope::setLowerBound(const TVec3d& coordinate)
    {
        m_lowerBound = coordinate;
    }

    const TVec3d& Envelope::getUpperBound() const
    {
        return m_upperBound;
    }

    void Envelope::setUpperBound(const TVec3d& coordinate)
    {
        m_upperBound = coordinate;
    }

    const std::string& Envelope::srsName() const
    {
        return m_srsName;
    }

    const bool Envelope::validBounds() const
    {
        return !(std::isnan(m_lowerBound[0]) ||  std::isnan(m_lowerBound[1]) || std::isnan(m_lowerBound[2])
                || std::isnan(m_upperBound[0]) ||  std::isnan(m_upperBound[1]) || std::isnan(m_upperBound[2]));
    }

    std::ostream& operator<<( std::ostream& os, const Envelope& e )
    {
        return os << e.getLowerBound() << " " << e.getUpperBound();
    }

}
