#pragma once

#include <citygml/citygml_api.h>
#include <citygml/texture.h>

namespace citygml {

    class LIBCITYGML_EXPORT GeoreferencedTexture : public Texture
    {
    public:
        GeoreferencedTexture( const std::string& id );

        bool getPreferWorldFile() const;
        void setPreferWorldFile(bool value);

        virtual Texture* asTexture();
        virtual const Texture* asTexture() const;

        virtual GeoreferencedTexture* asGeoreferencedTexture();
        virtual const GeoreferencedTexture* asGeoreferencedTexture() const;

        // TODO support referencePoint and orientation

    protected:
        bool m_preferWorldFile;
    };

}
