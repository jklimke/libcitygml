#include "citygml/texturetargetdefinition.h"
#include "citygml/texturecoordinates.h"

namespace citygml {

    TextureTargetDefinition::TextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id) : AppearanceTargetDefinition(targetID, appearance, id)
    {
    }

    unsigned int TextureTargetDefinition::getTextureCoordinatesCount() const
    {
        return m_coordinatesList.size();
    }

    TextureCoordinates* TextureTargetDefinition::getTextureCoordinates(unsigned int i)
    {
        return m_coordinatesList.at(i).get();
    }

    const TextureCoordinates*TextureTargetDefinition::getTextureCoordinates(unsigned int i) const
    {
        return m_coordinatesList.at(i).get();
    }

    TextureCoordinates* TextureTargetDefinition::getTextureCoordinatesForID(const std::string& ringID)
    {
        auto it = m_idTexCoordMap.find(ringID);
        if (it != m_idTexCoordMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    const TextureCoordinates* TextureTargetDefinition::getTextureCoordinatesForID(const std::string& ringID) const
    {
        const auto it = m_idTexCoordMap.find(ringID);
        if (it != m_idTexCoordMap.end()) {
            return it->second;
        }
        return nullptr;
    }

    void TextureTargetDefinition::addTexCoordinates(TextureCoordinates* texCoords)
    {
        m_coordinatesList.push_back(std::unique_ptr<TextureCoordinates>(texCoords));
        m_idTexCoordMap[texCoords->getTargetLinearRingID()] = texCoords;
    }

    TextureTargetDefinition::~TextureTargetDefinition()
    {

    }

}
