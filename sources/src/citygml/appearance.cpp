#include <citygml/appearance.h>

#include <algorithm>

namespace citygml {

    Appearance::Appearance(const std::string& id, const std::string& typeString) : Object( id ), m_typeString( typeString ), m_isFront(true)
    {

    }

    const std::string& Appearance::getType() const
    {
        return m_typeString;
    }

    bool Appearance::getIsFront() const
    {
        return m_isFront;
    }

    void Appearance::setIsFront(bool front)
    {
        m_isFront = front;
    }

    std::string Appearance::toString() const
    {
        return m_typeString + " " + m_id;
    }

    std::shared_ptr<Material> Appearance::asMaterial()
    {
        return nullptr;
    }

    std::shared_ptr<const Material> Appearance::asMaterial() const
    {
        return nullptr;
    }

    std::shared_ptr<Texture> Appearance::asTexture()
    {
        return nullptr;
    }

    std::shared_ptr<const Texture> Appearance::asTexture() const
    {
        return nullptr;
    }

    std::shared_ptr<GeoreferencedTexture> Appearance::asGeoreferencedTexture()
    {
        return nullptr;
    }

    std::shared_ptr<const GeoreferencedTexture> Appearance::asGeoreferencedTexture() const
    {
        return nullptr;
    }

    bool Appearance::inTheme(const std::string& themeName) const
    {
        return std::find(m_themes.begin(), m_themes.end(), themeName) != m_themes.end();
    }

    void Appearance::addToTheme(std::string themeName)
    {
        m_themes.push_back(themeName);
    }

    const std::vector<std::string>& Appearance::getThemes() const
    {
        return m_themes;
    }

}
