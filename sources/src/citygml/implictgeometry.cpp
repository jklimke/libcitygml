#include "citygml/implictgeometry.h"
#include "citygml/geometry.h"

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

    void ImplicitGeometry::addGeometry(std::shared_ptr<Geometry> geom)
    {
        m_geometries.push_back(geom);
    }

    unsigned int ImplicitGeometry::getGeometriesCount() const
    {
        return m_geometries.size();
    }

    Geometry& ImplicitGeometry::getGeometry(unsigned int i) const
    {
        return *m_geometries[i];
    }

    std::string ImplicitGeometry::getSRSName() const
    {
        return m_srsName;
    }

}
