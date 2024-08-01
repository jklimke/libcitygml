#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <citygml/citygml_api.h>

namespace citygml {

    class Polygon;
    class Geometry;
    class CityGMLLogger;

    class LIBCITYGML_EXPORT PolygonManager {
    public:
        PolygonManager(std::shared_ptr<CityGMLLogger> logger);

        void addPolygon(std::shared_ptr<Polygon> poly);

        /**
         * @brief the polygon with id polygonID will be added to geom when finished is called
         * @param geom the geometry object to which the polygon will be added
         * @param polygonID the id of the polygon
         */
        void requestSharedPolygonForGeometry(Geometry* geom, const std::string& polygonID);

        void finish();

        ~PolygonManager();
    private:
        struct PolygonRequest {
            PolygonRequest(Geometry* target, std::string polygonID) : target(target), polygonID(polygonID) {}
            Geometry* target;
            std::string polygonID;
        };

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4251 4275)
#endif
        std::shared_ptr<CityGMLLogger> m_logger;
        std::vector<PolygonRequest> m_polygonRequests;
        std::unordered_map<std::string, std::shared_ptr<Polygon> > m_sharedPolygons;
#ifdef _MSC_VER
#	pragma warning(pop)
#endif
    };

}
