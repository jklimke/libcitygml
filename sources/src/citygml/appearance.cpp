#include "citygml/appearance.h"

namespace citygml {

    Appearance::Appearance(const std::string& id, const std::string& typeString) : Object( id ), m_typeString( typeString ), m_isFront(true)
    {

    }

    std::string Appearance::getType() const
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

    Material* Appearance::asMaterial()
    {
        return nullptr;
    }

    const Material* Appearance::asMaterial() const
    {
        return nullptr;
    }

    Texture* Appearance::asTexture()
    {
        return nullptr;
    }

    const Texture*Appearance::asTexture() const
    {
        return nullptr;
    }

    GeoreferencedTexture* Appearance::asGeoreferencedTexture()
    {
        return nullptr;
    }

    const GeoreferencedTexture* Appearance::asGeoreferencedTexture() const
    {
        return nullptr;
    }

    bool Appearance::inTheme(const std::string& themeName) const
    {
        return m_themes.count(themeName) > 0;
    }

    bool Appearance::addToTheme(std::string themeName)
    {
        m_themes.insert(themeName);
    }

}
