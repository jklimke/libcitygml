#pragma once

#include <citygml/object.h>
#include <citygml/warnings.h>


namespace citygml {
    class LIBCITYGML_EXPORT ExternalObjectReference {
    public:
        ExternalObjectReference() = default;
        ~ExternalObjectReference() = default;

        const std::string& getName() const;
        const std::string& getUri() const;
        void setName(const std::string& name);
        void setUri(const std::string& uri);

    private:
        PRAGMA_WARN_DLL_BEGIN
        std::string value;
        PRAGMA_WARN_DLL_END

        enum class ObjectRefType { NAME, URI };
        ObjectRefType type;
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
