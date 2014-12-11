#pragma once

#include <string>
#include <map>

#include <citygml/citygml_api.h>

namespace citygml {

    typedef std::map< std::string, std::string > AttributesMap;

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

        void setAttribute(const std::string& name, const std::string& value, bool overwrite = true );

    protected:

        std::string m_id;

        AttributesMap m_attributes;
    };

    std::ostream& operator<<( std::ostream&, const citygml::Object& );

}
