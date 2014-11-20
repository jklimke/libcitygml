#pragma once

#include <string>
#include <unordered_set>

#include <citygml/citygml_api.h>
#include <citygml/object.h>

namespace citygml {

    class Material;
    class Texture;
    class GeoreferencedTexture;

    class LIBCITYGML_EXPORT Appearance : public Object
    {
    public:
        Appearance( const std::string& id, const std::string& typeString );

        std::string getType() const;

        bool getIsFront() const;
        void setIsFront(bool front);

        virtual std::string toString() const;

        /**
         * @brief returns if the appearance targets the citygml object
         */
        virtual bool targets(const Object& obj) const = 0;

        virtual Material* asMaterial();
        virtual const Material* asMaterial() const;

        virtual Texture* asTexture();
        virtual const Texture* asTexture() const;

        virtual GeoreferencedTexture* asGeoreferencedTexture();
        virtual const GeoreferencedTexture* asGeoreferencedTexture() const;

        bool inTheme(const std::string& themeName) const;
        bool addToTheme(std::string themeName);

        virtual ~Appearance() {}

    protected:
        std::string m_typeString;
        std::unordered_set<std::string> m_themes;
        bool m_isFront;
    };

}
