#pragma once

#include <vector>
#include <unordered_map>

#include <citygml/citygml_api.h>
#include <citygml/appearancetargetdefinition.h>
#include <citygml/texture.h>

namespace citygml {

    class TextureCoordinates;
    class CityGMLFactory;

    /**
     * @brief The TextureTargetDefinition associates a texture with a target surface and defines the mapping of the texture.
     * @note may be shared between different texture objects using the 'xlink:href' attribute.
     *       TextureTargets are applicable only to polygonal surfaces, whose boundaries are described by gml:LinearRing
     *       (e.g. gml:Triangle, gml:Polygon, or a gml:MultiSurface consisting of gml:Polygons).
     */
    class LIBCITYGML_EXPORT TextureTargetDefinition : public AppearanceTargetDefinition<Texture> {
        friend class CityGMLFactory;
    public:
        /**
         * @brief the number of TextureCoordinates objects for this texture target
         */
        unsigned int getTextureCoordinatesCount() const;

        /**
         * @brief the i-th texture coordinates in texture coordinates list (gml::TexCoordList)
         */
        TextureCoordinates* getTextureCoordinates(unsigned int i);
        const TextureCoordinates* getTextureCoordinates(unsigned int i) const;

        /**
         * @brief the texture coordinates for linear ring with the given id
         * @return the TextureCoordinates object or nullptr if no such object exists for ringID
         */
        TextureCoordinates* getTextureCoordinatesForID(const std::string& ringID);
        const TextureCoordinates* getTextureCoordinatesForID(const std::string& ringID) const;

        void addTexCoordinates(TextureCoordinates* texCoords);


        ~TextureTargetDefinition();

    protected:
        TextureTargetDefinition(const std::string& targetID, std::shared_ptr<Texture> appearance, const std::string& id);
        std::vector<std::unique_ptr<TextureCoordinates>> m_coordinatesList;
        std::unordered_map<std::string, TextureCoordinates*> m_idTexCoordMap;
    };
}
