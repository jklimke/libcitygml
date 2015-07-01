#pragma once

#include <citygml/citygml_api.h>
#include <citygml/object.h>

namespace citygml {

    class LIBCITYGML_EXPORT Address: public Object
    {
    public:
        Address(const std::string& id);

        const std::string& country() const;
        void setCountry(const std::string& country);

        const std::string& locality() const;
        void setLocality(const std::string& locality);

        const std::string& postalCode() const;
        void setPostalCode(const std::string& postalCode);

        const std::string& thoroughfareName() const;
        void setThoroughfareName(const std::string& thoroughfareName);

        const std::string& thoroughfareNumber() const;
        void setThoroughfareNumber(const std::string& thoroughfareNumber);

    protected:
        std::string m_country;
        std::string m_locality;
        std::string m_thoroughfareName;
        std::string m_thoroughfareNumber;
        std::string m_postalCode;
    };

} /* namespace citygml */
