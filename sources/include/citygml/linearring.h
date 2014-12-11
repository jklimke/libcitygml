#pragma once

#include <vector>
#include <memory>

#include <citygml/citygml_api.h>
#include <citygml/object.h>
#include <citygml/vecs.hpp>

namespace citygml {

    class TextureTargetDefinition;
    class CityGMLLogger;

    class LIBCITYGML_EXPORT LinearRing : public Object
    {
    public:
        LinearRing( const std::string& id, bool isExterior );

        bool isExterior() const;

        unsigned int size() const;

        const std::vector<TVec3d>& getVertices() const;
        std::vector<TVec3d>& getVertices();
        void setVertices(std::vector<TVec3d> vertices);

        void addVertex( const TVec3d& v );

        TVec3d computeNormal() const;

        void removeDuplicateVertices(const std::vector<TextureTargetDefinition*>& targets , std::shared_ptr<CityGMLLogger> logger);

        void forgetVertices();

    protected:
        bool m_exterior;

        std::vector<TVec3d> m_vertices;
    };

}
