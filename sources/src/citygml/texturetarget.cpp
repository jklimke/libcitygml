#include "citygml/texturetarget.h"
#include "citygml/texturecoordinates.h"

namespace citygml {

    TextureTarget::TextureTarget(std::string targetID) : Object()
    {
        m_targetID = targetID;
        m_coordinates = nullptr;
    }

    const TextureCoordinates& TextureTarget::getTexCoordinates() const
    {
        return m_coordinates;
    }

    TextureCoordinates& TextureTarget::getTexCoordinates() const
    {
        return m_coordinates;
    }

    void TextureTarget::setTexCoordinates(TextureCoordinates texCoords)
    {
        m_coordinates = texCoords;
    }

    std::string TextureTarget::getTargetID() const
    {
        return m_targetID;
    }


}
