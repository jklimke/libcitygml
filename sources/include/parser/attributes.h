#pragma once

#include <string>
#include <memory>

namespace citygml {

    class CityGMLLogger;
    class DocumentLocation;

    /**
     * @brief The Attributes class provides methods to access the attributes of an xml element
     */
    class Attributes {
    public:

        /**
         * @brief get the value of an attribute
         * @param attname the name of the attribute
         * @param defvalue the default value
         * @return the value of the attribute or defvalue if no such attribute exists
         */
        virtual std::string getAttribute( const std::string& attname, const std::string& defvalue = "" ) const = 0;
        virtual const DocumentLocation& getDocumentLocation() const = 0;

        std::string getCityGMLIDAttribute() const;

        bool hasXLinkAttribute() const;
        std::string getXLinkValue();

    protected:
        Attributes(std::shared_ptr<CityGMLLogger> logger);
        std::shared_ptr<CityGMLLogger> m_logger;
    };

}
