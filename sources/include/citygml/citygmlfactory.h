#pragma once

#include "citygml/geometry.h"

#include <memory>

namespace citygml {

    class AppearanceManager;
    class CityGMLLogger;

    class CityModel;
    class AppearanceTarget;
    class CityObject;
    class Geometry;
    class ImplicitGeometry;
    class Polygon;

    class Texture;
    class GeoreferencedTexture;
    class Material;

    class MaterialTargetDefinition;
    class TextureTargetDefinition;

    class CityGMLFactory {
    public:
        CityGMLFactory(AppearanceManager* appearanceManager, std::shared_ptr<CityGMLLogger> logger);

        template<class T> T* createCityObject(const std::string& id) {
            T* cityObject = new T(id);
            appearanceTargetCreated(cityObject);
            return cityObject;
        }

        CityModel* createCityModel(const std::string& id);
        Geometry* createGeometry(const std::string& id, Geometry::GeometryType type = Geometry::GeometryType::GT_Unknown, unsigned int lod = 0);
        Polygon* createPolygon(const std::string& id);
        citygml::ImplicitGeometry* createImplictGeometry(const std::string& id);

        std::shared_ptr<Texture> createTexture(const std::string& id);
        std::shared_ptr<Material> createMaterial(const std::string& id);
        std::shared_ptr<GeoreferencedTexture> createGeoReferencedTexture(const std::string& id);

        std::shared_ptr<MaterialTargetDefinition> createMaterialTargetDefinition(const std::string& targetID, std::shared_ptr<Material> appearance, const std::string& id);
        std::shared_ptr<TextureTargetDefinition> createTextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id);
    protected:
        void appearanceTargetCreated(AppearanceTarget* obj);

        std::shared_ptr<CityGMLLogger> m_logger;
        AppearanceManager* m_appearanceManager;
    };

}
