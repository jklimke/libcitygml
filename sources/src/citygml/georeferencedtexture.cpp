#include "citygml/georeferencedtexture.h"

namespace citygml {

    GeoreferencedTexture::GeoreferencedTexture(const std::string& id) : Texture( id, "GeoreferencedTexture" ), m_preferWorldFile(true)
    {

    }

    std::shared_ptr<GeoreferencedTexture> GeoreferencedTexture::createNewGeoreferencedTexture(const std::string& id)
    {
        return std::shared_ptr<GeoreferencedTexture>(new GeoreferencedTexture(id));
    }

    bool GeoreferencedTexture::getPreferWorldFile() const
    {
        return m_preferWorldFile;
    }

    void GeoreferencedTexture::setPreferWorldFile(bool value)
    {
        m_preferWorldFile = value;
    }

    std::shared_ptr<Texture> GeoreferencedTexture::asTexture()
    {
        return shared_from_this();
    }

    std::shared_ptr<const Texture> GeoreferencedTexture::asTexture() const
    {
        return shared_from_this();
    }

    std::shared_ptr<GeoreferencedTexture> GeoreferencedTexture::asGeoreferencedTexture()
    {
        return shared_from_this();
    }

    std::shared_ptr<const GeoreferencedTexture> GeoreferencedTexture::asGeoreferencedTexture() const
    {
        return shared_from_this();
    }

}
