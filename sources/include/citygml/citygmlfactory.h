#pragma once

#include "citygml/geometry.h"

#include <memory>

namespace citygml {

    class AppearanceManager;
    class CityGMLLogger;

    class AppearanceTarget;
    class CityObject;
    class Geometry;
    class Composite;
    class ImplictGeometry;
    class Polygon;

    class Texture;
    class GeoReferencedTexture;
    class Material;

    class CityGMLFactory {
    public:
        CityGMLFactory(AppearanceManager& appearanceManager, std::shared_ptr<CityGMLLogger> logger);

        template<class T> T* createCityObject(const std::string& id) {
            T* cityObject = new T(id);
            appearanceTargetCreated(cityObject);
            return cityObject;
        }

        Geometry* createGeometry(const std::string& id, Geometry::GeometryType type = Geometry::GeometryType::GT_Unknown, unsigned int lod = 0);
        ImplictGeometry* createImplictGeometry(const std::string& id);
        Composite* createComposite(const std::string& id, unsigned int lod = 0);
        Polygon* createPolygon(const std::string& id);

        std::shared_ptr<Texture> createTexture(const std::string& id);
        std::shared_ptr<Material> createMaterial(const std::string& id);
        std::shared_ptr<GeoReferencedTexture> createGeoReferencedTexture(const std::string& id);

    protected:
        void appearanceTargetCreated(AppearanceTarget& obj);

        std::shared_ptr<CityGMLLogger> m_logger;
        AppearanceManager& m_appearanceManager;
    };

}
