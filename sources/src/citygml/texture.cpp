#include "citygml/texture.h"

namespace citygml {

    Texture::Texture(const std::string& id) : Appearance( id, "Texture" ), m_repeat( false ), m_wrapMode( WrapMode::WM_NONE )
    {

    }

    Texture::Texture(const std::string& id, const std::string& type) : Appearance( id, type ), m_repeat( false ), m_wrapMode( WrapMode::WM_NONE )
    {

    }

    std::shared_ptr<Texture> Texture::createNewTexture(const std::string& id)
    {
        return std::shared_ptr<Texture>(new Texture(id));
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

    bool Texture::targets(const AppearanceTarget& obj) const
    {
        return m_idTargetMap.count(obj.getId()) > 0;
    }

    void Texture::copyTargetDefinition(const AppearanceTarget& oldTarget, const AppearanceTarget& newTarget)
    {
        std::unordered_map<std::string, TextureTarget&>::iterator it = m_idTargetMap.find(oldTarget.getId());
        if (it != m_idTargetMap.end()) {
            TextureTarget& texTarget = it->second;
            m_idTargetMap[newTarget.getTargetID()] = texTarget;
        }
    }

    void Texture::addTarget(TextureTarget* target)
    {
        m_targets.push_back(std::unique_ptr<TextureTarget>(target));
        m_idTargetMap[m_targets.back()->getTargetID()].push_back(*m_targets.back());
    }

    const TextureTarget invalidTarget;

    const TextureTarget& Texture::getTextureTargetFor(const Object& obj) const
    {
        const auto it = m_idTargetMap.find(obj.getId());
        if (it != m_idTargetMap.end()) {
            return *it;
        }
        return invalidTarget;
    }

    TextureTarget& Texture::getTextureTargetFor(const Object& obj)
    {
        auto it = m_idTargetMap.find(obj.getId());
        if (it != m_idTargetMap.end()) {
            return *it;
        }
        return invalidTarget;
    }

    std::vector<std::string> Texture::getTargetIDs() const
    {
        std::vector<std::string> targetIDs;
        for (const auto& pair : m_idTargetMap) {
            targetIDs.push_back(pair.first);
        }
        return targetIDs;
    }

    std::shared_ptr<Texture> Texture::asTexture()
    {
        return shared_from_this();
    }

    std::shared_ptr<const Texture> Texture::asTexture() const
    {
        return shared_from_this();
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
