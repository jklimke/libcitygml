#pragma once

#include <string>

#include <citygml/citygml_api.h>
#include <citygml/attributesmap.h>

namespace citygml {
    /**
     * @brief The base object associated with an unique id and a set of attributes (key-value pairs)
     */
    class LIBCITYGML_EXPORT Object
    {
    public:
        Object( const std::string& id );

        const std::string& getId() const;

        std::string getAttribute( const std::string& name ) const;

        const AttributesMap& getAttributes() const;

        AttributesMap& getAttributes();

        virtual ~Object() {}

        void setAttribute(const std::string& name, const std::string& value, AttributeType type = AttributeType::String, bool overwrite = true );

    protected:

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4251 4275)
#endif
        std::string m_id;

        AttributesMap m_attributes;
#ifdef _MSC_VER
#	pragma warning(pop)
#endif
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream&, const citygml::Object& );

}
