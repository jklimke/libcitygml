#include "parser/attributes.h"
#include "parser/parserutils.hpp"
#include "parser/documentlocation.h"

#include "citygml/citygmllogger.h"

#include "sstream"

namespace citygml {

    Attributes::Attributes(std::shared_ptr<CityGMLLogger> logger)
    {
        m_logger = logger;
    }

    std::string Attributes::getCityGMLIDAttribute() const
    {
        std::string id = getAttribute("gml:id", "");
        if (id.empty()) {
            std::stringstream defaultID;
            defaultID << "genID_" << getDocumentLocation().getDocumentFileName() << "_" << getDocumentLocation().getCurrentLine() << "_" << + getDocumentLocation().getCurrentColumn();
            id = defaultID.str();
        }
        return id;
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
