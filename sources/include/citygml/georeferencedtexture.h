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

        std::shared_ptr<Texture> asTexture() override;
        std::shared_ptr<const Texture> asTexture() const override;

        std::shared_ptr<GeoreferencedTexture> asGeoreferencedTexture() override;
        std::shared_ptr<const GeoreferencedTexture> asGeoreferencedTexture() const override;

        // TODO support referencePoint and orientation

    protected:
        GeoreferencedTexture( const std::string& id );
        bool m_preferWorldFile;
    };

}
