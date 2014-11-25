#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <citygml/citygml_api.h>
#include <citygml/appearance.h>
#include <citygml/texturetarget.h>
#include <citygml/vecs.hpp>

namespace citygml {

    class CityGMLFactory;

    class LIBCITYGML_EXPORT Texture : public Appearance
    {
        friend class CityGMLFactory;
    public:
        enum class WrapMode
        {
            WM_NONE,        // the resulting color is fully transparent
            WM_WRAP,		// the texture is repeated
            WM_MIRROR,		// the texture is repeated and mirrored
            WM_CLAMP,		// the texture is clamped to its edges
            WM_BORDER		// the resulting color is specified by the borderColor element (RGBA)
        };

        std::string getUrl() const;

        void setUrl(const std::string& url);

        bool getRepeat() const;

        WrapMode getWrapMode() const;
        void setWrapMode(WrapMode mode);

        TVec4f getBorderColor() const;
        void setBorderColor(TVec4f color);

        std::string toString() const;

        virtual bool targets(const AppearanceTarget& obj) const;
        virtual void copyTargetDefinition(const AppearanceTarget& oldTarget, const AppearanceTarget& newTarget);

        void addTarget(TextureTarget* target);
        const TextureTarget& getTextureTargetFor(const Object& obj) const;
        TextureTarget& getTextureTargetFor(const Object& obj);
        virtual std::vector<std::string> getTargetIDs() const;

        virtual std::shared_ptr<Texture> asTexture() override;
        virtual std::shared_ptr<const Texture> asTexture() const override;

        virtual ~Texture();

    protected:
        Texture( const std::string& id );
        Texture( const std::string& id, const std::string& type );
        std::string m_url;
        bool m_repeat;
        WrapMode m_wrapMode;
        TVec4f m_borderColor;
        std::vector<std::unique_ptr<TextureTarget>> m_targets;
        std::unordered_map<std::string, TextureTarget&> m_idTargetMap;
    };

}
