#include <citygml/geometrymanager.h>

#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>
#include <citygml/citygmllogger.h>

namespace citygml {

    GeometryManager::GeometryManager(std::shared_ptr<CityGMLLogger> logger)
    {
        m_logger = logger;
    }

    void GeometryManager::addSharedGeometry(std::shared_ptr<Geometry> geom)
    {
        if (m_sharedGeometries.count(geom->getId()) > 0) {
            CITYGML_LOG_WARN(m_logger, "Duplicate definition of shared geometry with id '" << geom->getId() << "'... overwriting existing object.");
        }

        m_sharedGeometries[geom->getId()] = geom;
    }

    void GeometryManager::requestSharedGeometryForImplicitGeometry(ImplicitGeometry* geom, const std::string& geometryID)
    {
        m_geometryRequests.push_back(GeometryRequest(geom, geometryID));
    }

    void GeometryManager::finish()
    {
        CITYGML_LOG_INFO(m_logger, "Start processing shared geometry requests (" << m_geometryRequests.size() << ").");
        for (const GeometryRequest& request : m_geometryRequests) {

            auto it = m_sharedGeometries.find(request.geometryID);
            if (it == m_sharedGeometries.end()) {
                CITYGML_LOG_WARN(m_logger, "ImplicitGeometry object with id '" << request.target->getId() << "' requests Geometry with id '" << request.geometryID << "' but no such"
                                 << "shared Geometry object exists.");
                continue;
            }

            request.target->addGeometry(it->second);

        }

        m_sharedGeometries.clear();
        m_geometryRequests.clear();

        CITYGML_LOG_INFO(m_logger, "Finished processing shared geometry requests.");
    }

    GeometryManager::~GeometryManager()
    {

    }


}
