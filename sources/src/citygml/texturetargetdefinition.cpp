#include <citygml/texturetargetdefinition.h>
#include <citygml/texturecoordinates.h>

namespace citygml {

    TextureTargetDefinition::TextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id) : AppearanceTargetDefinition(targetID, appearance, id)
    {
    }

    unsigned int TextureTargetDefinition::getTextureCoordinatesCount() const
    {
        return static_cast<unsigned int>(m_coordinatesList.size());
    }

    std::shared_ptr<TextureCoordinates> TextureTargetDefinition::getTextureCoordinates(unsigned int i)
    {
        return m_coordinatesList.at(i);
    }

    std::shared_ptr<const TextureCoordinates> TextureTargetDefinition::getTextureCoordinates(unsigned int i) const
    {
        return m_coordinatesList.at(i);
    }

    std::shared_ptr<TextureCoordinates> TextureTargetDefinition::getTextureCoordinatesForID(const std::string& ringID)
    {
        auto it = m_idTexCoordMap.find(ringID);
        if (it != m_idTexCoordMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::shared_ptr<const TextureCoordinates> TextureTargetDefinition::getTextureCoordinatesForID(const std::string& ringID) const
    {
        const auto it = m_idTexCoordMap.find(ringID);
        if (it != m_idTexCoordMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    void TextureTargetDefinition::addTexCoordinates(std::shared_ptr<TextureCoordinates> texCoords)
    {
        m_coordinatesList.push_back(texCoords);
        m_idTexCoordMap[texCoords->getTargetLinearRingID()] = texCoords;
    }

    TextureTargetDefinition::~TextureTargetDefinition()
    {

    }

}
