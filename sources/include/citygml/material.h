#pragma once

#include <citygml/citygml_api.h>
#include <citygml/appearance.h>
#include <citygml/vecs.hpp>
#include <unordered_set>

namespace citygml {

    class CityGMLFactory;

    class LIBCITYGML_EXPORT Material : virtual public Appearance
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

        float getShininess() const;
        void setShininess(float shininess);

        float getTransparency() const;
        void setTransparency(float transparancy);

        void addTargetID(std::string id);
        virtual bool targets(const AppearanceTarget& obj) const;
        virtual void copyTargetDefinition(const AppearanceTarget& oldTarget, const AppearanceTarget& newTarget);
        virtual std::vector<std::string> getTargetIDs() const;

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
        std::unordered_set<std::string> m_targetIDs;
    };

}
