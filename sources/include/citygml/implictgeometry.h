#pragma once

#include <vector>
#include <memory>

#include <citygml/object.h>
#include <citygml/transformmatrix.h>
#include <citygml/vecs.hpp>

namespace citygml {

    class Geometry;
    class CityGMLFactory;

    class ImplicitGeometry : public Object
    {
        friend class CityGMLFactory;
    public:
        void setMatrix(const TransformationMatrix& matrix);
        TransformationMatrix getMatrix() const;

        void setReferencePoint(const TVec3d& referencePoint);
        TVec3d getReferencePoint() const;

        void addGeometry(Geometry* geom);

        // Get the number of geometries contains in the object
        unsigned int getGeometriesCount() const;

        // Access the geometries
        Geometry& getGeometry( unsigned int i ) const;

        // Access the srs of the implicit geometry
        std::string getSRSName() const;

    protected:
        ImplicitGeometry(const std::string& id);

        TransformationMatrix     m_matrix;
        TVec3d                   m_referencePoint;
        std::vector<std::unique_ptr<Geometry>>   m_geometries;
        std::string              m_srsName;
    };
}
