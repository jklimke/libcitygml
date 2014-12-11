#pragma once

#include <string>
#include <vector>
#include <memory>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/appearancetarget.h>

namespace citygml {

    class Material;
    class Texture;
    class GeoreferencedTexture;

    class LIBCITYGML_EXPORT Appearance : public Object, public std::enable_shared_from_this<Appearance>
    {
    public:
        std::string getType() const;

        bool getIsFront() const;
        void setIsFront(bool front);

        virtual std::string toString() const;

        virtual std::shared_ptr<Material> asMaterial();
        virtual std::shared_ptr<const Material> asMaterial() const;

        virtual std::shared_ptr<Texture> asTexture();
        virtual std::shared_ptr<const Texture> asTexture() const;

        virtual std::shared_ptr<GeoreferencedTexture> asGeoreferencedTexture();
        virtual std::shared_ptr<const GeoreferencedTexture> asGeoreferencedTexture() const;

        bool inTheme(const std::string& themeName) const;
        void addToTheme(std::string themeName);
        const std::vector<std::string>& getThemes() const;

        virtual ~Appearance() {}

    protected:
        Appearance( const std::string& id, const std::string& typeString );
        std::string m_typeString;
        std::vector<std::string> m_themes;
        bool m_isFront;
    };

}
