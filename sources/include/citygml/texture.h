#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <citygml/citygml_api.h>
#include <citygml/appearance.h>
#include <citygml/texturetarget.h>
#include <citygml/vecs.hpp>

namespace citygml {

    class LIBCITYGML_EXPORT Texture : public Appearance
    {
    public:
        enum class WrapMode
        {
            WM_NONE,        // the resulting color is fully transparent
            WM_WRAP,		// the texture is repeated
            WM_MIRROR,		// the texture is repeated and mirrored
            WM_CLAMP,		// the texture is clamped to its edges
            WM_BORDER		// the resulting color is specified by the borderColor element (RGBA)
        };

        Texture( const std::string& id );

        std::string getUrl() const;

        void setUrl(const std::string& url);

        bool getRepeat() const;

        WrapMode getWrapMode() const;
        void setWrapMode(WrapMode mode);

        TVec4f getBorderColor() const;
        void setBorderColor(TVec4f color);

        std::string toString() const;

        virtual bool targets(const Object& obj) const;

        void addTarget(TextureTarget* target);
        std::vector<const TextureTarget*> getTextureTargetsFor(const Object& obj) const;
        std::vector<TextureTarget*> getTextureTargetsFor(const Object& obj);

        virtual Texture* asTexture() override;
        virtual const Texture* asTexture() const override;

        virtual ~Texture();

    protected:
        Texture( const std::string& id, const std::string& type );
        std::string m_url;
        bool m_repeat;
        WrapMode m_wrapMode;
        TVec4f m_borderColor;
        std::unordered_map<std::string, std::vector<TextureTarget*>> m_idTargetMap;

    };

}
