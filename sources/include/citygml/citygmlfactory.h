#pragma once

#include "citygml/geometry.h"
#include "citygml/cityobject.h"

#include <memory>
#include <unordered_map>

namespace citygml {

    class AppearanceManager;
    class CityGMLLogger;

    class CityModel;
    class AppearanceTarget;
    class CityObject;
    class Geometry;
    class ImplicitGeometry;
    class Polygon;

    class Appearance;
    class Texture;
    class GeoreferencedTexture;
    class Material;

    class MaterialTargetDefinition;
    class TextureTargetDefinition;

    class CityGMLFactory {
    public:
        CityGMLFactory(std::shared_ptr<CityGMLLogger> logger);

        CityModel* createCityModel(const std::string& id);
        CityObject* createCityObject(const std::string& id, CityObject::CityObjectsType type);
        Geometry* createGeometry(const std::string& id, Geometry::GeometryType type = Geometry::GeometryType::GT_Unknown, unsigned int lod = 0);
        Polygon* createPolygon(const std::string& id);

        citygml::ImplicitGeometry* createImplictGeometry(const std::string& id);
        std::shared_ptr<Geometry> shareGeometry(Geometry* geom);
        std::shared_ptr<Geometry> getSharedGeometryWithID(const std::string& id);

        std::shared_ptr<Texture> createTexture(const std::string& id);
        std::shared_ptr<Material> createMaterial(const std::string& id);
        std::shared_ptr<GeoreferencedTexture> createGeoReferencedTexture(const std::string& id);

        std::shared_ptr<MaterialTargetDefinition> createMaterialTargetDefinition(const std::string& targetID, std::shared_ptr<Material> appearance, const std::string& id);
        std::shared_ptr<TextureTargetDefinition> createTextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id);

        std::shared_ptr<Appearance> getAppearanceWithID(const std::string& id);

        void closeFactory();

        ~CityGMLFactory();
    protected:
        void appearanceTargetCreated(AppearanceTarget* obj);

        std::unordered_map<std::string, std::shared_ptr<Geometry>> m_sharedGeometriesMap;
        std::shared_ptr<CityGMLLogger> m_logger;
        std::unique_ptr<AppearanceManager> m_appearanceManager;
    };

}
