#pragma once

#include <citygml/citygml_api.h>
#include <citygml/texture.h>

namespace citygml {

    class CityGMLFactory;

    class LIBCITYGML_EXPORT GeoreferencedTexture : public Texture
    {
        friend class CityGMLFactory;
    public:

        bool getPreferWorldFile() const;
        void setPreferWorldFile(bool value);

        virtual std::shared_ptr<Texture> asTexture();
        virtual std::shared_ptr<const Texture> asTexture() const;

        virtual std::shared_ptr<GeoreferencedTexture> asGeoreferencedTexture();
        virtual std::shared_ptr<const GeoreferencedTexture> asGeoreferencedTexture() const;

        // TODO support referencePoint and orientation

    protected:
        GeoreferencedTexture( const std::string& id );
        bool m_preferWorldFile;
    };

}
