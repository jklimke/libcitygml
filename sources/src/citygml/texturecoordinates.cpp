#include <citygml/texturecoordinates.h>
#include <citygml/linearring.h>

namespace citygml {

    TextureCoordinates::TextureCoordinates(std::string id, std::string targetID) : Object(id)
    {
        m_targetID = targetID;
    }

    bool TextureCoordinates::targets(const LinearRing& ring) const
    {
        return m_targetID == ring.getId();
    }

    std::string TextureCoordinates::getTargetLinearRingID() const
    {
        return m_targetID;
    }

    bool TextureCoordinates::eraseCoordinate(unsigned int i)
    {
        if (i < m_coordlist.size()) {
            m_coordlist.erase(m_coordlist.begin() + i);
            return true;
        }
        return false;
    }

    const std::vector<TVec2f>& TextureCoordinates::getCoords() const
    {
        return m_coordlist;
    }

    void TextureCoordinates::setCoords(std::vector<TVec2f> texCoords)
    {
        m_coordlist = texCoords;
    }

}
