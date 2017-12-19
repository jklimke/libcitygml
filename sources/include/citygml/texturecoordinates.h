#pragma once

#include <string>
#include <vector>
#include <memory>

#include <citygml/citygml_api.h>
#include <citygml/vecs.hpp>
#include <citygml/object.h>

namespace citygml {

    class LinearRing;

    /**
     * @brief The TextureCoordinates class describes a mapping of texture coordinates to the vertices of a linear ring
     */
    class LIBCITYGML_EXPORT TextureCoordinates : public Object {
    public:
        TextureCoordinates(std::string id, std::string targetID);

        bool targets(const LinearRing& ring) const;
        std::string getTargetLinearRingID() const;

        const std::vector<TVec2f>& getCoords() const;
        void setCoords(std::vector<TVec2f> texCoords);

        bool eraseCoordinate(unsigned int i);

    protected:
        std::string m_targetID;
        std::vector<TVec2f> m_coordlist;
    };
}
