#pragma once

#include <memory>
#include <vector>
#include <map>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/cityobject.h>

namespace citygml {

    class AppearanceManager;
    class AppearanceTarget;
    class CityGMLLogger;
    class CityObject;

    typedef std::vector<std::unique_ptr<CityObject>> CityObjects;
    typedef std::vector<std::unique_ptr<const CityObject>> ConstCityObjects;
    typedef std::map< CityObject::CityObjectsType, std::vector<const CityObject&> > CityObjectsMap;

    class LIBCITYGML_EXPORT CityModel : public Object
    {
    public:

        // Return the envelope (ie. the bounding box) of the model
        const Envelope& getEnvelope() const;

        void setEnvelope(Envelope e);

        /**
        * @brief Return the roots elements of the model.
        */
        const ConstCityObjects& getRootCityObjects() const;

        const ConstCityObjects& getAllCityObjectsOfType( CityObject::CityObjectsType type ) const;

        const std::string& getSRSName() const;

        /**
        * @brief returns the translation parameters of the model
        */
        const TVec3d& getTranslationParameters() const;
        void setTranslationParameters(const TVec3d& param);

        void finish(bool tesselate, Tesselator& tesselator, bool mergePolygons);


        ~CityModel();

    protected:

        CityModel( const std::string& id = "CityModel");

        void addToCityObjectsMapRecursive(const CityObject& cityObj);

        Envelope m_envelope;

        CityObjects m_roots;

        CityObjectsMap m_cityObjectsMap;

        std::string m_srsName;

        TVec3d m_translation;
    };

    std::ostream& operator<<( std::ostream&, const citygml::CityModel & );
}
