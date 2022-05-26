#include <citygml/cityobject.h>
#include <citygml/citygmlfactory.h>
#include <citygml/appearancemanager.h>
#include <citygml/polygonmanager.h>
#include <citygml/geometrymanager.h>
#include <citygml/appearancetarget.h>
#include <citygml/polygon.h>
#include <citygml/linestring.h>
#include <citygml/implictgeometry.h>
#include <citygml/texture.h>
#include <citygml/georeferencedtexture.h>
#include <citygml/material.h>
#include <citygml/materialtargetdefinition.h>
#include <citygml/texturetargetdefinition.h>
#include <citygml/citymodel.h>
#include <citygml/implictgeometry.h>
#include <citygml/citygmllogger.h>
#include <citygml/rectifiedgridcoverage.h>
#include <filesystem>

namespace citygml {

    CityGMLFactory::CityGMLFactory(std::shared_ptr<CityGMLLogger> logger)
    {
        m_appearanceManager = std::unique_ptr<AppearanceManager>(new AppearanceManager(logger));
        m_polygonManager = std::unique_ptr<PolygonManager>(new PolygonManager(logger));
        m_geometryManager = std::unique_ptr<GeometryManager>(new GeometryManager(logger));
        m_logger = logger;
        m_codeLists = std::shared_ptr<CodeLists>(new CodeLists);
    }

    RectifiedGridCoverage* CityGMLFactory::createRectifiedGridCoverage(std::string const& id) {
        return new RectifiedGridCoverage(id);
    }

    CityModel* CityGMLFactory::createCityModel(const std::string& id)
    {
        return new CityModel(id);
    }

    CityObject* CityGMLFactory::createCityObject(const std::string& id, CityObject::CityObjectsType type)
    {
        CityObject* cityObject = new CityObject(id, type);
        return cityObject;
    }

    Geometry::GeometryType mapCityObjectsTypeToGeometryType(const CityObject::CityObjectsType& cityObjType) {

        switch (cityObjType) {
        case CityObject::CityObjectsType::COT_RoofSurface:
            return Geometry::GeometryType::GT_Roof;
        case CityObject::CityObjectsType::COT_WallSurface:
            return Geometry::GeometryType::GT_Wall;
        case CityObject::CityObjectsType::COT_GroundSurface:
            return Geometry::GeometryType::GT_Ground;
        case CityObject::CityObjectsType::COT_ClosureSurface:
            return Geometry::GeometryType::GT_Closure;
        case CityObject::CityObjectsType::COT_FloorSurface:
            return Geometry::GeometryType::GT_Floor;
        case CityObject::CityObjectsType::COT_InteriorWallSurface:
            return Geometry::GeometryType::GT_InteriorWall;
        case CityObject::CityObjectsType::COT_CeilingSurface:
            return Geometry::GeometryType::GT_Ceiling;
        case CityObject::CityObjectsType::COT_OuterCeilingSurface:
            return Geometry::GeometryType::GT_OuterCeiling;
        case CityObject::CityObjectsType::COT_OuterFloorSurface:
            return Geometry::GeometryType::GT_OuterFloor;
        case CityObject::CityObjectsType::COT_TINRelief:
            return Geometry::GeometryType::GT_Tin;
        default:
            return Geometry::GeometryType::GT_Unknown;
        }

    }

    Geometry* CityGMLFactory::createGeometry(const std::string& id, const CityObject::CityObjectsType& cityObjType, unsigned int lod, std::string srsName)
    {
        Geometry* geom = new Geometry(id, mapCityObjectsTypeToGeometryType(cityObjType), lod, srsName);
        appearanceTargetCreated(geom);
        return geom;
    }

    std::shared_ptr<Polygon> CityGMLFactory::createPolygon(const std::string& id)
    {
        Polygon* poly = new Polygon(id, m_logger);
        appearanceTargetCreated(poly);

        std::shared_ptr<Polygon> shared = std::shared_ptr<Polygon>(poly);
        m_polygonManager->addPolygon(shared);

        return shared;
    }

    std::shared_ptr<LineString> CityGMLFactory::createLineString(const std::string& id)
    {
        LineString* lineString = new LineString(id);
        return std::shared_ptr<LineString>(lineString);
    }

    std::shared_ptr<ExternalReference> CityGMLFactory::createExternalReference(const std::string& id)
    {
        ExternalReference * externalReference = new ExternalReference(id);
        return std::shared_ptr<ExternalReference>(externalReference);
    }

