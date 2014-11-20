#include "citygml/texture.h"

namespace citygml {

    Texture::Texture(const std::string& id) : Appearance( id, "Texture" ), m_repeat( false ), m_wrapMode( WrapMode::WM_NONE )
    {

    }

    Texture::Texture(const std::string& id, const std::string& type) : Appearance( id, type ), m_repeat( false ), m_wrapMode( WrapMode::WM_NONE )
    {

    }

    std::string Texture::getUrl() const
    {
        return m_url;
    }

    void Texture::setUrl(const std::string& url)
    {
        m_url = url;
    }

    bool Texture::getRepeat() const
    {
        return m_repeat;
    }

    Texture::WrapMode Texture::getWrapMode() const
    {
        return m_wrapMode;
    }

    void Texture::setWrapMode(Texture::WrapMode mode)
    {
        m_wrapMode = mode;
    }

    TVec4f Texture::getBorderColor() const
    {
        return m_borderColor;
    }

    void Texture::setBorderColor(TVec4f color)
    {
        m_borderColor = color;
    }

    std::string Texture::toString() const
    {
        return Appearance::toString() + " (url: " + m_url + ")";
    }

    bool Texture::targets(const Object& obj) const
    {
        return m_idTargetMap.count(obj.getId()) > 0;
    }

    void Texture::addTarget(TextureTarget* target)
    {
        m_idTargetMap[target.getTargetID()].push_back(target);
    }

    const std::vector<TextureTarget> emptyTargets;

    std::vector<const TextureTarget*> Texture::getTextureTargetsFor(const Object& obj) const
    {
        const auto it = m_idTargetMap.find(obj.getId());
        if (it != m_idTargetMap.end()) {
            return *it;
        }
        return emptyTargets;
    }

    std::vector<TextureTarget*> Texture::getTextureTargetsFor(const Object& obj)
    {
        auto it = m_idTargetMap.find(obj.getId());
        if (it != m_idTargetMap.end()) {
            return (*it).get();
        }
        return emptyTargets;
    }

    Texture* Texture::asTexture()
    {
        return this;
    }

    const Texture* Texture::asTexture() const
    {
        return this;
    }

    Texture::~Texture()
    {
        for (auto& pair : m_idTargetMap) {
            for (TextureTarget* target : pair.second) {
                delete target;
            }
        }
    }


}
