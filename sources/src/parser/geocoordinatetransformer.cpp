#include "parser/geocoordinatetransformer.h"

#include "citygml/citygmllogger.h"
#include "citygml/citymodel.h"
#include "citygml/cityobject.h"
#include "citygml/implictgeometry.h"
#include "citygml/geometry.h"
#include "citygml/polygon.h"
#include "citygml/vecs.hpp"
#include "citygml/envelope.h"

#ifdef USE_GDAL
#include "ogrsf_frmts.h"

#include <stdexcept>

class GeoTransform
{
public:
    GeoTransform( const std::string& destURN, std::shared_ptr<citygml::CityGMLLogger> logger )
    {
        m_logger = logger;
        m_transformation = nullptr;
        m_destSRSURN = destURN;
        m_sourceURN = "";

        OGRErr err = m_destSRS.SetFromUserInput(destURN.c_str());

        if (err != OGRERR_NONE) {
            CITYGML_LOG_ERROR(m_logger, "Could not create OGRSpatialReference for destination SRS " << destURN << ". OGR error code: " << err << ".");
            throw std::runtime_error("Invalid destination spatial reference system.");
        }
    }

    GeoTransform(const GeoTransform& other) {
        m_logger = other.m_logger;
        m_destSRSURN = other.m_destSRSURN;
        m_destSRS = other.m_destSRS;
        m_sourceURN = "";

        m_transformation = nullptr;
    }

    ~GeoTransform()
    {
        if (m_transformation != nullptr) {
            OCTDestroyCoordinateTransformation(m_transformation);
        }
    }

    bool valid() const {
        return m_transformation != nullptr;
    }

    void transform( TVec3d &p ) const
    {
        if (m_transformation == nullptr) {
            return;
        }

        m_transformation->Transform( 1, &p.x, &p.y );
    }

    void transform( TVec2d &p ) const
    {
        if (m_transformation == nullptr) {
            return;
        }

        m_transformation->Transform( 1, &p.x, &p.y );
    }

    bool hasSourceSRS(const std::string& sourceURN) {

        if (m_transformation == nullptr) {
            return false;
        }

        OGRSpatialReference refSys;
        OGRErr err = refSys.SetFromUserInput(sourceURN.c_str());

        if (err != OGRERR_NONE) {
            return false;
        }

        return refSys.IsSame(m_transformation->GetSourceCS());

    }

    void setSourceSRS(const std::string& sourceURN) {

        if (m_transformation != nullptr) {
            OCTDestroyCoordinateTransformation(m_transformation);
            m_transformation = nullptr;
        }

        OGRSpatialReference sourceSRS;
        OGRErr err = sourceSRS.SetFromUserInput(sourceURN.c_str());

        if (err != OGRERR_NONE) {
            CITYGML_LOG_ERROR(m_logger, "Could not create OGRSpatialReference for source SRS " << sourceURN << ". OGR error code: " << err << ".");
            return;
        }

        m_transformation = OGRCreateCoordinateTransformation(&sourceSRS, &m_destSRS);
        if (m_transformation == nullptr) {
            CITYGML_LOG_ERROR(m_logger, "Could not create transformation from source SRS " << sourceURN << " to destination SRS " << m_destSRSURN << ".");
            return;
        }

        m_sourceURN = sourceURN;
    }

    std::string sourceURN() const { return m_sourceURN; }

private:
    OGRSpatialReference m_destSRS;
    std::string m_destSRSURN;
    std::string m_sourceURN;

    OGRCoordinateTransformation* m_transformation;
    std::shared_ptr<citygml::CityGMLLogger> m_logger;
};

namespace citygml {

    GeoCoordinateTransformer::GeoCoordinateTransformer(const std::string& destSRS, std::shared_ptr<CityGMLLogger> logger)
    {
        m_destinationSRS = destSRS;
        m_logger = logger;
    }

    void GeoCoordinateTransformer::transformToDestinationSRS(CityModel* model)
    {
        GeoTransform transformation(m_destinationSRS, m_logger);

        if (!model->getEnvelope().srsName().empty()) {
            transformation.setSourceSRS(model->getEnvelope().srsName());
        }

        for (unsigned int i = 0; i < model->getNumRootCityObjects(); i++) {
            transformRecursive(model->getRootCityObject(i), transformation);
        }
    }

    void GeoCoordinateTransformer::transformRecursive(CityObject& obj, GeoTransform& transformation) {

        if (!obj.getEnvelope().srsName().empty() && !transformation.hasSourceSRS(obj.getEnvelope().srsName())) {
            GeoTransform childTransformation = transformation;
            childTransformation.setSourceSRS(obj.getEnvelope().srsName());

            transformRecursive_helper(obj, childTransformation);
        } else {
            transformRecursive_helper(obj, transformation);
        }

    }

