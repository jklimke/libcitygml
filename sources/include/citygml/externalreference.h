#pragma once

#include <citygml/object.h>


namespace citygml {
    union LIBCITYGML_EXPORT ExternalObjectReference {
#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4251 4275)
#endif
        std::string name;
        std::string uri;
#ifdef _MSC_VER
#	pragma warning(pop)
#endif
        
        ExternalObjectReference();
        ~ExternalObjectReference();
    };

    class LIBCITYGML_EXPORT ExternalReference: public Object {
        friend class CityGMLFactory;
        
    protected:
        ExternalReference(std::string const& id);
//        ~ExternalReference() noexcept override;                    // Destructor
    public:
#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4251 4275)
#endif
        std::string informationSystem;
#ifdef _MSC_VER
#	pragma warning(pop)
#endif
        ExternalObjectReference externalObject;
    };
}
