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

    Texture* GeoreferencedTexture::asTexture()
    {
        return this;
    }

    const Texture* GeoreferencedTexture::asTexture() const
    {
        return this;
    }

    GeoreferencedTexture* GeoreferencedTexture::asGeoreferencedTexture()
    {
        return this;
    }

    const GeoreferencedTexture* GeoreferencedTexture::asGeoreferencedTexture() const
    {
        return this;
    }

}
