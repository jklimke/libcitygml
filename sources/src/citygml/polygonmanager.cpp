#include "citygml/polygonmanager.h"

#include "citygml/polygon.h"
#include "citygml/citygmllogger.h"
#include "citygml/geometry.h"

namespace citygml {

    PolygonManager::PolygonManager(std::shared_ptr<CityGMLLogger> logger)
    {
        m_logger = logger;
    }

    void PolygonManager::addPolygon(std::shared_ptr<Polygon> poly)
    {
        if (m_sharedPolygons.count(poly->getId()) > 0) {
            CITYGML_LOG_WARN(m_logger, "Duplicate definition of Polygon with id '" << poly->getId() << "'... overwriting existing object.");
        }

        m_sharedPolygons[poly->getId()] = poly;
    }

    void PolygonManager::requestSharedPolygonForGeometry(Geometry* geom, const std::string& polygonID)
    {
        m_polygonRequests.push_back(PolygonRequest(geom, polygonID));
    }

    void PolygonManager::finish()
    {
        CITYGML_LOG_INFO(m_logger, "Start processing polygon requests (" << m_polygonRequests.size() << ").");
        for (const PolygonRequest& request : m_polygonRequests) {

            auto it = m_sharedPolygons.find(request.polygonID);
            if (it == m_sharedPolygons.end()) {
                CITYGML_LOG_WARN(m_logger, "Geometry object with id '" << request.target->getId() << "' requests Polygon with id '" << request.polygonID << "' but no such"
                                 << " Polygon object exists.");
                continue;
            }

            request.target->addPolygon(it->second);

        }

        m_sharedPolygons.clear();
        m_polygonRequests.clear();

        CITYGML_LOG_INFO(m_logger, "Finished processing polygon requests.");
    }

    PolygonManager::~PolygonManager()
    {

    }


}
