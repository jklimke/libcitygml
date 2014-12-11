#include "parser/attributes.h"
#include "parser/parserutils.hpp"
#include "parser/documentlocation.h"

#include "citygml/citygmllogger.h"

namespace citygml {

    Attributes::Attributes(std::shared_ptr<CityGMLLogger> logger)
    {
        m_logger = logger;
    }

    std::string Attributes::getCityGMLIDAttribute() const
    {
        return getAttribute("gml:id", "");
    }

    bool Attributes::hasXLinkAttribute() const
    {
        return !getAttribute("xlink:href", "").empty();
    }

    std::string Attributes::getXLinkValue()
    {
        return parseReference(getAttribute("xlink:href", ""), m_logger, getDocumentLocation());
    }



}
