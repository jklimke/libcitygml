#include <citygml/address.h>

namespace citygml {

    Address::Address(const std::string& id)
    : Object(id)
    {
    }

    const std::string& Address::country() const {
        return m_country;
    }

    void Address::setCountry(const std::string& country) {
        m_country = country;
    }

    const std::string& Address::locality() const {
        return m_locality;
    }

    void Address::setLocality(const std::string& locality) {
        m_locality = locality;
    }

    const std::string& Address::postalCode() const {
        return m_postalCode;
    }

    void Address::setPostalCode(const std::string& postalCode) {
        m_postalCode = postalCode;
    }

    const std::string& Address::thoroughfareName() const {
        return m_thoroughfareName;
    }

    void Address::setThoroughfareName(const std::string& thoroughfareName) {
        m_thoroughfareName = thoroughfareName;
    }

    const std::string& Address::thoroughfareNumber() const {
        return m_thoroughfareNumber;
    }

    void Address::setThoroughfareNumber(const std::string& thoroughfareNumber) {
        m_thoroughfareNumber = thoroughfareNumber;
    }

} /* namespace citygml */
