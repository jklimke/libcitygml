#include "citygml/citygmlfactory.h"
#include "citygml/appearancemanager.h"
#include "citygml/composite.h"
#include "citygml/cityobject.h"
#include "citygml/appearancetarget.h"
#include "citygml/polygon.h"
#include "citygml/implictgeometry.h"

namespace citygml {

    CityGMLFactory::CityGMLFactory(AppearanceManager& appearanceManager, std::shared_ptr<CityGMLLogger> logger)
    {
        m_appearanceManager = appearanceManager;
        m_logger = logger;
    }

    Geometry* CityGMLFactory::createGeometry(const std::string& id, GeometryType type, unsigned int lod)
    {
        Geometry* geom = new Geometry(id, type, lod);
        appearanceTargetCreated(geom);
        return geom;
    }

    ImplictGeometry* CityGMLFactory::createImplictGeometry(const std::string& id)
    {
        ImplicitGeometry* geom = new ImplicitGeometry(id);
        appearanceTargetCreated(geom);
        return geom;
    }

    Composite* CityGMLFactory::createComposite(const std::string& id, unsigned int lod)
    {
        Composite* comp = new Composite(id, lod);
        appearanceTargetCreated(comp);
        return comp;
    }

    Polygon* CityGMLFactory::createPolygon(const std::string& id)
    {
        Polygon* poly = new Polygon(id, m_logger);
        appearanceTargetCreated(poly);
        return poly;
    }

    std::shared_ptr<Texture> CityGMLFactory::createTexture(const std::string& id)
    {
        std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture(id));
        m_appearanceManager.addAppearance(tex);
        return tex;
    }

}
