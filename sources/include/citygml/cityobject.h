#pragma once

#include <vector>
#include <memory>

#include <citygml/featureobject.h>
#include <citygml/citygml_api.h>
#include <citygml/enum_type_bitmask.h>
#include <citygml/rectifiedgridcoverage.h>
#include <citygml/externalreference.h>
class TesselatorBase;

namespace citygml {

    class ParserParams;
    class Geometry;
    class ImplicitGeometry;
    class Composite;
    class CityGMLLogger;
    class AppearanceManager;
    class Address;

    class LIBCITYGML_EXPORT CityObject : public FeatureObject
    {
    public:

        enum class CityObjectsType : uint64_t {
            COT_GenericCityObject           = 1ll,
            COT_Building                    = 1ll<< 1,
            COT_Room                        = 1ll<< 2,
            COT_BuildingInstallation        = 1ll<< 3,
            COT_BuildingFurniture           = 1ll<< 4,
            COT_Door                        = 1ll<< 5,
            COT_Window                      = 1ll<< 6,
            COT_CityFurniture               = 1ll<< 7,
            COT_Track                       = 1ll<< 8,
            COT_Road                        = 1ll<< 9,
            COT_Railway                     = 1ll<< 10,
            COT_Square                      = 1ll<< 11,
            COT_PlantCover                  = 1ll<< 12,
            COT_SolitaryVegetationObject    = 1ll<< 13,
            COT_WaterBody                   = 1ll<< 14,
            COT_ReliefFeature               = 1ll<< 15,
            COT_ReliefComponent             = 1ll<< 35,
            COT_TINRelief                   = 1ll<< 36,
            COT_MassPointRelief             = 1ll<< 37,
            COT_BreaklineRelief             = 1ll<< 38,
            COT_RasterRelief                = 1ll<< 39,
            COT_LandUse                     = 1ll<< 16,
            COT_Tunnel                      = 1ll<< 17,
            COT_Bridge                      = 1ll<< 18,
            COT_BridgeConstructionElement   = 1ll<< 19,
            COT_BridgeInstallation          = 1ll<< 20,
            COT_BridgePart                  = 1ll<< 21,
            COT_BuildingPart                = 1ll<< 22,

            COT_WallSurface                 = 1ll<< 23,
            COT_RoofSurface                 = 1ll<< 24,
            COT_GroundSurface               = 1ll<< 25,
            COT_ClosureSurface              = 1ll<< 26,
            COT_FloorSurface                = 1ll<< 27,
            COT_InteriorWallSurface         = 1ll<< 28,
            COT_CeilingSurface              = 1ll<< 29,
            COT_CityObjectGroup             = 1ll<< 30,
            COT_OuterCeilingSurface         = 1ll<< 31,
            COT_OuterFloorSurface           = 1ll<< 32,


            // covers all supertypes of tran::_TransportationObject that are not Track, Road, Railway or Square...
            // there are to many for to few bits to explicitly enumerate them. However Track, Road, Railway or Square should be used most of the time
            COT_TransportationObject        = 1ll<< 33,
            
           	// ADD Buildding model 
			COT_IntBuildingInstallation		= 1ll<< 34,

            COT_GenericOccupiedSpace        = 1ll<< 40,
            COT_GenericUnoccupiedSpace      = 1ll<< 41,
            COT_GenericLogicalSpace         = 1ll<< 42,
            COT_GenericThematicSurface      = 1ll<< 43,
            COT_TrafficSpace                = 1ll<< 44,
            COT_AuxiliaryTrafficSpace       = 1ll<< 45,
            COT_Intersection                = 1ll<< 46,
            COT_Section                     = 1ll<< 47,
            COT_Waterway                    = 1ll<< 48,
            COT_BuildingConstructiveElement = 1ll<< 49,
            COT_BuildingRoom = 1ll<< 50,

            COT_All                         = 0xFFFFFFFFFFFFFFFFll
        };

        CityObject( const std::string& id, CityObjectsType type );

        // Get the object type
        CityObjectsType getType() const;
        std::string getTypeAsString() const;

        // Get the number of geometries contains in the object
        unsigned int getGeometriesCount() const;

        // Access the geometries
        const Geometry& getGeometry( unsigned int i ) const;
        Geometry& getGeometry( unsigned int i );

        void addGeometry(Geometry* geom);

        // Get the number of implicit geometries contains in the object
        unsigned int getImplicitGeometryCount() const;

        // Access the implicit geometries
        const ImplicitGeometry& getImplicitGeometry( unsigned int i ) const;
        ImplicitGeometry& getImplicitGeometry( unsigned int i );

        void addImplictGeometry(ImplicitGeometry* implictGeom);

        // Access the children
        unsigned int getChildCityObjectsCount() const;
        const CityObject& getChildCityObject( unsigned int i ) const;
        CityObject& getChildCityObject( unsigned int i );

        void addChildCityObject(CityObject* cityObj);

        // Access address
        const Address* address() const;
        void setAddress(std::unique_ptr<Address>&& address);
        
        // Access rectifiedGridCoverage
        RectifiedGridCoverage const* rectifiedGridCoverage() const;
        void setRectifiedGridCoverage(RectifiedGridCoverage * rectifiedGridCoverage);
        
        // Access externalReference
        ExternalReference const* externalReference() const;
        void setExternalReference(ExternalReference * externalReference);

        void finish(TesselatorBase* tesselator, bool optimize, std::shared_ptr<citygml::CityGMLLogger> logger);

        virtual ~CityObject();

    protected:
        CityObjectsType m_type;

        std::vector<std::unique_ptr<Geometry> > m_geometries;
        std::vector<std::unique_ptr<ImplicitGeometry> > m_implicitGeometries;
        std::vector<std::unique_ptr<CityObject> > m_children;
        std::unique_ptr<Address> m_address;
        std::unique_ptr<RectifiedGridCoverage> m_rectifiedGridCoverage;
        std::unique_ptr<ExternalReference> m_externalReference;
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream& os, const CityObject& o );

    LIBCITYGML_EXPORT std::string cityObjectsTypeToString(const CityObject::CityObjectsType& t);
    LIBCITYGML_EXPORT CityObject::CityObjectsType cityObjectsTypeFromString(const std::string& s, bool& valid);

}

ENUM_CLASS_BITWISE_OPERATORS_DEFS(citygml::CityObject::CityObjectsType);

