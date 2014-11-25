#include "citygml/texturecoordinates.h"
#include "citygml/linearring.h"

namespace citygml {

    TextureCoordinates::TextureCoordinates(std::string id, std::string targetID, std::vector<TVec2f> coords) : Object(id)
    {
        m_targetID = targetID;
        m_coordlist = coords;
    }

    bool TextureCoordinates::targets(const LinearRing& ring) const
    {
        return m_targetID == ring.getId();
    }

    bool TextureCoordinates::eraseCoordinate(unsigned int i)
    {
        if (i < m_coordlist.size()) {
            m_coordlist.erase(m_coordlist.begin() + i);
        }
    }

    const std::vector<TVec2f>& TextureCoordinates::getCoords() const
    {
        return m_coordlist;
    }

}
