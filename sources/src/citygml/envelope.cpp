#include <citygml/envelope.h>

#include <numeric>
#include <cmath>

#ifndef NAN
  #define NAN std::numeric_limits<double>::quiet_NaN()
#endif

#ifdef WIN32
  #define ISNAN _isnan
#else
  #define ISNAN std::isnan
#endif

namespace citygml {


    Envelope::Envelope()
    {
        m_lowerBound = TVec3d(NAN,NAN,NAN);
        m_upperBound = TVec3d(NAN,NAN,NAN);
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
        return !(ISNAN(m_lowerBound.x) ||  ISNAN(m_lowerBound.y) || ISNAN(m_lowerBound.z)
                || ISNAN(m_upperBound.x) ||  ISNAN(m_upperBound.y) || ISNAN(m_upperBound.z));
    }

    std::ostream& operator<<( std::ostream& os, const Envelope& e )
    {
        return os << e.getLowerBound() << " " << e.getUpperBound();
    }

}
