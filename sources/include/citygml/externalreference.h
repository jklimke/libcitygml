#pragma once

#include <citygml/object.h>
#include <citygml/warnings.h>


namespace citygml {
    union LIBCITYGML_EXPORT ExternalObjectReference {
        PRAGMA_WARN_DLL_BEGIN
        std::string name;
        std::string uri;
        PRAGMA_WARN_DLL_END
        
        ExternalObjectReference();
        ~ExternalObjectReference();
    };

    class LIBCITYGML_EXPORT ExternalReference: public Object {
        friend class CityGMLFactory;
        
    protected:
        ExternalReference(std::string const& id);
//        ~ExternalReference() noexcept override;                    // Destructor
    public:
        PRAGMA_WARN_DLL_BEGIN
        std::string informationSystem;
        PRAGMA_WARN_DLL_END
        ExternalObjectReference externalObject;
    };
}
