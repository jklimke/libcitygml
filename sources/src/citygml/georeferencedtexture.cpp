#include "citygml/georeferencedtexture.h"

namespace citygml {

    GeoreferencedTexture::GeoreferencedTexture(const std::string& id) : Texture( id, "GeoreferencedTexture" ), m_preferWorldFile(true)
    {

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
        return std::static_pointer_cast<Texture>(shared_from_this());
    }

    std::shared_ptr<const Texture> GeoreferencedTexture::asTexture() const
    {
        return std::static_pointer_cast<const Texture>(shared_from_this());
    }

    std::shared_ptr<GeoreferencedTexture> GeoreferencedTexture::asGeoreferencedTexture()
    {
        return std::static_pointer_cast<GeoreferencedTexture>(shared_from_this());
    }

    std::shared_ptr<const GeoreferencedTexture> GeoreferencedTexture::asGeoreferencedTexture() const
    {
        return std::static_pointer_cast<const GeoreferencedTexture>(shared_from_this());
    }

}
