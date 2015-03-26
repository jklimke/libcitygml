#pragma once

#include <citygml/citygml_api.h>
#include <citygml/appearance.h>
#include <citygml/vecs.hpp>
#include <unordered_set>

namespace citygml {

    class CityGMLFactory;

    class LIBCITYGML_EXPORT Material : public Appearance
    {
        friend class CityGMLFactory;
    public:

        TVec3f getDiffuse() const;
        void setDiffuse(TVec3f diffuse);

        TVec3f getEmissive() const;
        void setEmissive(TVec3f emissive);

        TVec3f getSpecular() const;
        void setSpecular(TVec3f specular);

        float getAmbientIntensity() const;
        void setAmbientIntensity(float intensity);

        /**
         * @brief the shininess of the material
         * @return a value between 0 and 1, where 1 is the brightest intensity
         * @note openGL defines the shininess as a value beteen 0 and 128 with 128 beeing the brightest intensity
         */
        float getShininess() const;
        void setShininess(float shininess);

        float getTransparency() const;
        void setTransparency(float transparancy);

        bool isSmooth() const;
        void setIsSmooth(bool isSmooth);

        virtual std::shared_ptr<Material> asMaterial() override;
        virtual std::shared_ptr<const Material> asMaterial() const override;

    protected:
        Material( const std::string& id );
        TVec3f m_diffuse;
        TVec3f m_emissive;
        TVec3f m_specular;
        float m_ambientIntensity;
        float m_shininess;
        float m_transparency;
        bool m_isSmooth;
    };

}
