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

        /**
         * @brief gets the citygml id attribute value or if not present generates a value based on the document location
         * @return the value of the gml::id attribute or "genID_<CityGMLFileName>_<LineNumber>_<ColumnNumber>" if there is no id attribute
         */
        std::string getCityGMLIDAttribute() const;

        bool hasXLinkAttribute() const;
        std::string getXLinkValue();

    protected:
        Attributes(std::shared_ptr<CityGMLLogger> logger);
        std::shared_ptr<CityGMLLogger> m_logger;
    };

}