    void  GeoCoordinateTransformer::transformRecursive_helper(CityObject& obj, GeoTransform& transformation) {

        if (obj.getEnvelope().validBounds()) {

            if (transformation.valid()) {
                TVec3d lowerBound = obj.getEnvelope().getLowerBound();
                TVec3d upperBound = obj.getEnvelope().getUpperBound();

                transformation.transform(lowerBound);
                transformation.transform(upperBound);

                Envelope* newEnvelope = new Envelope(m_destinationSRS);
                newEnvelope->setLowerBound(lowerBound);
                newEnvelope->setUpperBound(upperBound);

                obj.setEnvelope(newEnvelope);
            } else {
                CITYGML_LOG_WARN(m_logger, "No valid spatial reference system is given for CityObject with id '" << obj.getId() << "'. Envelope (Bounding Box) is not transformed.");
            }
        }

        for (unsigned int i = 0; i < obj.getChildCityObjectsCount(); i++) {
            transformRecursive(obj.getChildCityObject(i), transformation);
        }

        for (unsigned int i = 0; i < obj.getImplicitGeometryCount(); i++) {
            transformRecursive(obj.getImplicitGeometry(i), transformation);
        }

        for (unsigned int i = 0; i < obj.getGeometriesCount(); i++) {
            transform(obj.getGeometry(i), transformation);
        }
    }

    void GeoCoordinateTransformer::transformRecursive(ImplicitGeometry& obj, GeoTransform& transformation) {

        if (!obj.getSRSName().empty() && !transformation.hasSourceSRS(obj.getSRSName())) {
            GeoTransform childTransformation = transformation;
            childTransformation.setSourceSRS(obj.getSRSName());

            transformRecursive_helper(obj, childTransformation);
        } else {
            transformRecursive_helper(obj, transformation);
        }

    }

    void  GeoCoordinateTransformer::transformRecursive_helper(ImplicitGeometry& obj, GeoTransform& transformation) {

        TVec3d referencePoint = obj.getReferencePoint();
        if (transformation.valid()) {
            transformation.transform(referencePoint);
            obj.setReferencePoint(referencePoint);
            obj.setSRSName(m_destinationSRS);
        } else {
            CITYGML_LOG_WARN(m_logger, "No valid spatial reference system is given for ImplicitGeometry with id '" << obj.getId() << "'. Reference Point is not transformed.");
        }

        // Do not transform the geometry of an ImplicitGeometry object. Implicit Geometries share Geometry objects but each implicit geometry
        // defines its own transformation on the vertices of the shared geometry. Hence those vertices are in a local coordinate system. Read
        // the citygml documentation for more details.
        //for (unsigned int i = 0; i < obj.getGeometriesCount(); i++) {
        //  transform(obj.getGeometry(i), transformation);
        //}
    }

    void GeoCoordinateTransformer::transform(Geometry& obj, GeoTransform& transformation) {

        if (!transformation.valid()) {
            CITYGML_LOG_WARN(m_logger, "No valid spatial reference system is given for Geometry with id '" << obj.getId() << "'. Child Polygons are not transformed"
                                       << "(unless they are shared with another geometry for which a spatial reference system is defined)");
            return;
        }

        for (unsigned int i = 0; i < obj.getPolygonsCount(); i++) {

            Polygon& poly = obj.getPolygon(i);

            auto it = m_transformedPolygonsSourceURNMap.find(poly.getId());

            if (it == m_transformedPolygonsSourceURNMap.end()) {

                for (TVec3d& vertex : poly.getVertices()) {
                    transformation.transform(vertex);
                }

                m_transformedPolygonsSourceURNMap[poly.getId()] = transformation.sourceURN();

            } else if (it->second != transformation.sourceURN()) {
                CITYGML_LOG_WARN(m_logger, "Polygon with id '" << poly.getId() << "' was already transformed from " << it->second << " to " << m_destinationSRS
                                 << ". But the spatial reference system of Geometry object with id '" << obj.getId() << "' that also contains the polygon is different "
                                 << "(" << transformation.sourceURN() << "). Ignoring new source SRS.");
            }
        }

        for (unsigned int i = 0; i < obj.getGeometriesCount(); i++) {
            transform(obj.getGeometry(i), transformation);
        }
    }
}
#else
class GeoTransform {
};

namespace citygml {


    void GeoCoordinateTransformer::transformToDestinationSRS(CityModel* model) {
        CITYGML_LOG_WARN(m_model, "Coordinate transformation to " << m_destinationSRS << " requested, but libcitygml was build without GDAL. The coordinates will not be transformed.");
    }

    void GeoCoordinateTransformer::transformRecursive(CityObject&, GeoTransform&) {}
    void GeoCoordinateTransformer::transformRecursive_helper(CityObject&, GeoTransform&) {}
    void GeoCoordinateTransformer::transformRecursive(ImplicitGeometry&, GeoTransform&) {}
    void GeoCoordinateTransformer::transformRecursive_helper(ImplicitGeometry&, GeoTransform&) {}
    void GeoCoordinateTransformer::transform(Geometry&, GeoTransform&) {}
}
#endif




