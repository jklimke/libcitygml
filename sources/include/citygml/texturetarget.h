#pragma once

#include <string>
#include <memory>

#include <citygml/citygml_api.h>
#include <citygml/object.h>

namespace citygml {

    class TextureCoordinates;

    class TextureTarget : public Object {
    public:
        TextureTarget(std::string targetID);

        const TextureCoordinates& getTexCoordinates() const;
        TextureCoordinates& getTexCoordinates() const;

        void setTexCoordinates(TextureCoordinates texCoords);

        std::string getTargetID() const;

    protected:
        std::string m_targetID;
        TextureCoordinates m_coordinates;
    };
}
