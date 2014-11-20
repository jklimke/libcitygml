#pragma once

#include <vector>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/vecs.hpp>

namespace citygml {

    class TextureTarget;

    class LIBCITYGML_EXPORT LinearRing : public Object
    {
    public:
        LinearRing( const std::string& id, bool isExterior );

        bool isExterior() const;

        unsigned int size() const;

        const std::vector<TVec3d>& getVertices() const;

        void addVertex( const TVec3d& v );

        TVec3d computeNormal() const;

        inline std::vector<TVec3d>& getVertices();

        void removeDuplicateVertices(const std::vector<TextureTarget*>& targets );

        void forgetVertices();

    protected:
        bool m_exterior;

        std::vector<TVec3d> m_vertices;
    };

}
