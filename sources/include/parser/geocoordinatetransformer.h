#pragma once

#include <string>
#include <memory>
#include <unordered_map>

class GeoTransform;

namespace citygml {

    class CityGMLLogger;
    class CityModel;
    class CityObject;
    class ImplicitGeometry;
    class Geometry;
    class Polygon;
    class LineString;

    class GeoCoordinateTransformer {
    public:
        GeoCoordinateTransformer(const std::string& destSRS, std::shared_ptr<CityGMLLogger> logger);

        void transformToDestinationSRS(CityModel* model);
    private:
        std::string m_destinationSRS;
        std::shared_ptr<CityGMLLogger> m_logger;
        std::unordered_map<Polygon*, std::string> m_transformedPolygonsSourceURNMap;
        std::unordered_map<LineString*, std::string> m_transformedLineStringsSourceURNMap;

        void transformRecursive(CityObject& obj, GeoTransform& transformation);
        void transformRecursive_helper(CityObject& obj, GeoTransform& transformation);
        void transformRecursive(ImplicitGeometry& obj, GeoTransform& transformation);
        void transformRecursive_helper(ImplicitGeometry& obj, GeoTransform& transformation);
        void transform(Geometry& obj, GeoTransform& transformation);
    };

}



