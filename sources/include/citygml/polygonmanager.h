#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace citygml {

    class Polygon;
    class Geometry;
    class CityGMLLogger;

    class PolygonManager {
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

        std::shared_ptr<CityGMLLogger> m_logger;
        std::vector<PolygonRequest> m_polygonRequests;
        std::unordered_map<std::string, std::shared_ptr<Polygon> > m_sharedPolygons;
    };

}
