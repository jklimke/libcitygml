#pragma once

#include <string>
#include <vector>
#include <memory>

#include <citygml/citygml_api.h>
#include <citygml/vecs.hpp>
#include <citygml/object.h>

namespace citygml {

    class LinearRing;

    class TextureCoordinates : public Object {
    public:
        TextureCoordinates(std::string targetID, std::vector<TVec2f> coords);
        bool targets(const LinearRing& ring) const;
        const std::vector<TVec2f>& getCoords() const;
        bool eraseCoordinate(unsigned int i);

    protected:
        std::string m_targetID;
        std::vector<TVec2f> m_coordlist;
    };
}
