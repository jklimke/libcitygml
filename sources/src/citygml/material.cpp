#include "citygml/material.h"

namespace citygml {

    Material::Material(const std::string& id) : Appearance( id, "Material" )
    {
        // Sets default values of the X3DMaterial definied in the citygml 1.0.0 spec see page 32 (section 9.3 Material)
        m_ambientIntensity =  0.2f;

        m_diffuse = TVec3f(0.8f, 0.8f, 0.8f);
        m_emissive = TVec3f(0.f, 0.f, 0.f);
        m_specular = TVec3f(1.f, 1.f, 1.f);

        m_shininess = 0.2f;
        m_transparency = 0.f;

        m_isSmooth = false;

    }

    bool Material::isSmooth() const
    {
        return m_isSmooth;
    }

    void Material::setIsSmooth(bool isSmooth)
    {
        m_isSmooth = isSmooth;
    }


    TVec3f Material::getDiffuse() const
    {
        return m_diffuse;
    }

    void Material::setDiffuse(TVec3f diffuse)
    {
        m_diffuse = diffuse;
    }

    TVec3f Material::getEmissive() const
    {
        return m_emissive;
    }

    void Material::setEmissive(TVec3f emissive)
    {
        m_emissive = emissive;
    }

    TVec3f Material::getSpecular() const
    {
        return m_specular;
    }

    void Material::setSpecular(TVec3f specular)
    {
        m_specular = specular;
    }

    float Material::getAmbientIntensity() const
    {
        return m_ambientIntensity;
    }

    void Material::setAmbientIntensity(float intensity)
    {
        m_ambientIntensity = intensity;
    }

    float Material::getShininess() const
    {
        return m_shininess;
    }

    void Material::setShininess(float shininess)
    {
        m_shininess = shininess;
    }

    float Material::getTransparency() const
    {
        return m_transparency;
    }

    void Material::setTransparency(float transparancy)
    {
        m_transparency = transparancy;
    }

    std::shared_ptr<Material> Material::asMaterial()
    {
        return std::static_pointer_cast<Material>(shared_from_this());
    }

    std::shared_ptr<const Material> Material::asMaterial() const
    {
        return std::static_pointer_cast<const Material>(shared_from_this());
    }

}
