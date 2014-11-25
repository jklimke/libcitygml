#include "citygml/texturetarget.h"
#include "citygml/texturecoordinates.h"

namespace citygml {

    TextureTarget::TextureTarget(std::string targetID) : Object()
    {
        m_targetID = targetID;
        m_coordinatesList = nullptr;
        m_valid = true;
    }

    TextureTarget::TextureTarget()
    {
        m_valid = false;
    }

    const std::vector<TextureCoordinates>& TextureTarget::getTexCoordinatesList() const
    {
        return m_coordinatesList;
    }

    void TextureTarget::addTexCoordinates(TextureCoordinates texCoords)
    {
        m_coordinatesList.push_back(texCoords);
    }

    bool TextureTarget::valid() const
    {
        return m_valid;
    }

    std::string TextureTarget::getTargetID() const
    {
        return m_targetID;
    }

}
