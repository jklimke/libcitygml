#pragma once

#include <memory>
#include <vector>
#include <map>

#include <citygml/citygml_api.h>
#include <citygml/cityobject.h>
#include <citygml/featureobject.h>

namespace citygml {

    class AppearanceManager;
    class AppearanceTarget;
    class CityGMLLogger;
    class CityObject;
    class CityGMLFactory;

    typedef std::vector<std::unique_ptr<CityObject> > CityObjects;
    typedef std::vector<const CityObject*> ConstCityObjects;
    typedef std::map< CityObject::CityObjectsType, std::vector<const CityObject*> > CityObjectsMap;
    typedef std::map<std::string, const CityObject*> IdToCityObjMap;

    class LIBCITYGML_EXPORT CityModel : public FeatureObject
    {
        friend class CityGMLFactory;
    public:

        /**
        * @brief Return the roots elements of the model.
        */
        const ConstCityObjects getRootCityObjects() const;
        void addRootObject(CityObject* obj);

        unsigned int getNumRootCityObjects() const;
        CityObject& getRootCityObject(int i);
        const CityObject& getRootCityObject(int i) const;

        const ConstCityObjects getAllCityObjectsOfType( CityObject::CityObjectsType type ) const;

        /**
         * @brief Return a CityObject from ID. If not found, return nullptr.
         */
        const CityObject* getCityObjectById(const std::string& id ) const;

        const std::string& getSRSName() const;

        void finish(Tesselator& tesselator, bool optimize, bool tesselate, std::shared_ptr<CityGMLLogger> logger);

        std::vector<std::string> themes() const;
        void setThemes(std::vector<std::string> themes);

        ~CityModel();

        const std::string& getGmlPath() const;
        void setGmlPath(const std::string& gmlPath);

    protected:

        CityModel( const std::string& id = "CityModel");

        void addToCityObjectsMapRecursive(const CityObject* cityObj);

        void addToIdToCityObjMapRecursive(const CityObject* cityObj);

        CityObjects m_roots;

        CityObjectsMap m_cityObjectsMap;

        IdToCityObjMap m_idToCityObjMap;

        std::string m_srsName;

        std::vector<std::string> m_themes;

        std::string m_gmlPath;
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream&, const citygml::CityModel & );
}
