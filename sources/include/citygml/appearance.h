#pragma once

#include <string>
#include <unordered_set>
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

        /**
         * @brief returns if the appearance targets the citygml object
         */
        virtual bool targets(const AppearanceTarget& obj) const = 0;

        virtual std::vector<std::string> getTargetIDs() const = 0;

        /**
         * @brief lets the appearance target newTarget with the same settings as oldTarget
         * @param oldTarget
         * @param newTarget
         */
        virtual void copyTargetDefinition(const AppearanceTarget& oldTarget, const AppearanceTarget& newTarget) = 0;

        virtual std::shared_ptr<Material> asMaterial();
        virtual std::shared_ptr<const Material> asMaterial() const;

        virtual std::shared_ptr<Texture> asTexture();
        virtual std::shared_ptr<const Texture> asTexture() const;

        virtual std::shared_ptr<GeoreferencedTexture> asGeoreferencedTexture();
        virtual std::shared_ptr<const GeoreferencedTexture> asGeoreferencedTexture() const;

        bool inTheme(const std::string& themeName) const;
        bool addToTheme(std::string themeName);

        virtual ~Appearance() {}

    protected:
        Appearance( const std::string& id, const std::string& typeString );
        std::string m_typeString;
        std::unordered_set<std::string> m_themes;
        bool m_isFront;
    };

}
