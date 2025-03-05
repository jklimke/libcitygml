#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <citygml/citygml_api.h>
#include <citygml/warnings.h>

namespace citygml {

    class ImplicitGeometry;
    class Geometry;
    class CityGMLLogger;

    class LIBCITYGML_EXPORT GeometryManager {
    public:
        GeometryManager(std::shared_ptr<CityGMLLogger> logger);

        void addSharedGeometry(std::shared_ptr<Geometry> geom);

        /**
         * @brief the Geometry with id geometryID will be added to geom when finished is called
         * @param geom the ImplicitGeometry object to which the Geometry object will be added
         * @param geometryID the id of the Geometry
         */
        void requestSharedGeometryForImplicitGeometry(ImplicitGeometry* geom, const std::string& geometryID);

        void finish();

        ~GeometryManager();
    private:
        struct GeometryRequest {
            GeometryRequest(ImplicitGeometry* target, std::string geometryID) : target(target), geometryID(geometryID) {}
            ImplicitGeometry* target;
            std::string geometryID;
        };

        PRAGMA_WARN_DLL_BEGIN
        std::shared_ptr<CityGMLLogger> m_logger;
        std::vector<GeometryRequest> m_geometryRequests;
        std::unordered_map<std::string, std::shared_ptr<Geometry> > m_sharedGeometries;
        PRAGMA_WARN_DLL_END
    };

}
