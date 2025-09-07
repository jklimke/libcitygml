#pragma once

#include <bitset>
#include <vector>
#include <memory>

#include <citygml/featureobject.h>
#include <citygml/citygml_api.h>
#include <citygml/rectifiedgridcoverage.h>
#include <citygml/externalreference.h>
#include <citygml/warnings.h>
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

        enum class CityObjectsType {
            COT_GenericCityObject,
            COT_Building,
            COT_Room,
            COT_BuildingInstallation,
            COT_BuildingFurniture,
            COT_Door,
            COT_Window,
            COT_CityFurniture,
            COT_Track,
            COT_Road,
            COT_Railway,
            COT_Square,
            COT_PlantCover,
            COT_SolitaryVegetationObject,
            COT_WaterBody,
            COT_ReliefFeature,
            COT_ReliefComponent,
            COT_TINRelief,
            COT_MassPointRelief,
            COT_BreaklineRelief,
            COT_RasterRelief,
            COT_LandUse,
            COT_Tunnel,
            COT_Bridge,
            COT_BridgeConstructionElement,
            COT_BridgeInstallation,
            COT_BridgePart,
            COT_BuildingPart,

            COT_WallSurface,
            COT_RoofSurface,
            COT_GroundSurface,
            COT_ClosureSurface,
            COT_FloorSurface,
            COT_InteriorWallSurface,
            COT_CeilingSurface,
            COT_CityObjectGroup,
            COT_OuterCeilingSurface,
            COT_OuterFloorSurface,


            // covers all supertypes of tran::_TransportationObject that are not Track, Road, Railway or Square...
            // there are to many for to few bits to explicitly enumerate them. However Track, Road, Railway or Square should be used most of the time
            COT_TransportationObject,
            
           	// ADD Buildding model 
			COT_IntBuildingInstallation,

            COT_GenericOccupiedSpace,
            COT_GenericUnoccupiedSpace,
            COT_GenericLogicalSpace,
            COT_GenericThematicSurface,
            COT_TrafficSpace,
            COT_AuxiliaryTrafficSpace,
            COT_Intersection,
            COT_Section,
            COT_Waterway,
            COT_BuildingConstructiveElement,
            COT_BuildingRoom,
            COT_FillingSurface,
            COT_WindowSurface,
            COT_DoorSurface,

            COT_All
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

        ~CityObject() override;

    protected:
        CityObjectsType m_type;

        PRAGMA_WARN_DLL_BEGIN
        std::vector<std::unique_ptr<Geometry> > m_geometries;
        std::vector<std::unique_ptr<ImplicitGeometry> > m_implicitGeometries;
        std::vector<std::unique_ptr<CityObject> > m_children;
        std::unique_ptr<Address> m_address;
        std::unique_ptr<RectifiedGridCoverage> m_rectifiedGridCoverage;
        std::unique_ptr<ExternalReference> m_externalReference;
        PRAGMA_WARN_DLL_END
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream& os, const CityObject& o );

    LIBCITYGML_EXPORT std::string cityObjectsTypeToString(const CityObject::CityObjectsType& t);
    LIBCITYGML_EXPORT CityObject::CityObjectsType cityObjectsTypeFromString(const std::string& s, bool& valid);

    using CityObjectsTypeMask = std::bitset<static_cast<size_t>(CityObject::CityObjectsType::COT_All)>;

    LIBCITYGML_EXPORT CityObjectsTypeMask toMask(CityObject::CityObjectsType l);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator|(CityObject::CityObjectsType l, CityObject::CityObjectsType r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator&(CityObject::CityObjectsType l, CityObject::CityObjectsType r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator^(CityObject::CityObjectsType l, CityObject::CityObjectsType r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator~(CityObject::CityObjectsType l);

    LIBCITYGML_EXPORT CityObjectsTypeMask operator|(CityObjectsTypeMask l, CityObject::CityObjectsType r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator&(CityObjectsTypeMask l, CityObject::CityObjectsType r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator^(CityObjectsTypeMask l, CityObject::CityObjectsType r);

    LIBCITYGML_EXPORT CityObjectsTypeMask operator|(CityObject::CityObjectsType l, CityObjectsTypeMask r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator&(CityObject::CityObjectsType l, CityObjectsTypeMask r);
    LIBCITYGML_EXPORT CityObjectsTypeMask operator^(CityObject::CityObjectsType l, CityObjectsTypeMask r);

}

