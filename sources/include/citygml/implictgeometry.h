#pragma once

#include <vector>

#include <citygml/object.h>
#include <citygml/transformmatrix.h>
#include <citygml/vecs.hpp>

namespace citygml {

    class Geometry;

    class ImplicitGeometry : public Object
    {
    public:
        ImplicitGeometry(const std::string& id);

        void setMatrix(const TransformationMatrix& matrix);
        TransformationMatrix getMatrix() const;

        void setReferencePoint(const TVec3d& referencePoint);
        TVec3d getReferencePoint() const;

        // Get the number of geometries contains in the object
        unsigned int size() const;

        // Access the geometries
        Geometry* getGeometry( unsigned int i ) const;

        // Access the srs of the implicit geometry
        std::string getSRSName() const;

    protected:
        TransformationMatrix     m_matrix;
        TVec3d                   m_referencePoint;
        std::vector<Geometry*>   m_geometries;
        std::string              m_srsName;
    };
}
