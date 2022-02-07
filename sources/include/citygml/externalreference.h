#pragma once

#include <citygml/object.h>


namespace citygml {
    union LIBCITYGML_EXPORT ExternalObjectReference {
        std::string name;
        std::string uri;
        
        ExternalObjectReference();
        ~ExternalObjectReference();
    };

    class LIBCITYGML_EXPORT ExternalReference: public Object {
        friend class CityGMLFactory;
        
    protected:
        ExternalReference(std::string const& id);
//        ~ExternalReference() noexcept override;                    // Destructor
    public:
        std::string informationSystem;
        ExternalObjectReference externalObject;
    };
}
