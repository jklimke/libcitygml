#include <citygml/object.h>

#include <sstream>
#include <iostream>

namespace citygml {

    Object::Object(const std::string& id) : m_id( id )
    {
        if ( m_id == "" )
        {
         std::stringstream ss;
         ss << "PtrId_" << this;
         m_id = ss.str();
        }
    }

    const std::string&Object::getId() const
    {
        return m_id;
    }

    std::string Object::getAttribute(const std::string& name) const
    {
        std::map< std::string, std::string >::const_iterator elt = m_attributes.find( name );
        return elt != m_attributes.end() ? elt->second : "";
    }

    const AttributesMap&Object::getAttributes() const
    {
        return m_attributes;
    }

    AttributesMap&Object::getAttributes()
    {
        return m_attributes;
    }

    void Object::setAttribute(const std::string& name, const std::string& value, bool overwrite)
    {
        if ( !overwrite )
        {
            if ( m_attributes.count(name) > 1 ) return;
        }
        m_attributes[ name ] = value;
    }

    std::ostream& operator<<( std::ostream& os, const Object& o )
    {
        return os << o.getId();
    }

}
