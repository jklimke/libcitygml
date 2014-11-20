#include "citygml/implictgeometry.h"

namespace citygml {

    ImplicitGeometry::ImplicitGeometry(const std::string& id) : Object(id)
    {

    }

    void ImplicitGeometry::setMatrix(const TransformationMatrix& matrix)
    {
        m_matrix = matrix;
    }

    TransformationMatrix ImplicitGeometry::getMatrix() const
    {
        return m_matrix;
    }

    void ImplicitGeometry::setReferencePoint(const TVec3d& referencePoint)
    {
        m_referencePoint = referencePoint;
    }

    TVec3d ImplicitGeometry::getReferencePoint() const
    {
        return m_referencePoint;
    }

    unsigned int ImplicitGeometry::size() const
    {
        return m_geometries.size();
    }

    Geometry*ImplicitGeometry::getGeometry(unsigned int i) const
    {
        return m_geometries[i];
    }

    std::string ImplicitGeometry::getSRSName() const
    {
        return m_srsName;
    }

}