    void CityGMLFactory::requestSharedPolygonForGeometry(Geometry* geom, const std::string& polygonId)
    {
        m_polygonManager->requestSharedPolygonForGeometry(geom, polygonId);
    }

    ImplicitGeometry *CityGMLFactory::createImplictGeometry(const std::string& id)
    {
        return new ImplicitGeometry(id);
    }

    std::shared_ptr<Geometry> CityGMLFactory::shareGeometry(Geometry* geom)
    {
        std::shared_ptr<Geometry> shared = std::shared_ptr<Geometry>(geom);

        m_geometryManager->addSharedGeometry(shared);

        return shared;
    }

    void CityGMLFactory::requestSharedGeometryWithID(ImplicitGeometry* implicitGeom, const std::string& id)
    {
        m_geometryManager->requestSharedGeometryForImplicitGeometry(implicitGeom, id);
    }

    std::shared_ptr<Texture> CityGMLFactory::createTexture(const std::string& id)
    {
        std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture(id));
        m_appearanceManager->addAppearance(tex);
        return tex;
    }

    std::shared_ptr<Material> CityGMLFactory::createMaterial(const std::string& id)
    {
        std::shared_ptr<Material> mat = std::shared_ptr<Material>(new Material(id));
        m_appearanceManager->addAppearance(mat);
        return mat;
    }

    std::shared_ptr<GeoreferencedTexture> CityGMLFactory::createGeoReferencedTexture(const std::string& id)
    {
        std::shared_ptr<GeoreferencedTexture> tex = std::shared_ptr<GeoreferencedTexture>(new GeoreferencedTexture(id));
        m_appearanceManager->addAppearance(tex);
        return tex;
    }

    std::shared_ptr<MaterialTargetDefinition> CityGMLFactory::createMaterialTargetDefinition(const std::string& targetID, std::shared_ptr<Material> appearance, const std::string& id)
    {
        std::shared_ptr<MaterialTargetDefinition> targetDef = std::shared_ptr<MaterialTargetDefinition>(new MaterialTargetDefinition(targetID, appearance, id));
        m_appearanceManager->addMaterialTargetDefinition(targetDef);
        return targetDef;
    }

    std::shared_ptr<TextureTargetDefinition> CityGMLFactory::createTextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id)
    {
        std::shared_ptr<TextureTargetDefinition> targetDef = std::shared_ptr<TextureTargetDefinition>(new TextureTargetDefinition(targetID, appearance, id));
        m_appearanceManager->addTextureTargetDefinition(targetDef);
        return targetDef;
    }

    std::shared_ptr<Appearance> CityGMLFactory::getAppearanceWithID(const std::string& id)
    {
        return m_appearanceManager->getAppearanceByID(id);
    }

    std::vector<std::string> CityGMLFactory::getAllThemes()
    {
        return m_appearanceManager->getAllThemes();
    }

    void CityGMLFactory::closeFactory()
    {
        m_polygonManager->finish();
        m_geometryManager->finish();
        m_appearanceManager->assignAppearancesToTargets();
    }

    CityGMLFactory::~CityGMLFactory()
    {

    }

    void CityGMLFactory::appearanceTargetCreated(AppearanceTarget* obj)
    {
        m_appearanceManager->addAppearanceTarget(obj);
    }

    std::string CityGMLFactory::getCodeValue(const std::string& codeSpace, const std::string& gmlPath, const std::string& id)
    {
        const auto codeSpacePath =
            std::filesystem::path(gmlPath)
            .remove_filename().append(codeSpace).string();

        if (m_codeLists->find(codeSpacePath) == m_codeLists->end()) {
            if (!std::filesystem::exists(codeSpacePath)) {
                CITYGML_LOG_ERROR(m_logger, "Can not find codelist file " << codeSpacePath);
                throw std::runtime_error("Can not find codelist file: " + codeSpacePath);
            }
            CITYGML_LOG_INFO(m_logger, "Parsing " << codeSpacePath);

            CodeListParser handler;
            const auto code_list = handler.parse(codeSpacePath);
            m_codeLists->emplace(codeSpacePath, code_list);
        }
        
        std::string codeValue;
        if (m_codeLists->find(codeSpacePath) != m_codeLists->end()) {
            const auto codeList = m_codeLists->find(codeSpacePath)->second;
            if (codeList.find(id) != codeList.end()) codeValue = codeList.find(id)->second;
        }

        return codeValue;
    }
}
