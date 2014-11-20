#pragma once

#include <memory>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/cityobject.h>

namespace citygml {

    class Appearance;
    class AppearanceManager;
    class CityGMLLogger;

    typedef std::vector< CityObject* > CityObjects;
    typedef std::map< CityObject::CityObjectsType, CityObjects > CityObjectsMap;
    typedef std::map<std::string, AppearanceManager*>  AppearanceThemes;

    class LIBCITYGML_EXPORT CityModel : public Object
    {
    public:
        CityModel( const std::string& id = "CityModel");

         ~CityModel();

        // Return the envelope (ie. the bounding box) of the model
        const Envelope& getEnvelope() const;

        // Return the translation parameters of the model
        const TVec3d& getTranslationParameters() const;

        // Get the number of city objects
        unsigned int size() const;

        const CityObjectsMap& getCityObjectsMap() const;

        const CityObjects* getCityObjectsByType( CityObject::CityObjectsType type ) const;

        /**
         * @brief Return the roots elements of the model. You can then navigate the hierarchy using object->getChildren().
         */
        const CityObjects& getCityObjectsRoots() const;

        const std::string& getSRSName() const;

        AppearanceManager& getOrCreateAppearanceManager(const std::string& theme, std::shared_ptr<citygml::CityGMLLogger> logger);

        /**
         * @brief searches in all themes for the appearance
         * @param appearanceID the id of the appearance
         * @return the first appearance with matching id or nullptr if no such appearance was found
         */
        std::shared_ptr<Appearance> findAppearanceInAllThemes(const std::string& appearanceID);

        std::string getDefaultTheme() const;
        void reassignNodeToAllAppearances(Polygon* polygon, std::vector<std::string>& cityGMLidStack);

    protected:
       void addCityObject( CityObject* o );

       void addCityObjectAsRoot( CityObject* o ) { if ( o ) m_roots.push_back( o ); }

       void finish(const ParserParams& , std::shared_ptr<CityGMLLogger> logger);

    protected:
        Envelope m_envelope;

        CityObjects m_roots;

        CityObjectsMap m_cityObjectsMap;

        AppearanceThemes m_appearanceThemes;

        std::string m_srsName;

        TVec3d m_translation;
    };

    std::ostream& operator<<( std::ostream&, const citygml::CityModel & );
}
