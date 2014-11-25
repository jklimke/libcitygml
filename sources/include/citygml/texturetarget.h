#pragma once

#include <string>
#include <memory>
#include <vector>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/appearancetarget.h>

namespace citygml {

    class TextureCoordinates;

    /**
     * @brief The TextureTarget associates a texture with a target surface and defines the mapping of the texture.
     * @note may be shared between different texture objects using the 'xlink:href' attribute
     */
    class TextureTarget : public Object {
    public:
        TextureTarget(std::string targetID);

        /**
         * @brief creates an invalid texture target object
         */
        TextureTarget();

        /**
         * @brief the texture coordinates that define the mapping of the texture to the surface (gml::TexCoordList)
         * @return a list that contains one TextureCoordinates object for each linear ring of the target surface
         */
        const std::vector<TextureCoordinates>& getTexCoordinatesList() const;

        void addTexCoordinates(TextureCoordinates texCoords);

        bool valid() const;

        /**
         * @brief the id of the target surface
         * @note TextureTargets are applicable only to polygonal surfaces, whose boundaries are described by gml:LinearRing
         *       (e.g. gml:Triangle, gml:Polygon, or a gml:MultiSurface consisting of gml:Polygons). The targetID must not be
         *       the id of the object on which the texture is applied at the end (e.g. the TextureTarget may be passed down to the children of the target object)
         */
        std::string getTargetID() const;

    protected:
        bool m_valid;
        std::string m_targetID;
        std::vector<TextureCoordinates> m_coordinatesList;
    };
}
