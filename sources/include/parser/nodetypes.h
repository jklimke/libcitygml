#pragma once

#include <string>
#include <ostream>
#include <mutex>
#include <unordered_map>

namespace citygml {


    class NodeType {
    public:

        class XMLNode {
        public:
            XMLNode();
            XMLNode(std::string prefix, std::string name);

            const std::string name() const;
            const std::string& prefix() const;
            const std::string& baseName() const;
            int typeID() const;

            bool operator==(const XMLNode& other) const;

            bool operator!=(const XMLNode& other) const;

            bool valid() const;
        private:
            std::string m_name;
            std::string m_prefix;
            int m_typeID;
        };

        static const XMLNode& getXMLNodeFor(const std::string& name);

        static const XMLNode InvalidNode;

        #define NODETYPE( prefix, elementName ) static XMLNode prefix ## _ ## elementName ## Node;

        // CORE
        NODETYPE( CORE, CityModel )
        NODETYPE( CORE, CityObjectMember )
        NODETYPE( CORE, CreationDate )
        NODETYPE( CORE, TerminationDate )
        NODETYPE( CORE, GeneralizesTo)
        NODETYPE( CORE, PointCloud)

        NODETYPE( CORE, ExternalReference)
        NODETYPE( CORE, InformationSystem)
        NODETYPE( CORE, ExternalObject)

        NODETYPE( CORE, Uri)
        NODETYPE( CORE, Name)

        NODETYPE( CORE, Address )
        NODETYPE( CORE, XalAddress )

        NODETYPE( CORE, ImplicitGeometry )
        NODETYPE( CORE, RelativeGMLGeometry )
        NODETYPE( CORE, TransformationMatrix )
        NODETYPE( CORE, ReferencePoint)
        NODETYPE( CORE, MimeType)
        NODETYPE( CORE, LibraryObject)
        NODETYPE( CORE, GenericAttribute)

        NODETYPE( CORE, Lod0MultiSurface)
        NODETYPE( CORE, Lod1MultiSurface)
        NODETYPE( CORE, Lod2MultiSurface)
        NODETYPE( CORE, Lod3MultiSurface)

        NODETYPE( CORE, Boundary )
        NODETYPE( CORE, RelativeGeometry )

        // GRP
        NODETYPE( GRP, CityObjectGroup )
        NODETYPE( GRP, GroupMember )
        NODETYPE( GRP, Class )
        NODETYPE( GRP, Function )
        NODETYPE( GRP, Usage )
        NODETYPE( GRP, Parent )
        NODETYPE( GRP, Geometry )

        // GEN
        NODETYPE( GEN, Class )
        NODETYPE( GEN, Function )
        NODETYPE( GEN, Usage )
        NODETYPE( GEN, GenericCityObject )
        NODETYPE( GEN, StringAttribute )
        NODETYPE( GEN, DoubleAttribute )
        NODETYPE( GEN, IntAttribute )
        NODETYPE( GEN, DateAttribute )
        NODETYPE( GEN, UriAttribute )
        NODETYPE( GEN, Name )
        NODETYPE( GEN, Value )

        NODETYPE( GEN, Lod0Geometry )
        NODETYPE( GEN, Lod1Geometry )
        NODETYPE( GEN, Lod2Geometry )
        NODETYPE( GEN, Lod3Geometry )
        NODETYPE( GEN, Lod4Geometry )
        NODETYPE( GEN, Lod0TerrainIntersection )
        NODETYPE( GEN, Lod1TerrainIntersection )
        NODETYPE( GEN, Lod2TerrainIntersection )
        NODETYPE( GEN, Lod3TerrainIntersection )
        NODETYPE( GEN, Lod4TerrainIntersection )
        NODETYPE( GEN, Lod0ImplicitRepresentation )
        NODETYPE( GEN, Lod1ImplicitRepresentation )
        NODETYPE( GEN, Lod2ImplicitRepresentation )
        NODETYPE( GEN, Lod3ImplicitRepresentation )
        NODETYPE( GEN, Lod4ImplicitRepresentation )

        NODETYPE( GEN, GenericOccupiedSpace )
        NODETYPE( GEN, GenericUnoccupiedSpace )
        NODETYPE( GEN, GenericLogicalSpace )
        NODETYPE( GEN, GenericThematicSurface )

        NODETYPE( GEN, Lod0MultiCurve )
        NODETYPE( GEN, Lod2MultiCurve )
        NODETYPE( GEN, Lod3MultiCurve )
        NODETYPE( GEN, Lod0MultiSurface )
        NODETYPE( GEN, Lod2MultiSurface )
        NODETYPE( GEN, Lod3MultiSurface )

        NODETYPE( GEN, Area )
        NODETYPE( GEN, SpaceType )
        NODETYPE( GEN, Volume )

        // TEX
        // NODETYPE( GML, TexturedSurface ) // Deprecated

        // GML
        NODETYPE( GML, Description )
        NODETYPE( GML, Identifier )
        NODETYPE( GML, Name )
        NODETYPE( GML, DescriptionReference )
        NODETYPE( GML, MetaDataProperty )
        NODETYPE( GML, Coordinates )
        NODETYPE( GML, Pos )
        NODETYPE( GML, BoundedBy )
        NODETYPE( GML, Envelope )
        NODETYPE( GML, LowerCorner )
        NODETYPE( GML, UpperCorner )
        NODETYPE( GML, Solid )
        NODETYPE( GML, SurfaceMember )
        NODETYPE( GML, BaseSurface )
        NODETYPE( GML, Patches )
        NODETYPE( GML, TrianglePatches )
        NODETYPE( GML, SolidMember )
        NODETYPE( GML, TriangulatedSurface )
        NODETYPE( GML, Triangle )
        NODETYPE( GML, Polygon )
        NODETYPE( GML, Rectangle )
        NODETYPE( GML, PosList )
        NODETYPE( GML, OrientableSurface )
        NODETYPE( GML, LinearRing )

        NODETYPE( GML, MultiPoint )
        NODETYPE( GML, MultiCurve )
        NODETYPE( GML, MultiSurface )
        NODETYPE( GML, MultiSolid )
        NODETYPE( GML, MultiGeometry )

        NODETYPE( GML, CompositeCurve )
        NODETYPE( GML, CompositeSurface )
        NODETYPE( GML, CompositeSolid )

        NODETYPE( GML, ReferencePoint )
        NODETYPE( GML, Point )

        NODETYPE( GML, Interior )
        NODETYPE( GML, Exterior )

        NODETYPE( GML, Shell )
        NODETYPE( GML, PolyhedralSurface )
        NODETYPE( GML, Surface )
        NODETYPE( GML, PolygonPatch )

        NODETYPE( GML, LineString )
        
        NODETYPE( GML, RectifiedGridCoverage )

        // BLDG
        NODETYPE( BLDG, Building )
        NODETYPE( BLDG, BuildingPart )
        NODETYPE( BLDG, Room )
        NODETYPE( BLDG, Door )
        NODETYPE( BLDG, Window )
        NODETYPE( BLDG, BuildingInstallation )
        NODETYPE( BLDG, MeasuredHeight )
        NODETYPE( BLDG, Class )
        NODETYPE( BLDG, Type )
        NODETYPE( BLDG, Function )
        NODETYPE( BLDG, Usage )
        NODETYPE( BLDG, YearOfConstruction )
        NODETYPE( BLDG, YearOfDemolition )
        NODETYPE( BLDG, StoreysAboveGround )
        NODETYPE( BLDG, StoreysBelowGround )
        NODETYPE( BLDG, StoreyHeightsAboveGround )
        NODETYPE( BLDG, StoreyHeightsBelowGround )
        NODETYPE( BLDG, BoundedBy )
        NODETYPE( BLDG, OuterBuildingInstallation)
        NODETYPE( BLDG, InteriorBuildingInstallation)
        NODETYPE( BLDG, InteriorRoom)
        NODETYPE( BLDG, InteriorFurniture)
        NODETYPE( BLDG, RoomInstallation)
        NODETYPE( BLDG, Opening)
        NODETYPE( BLDG, RoofType)
        NODETYPE( BLDG, BuildingSubdivision)
        NODETYPE( BLDG, Storey)
        
        NODETYPE( BLDG, Lod0FootPrint )
        NODETYPE( BLDG, Lod0RoofEdge )
        NODETYPE( BLDG, Lod1Solid )
        NODETYPE( BLDG, Lod2Solid )
        NODETYPE( BLDG, Lod3Solid )
        NODETYPE( BLDG, Lod4Solid )
        NODETYPE( BLDG, Lod2Geometry )
        NODETYPE( BLDG, Lod3Geometry )
        NODETYPE( BLDG, Lod4Geometry )
        NODETYPE( BLDG, Lod1MultiCurve )
        NODETYPE( BLDG, Lod2MultiCurve )
        NODETYPE( BLDG, Lod3MultiCurve )
        NODETYPE( BLDG, Lod4MultiCurve )
        NODETYPE( BLDG, Lod1MultiSurface )
        NODETYPE( BLDG, Lod2MultiSurface )
        NODETYPE( BLDG, Lod3MultiSurface )
        NODETYPE( BLDG, Lod4MultiSurface )
        NODETYPE( BLDG, Lod1TerrainIntersection )
        NODETYPE( BLDG, Lod2TerrainIntersection )
        NODETYPE( BLDG, Lod3TerrainIntersection )
        NODETYPE( BLDG, Lod4TerrainIntersection )
        NODETYPE( BLDG, ConsistsOfBuildingPart )

        NODETYPE( CON, FillingSurface )
        NODETYPE( CON, WindowSurface )
        NODETYPE( CON, DoorSurface )
        NODETYPE( CON, OtherConstruction )

        // BoundarySurfaceType
        NODETYPE( BLDG, WallSurface )
        NODETYPE( BLDG, RoofSurface )
        NODETYPE( BLDG, GroundSurface )
        NODETYPE( BLDG, ClosureSurface )
        NODETYPE( BLDG, FloorSurface )
        NODETYPE( BLDG, InteriorWallSurface )
        NODETYPE( BLDG, CeilingSurface )
        NODETYPE( BLDG, OuterCeilingSurface )
        NODETYPE( BLDG, OuterFloorSurface )

        NODETYPE( CON, WallSurface )
        NODETYPE( CON, RoofSurface )
        NODETYPE( CON, GroundSurface )
        NODETYPE( CON, FloorSurface )
        NODETYPE( CON, InteriorWallSurface )
        NODETYPE( CON, CeilingSurface )
        NODETYPE( CON, OuterCeilingSurface )
        NODETYPE( CON, OuterFloorSurface )
        NODETYPE( BLDG, BuildingFurniture )
        NODETYPE( BLDG, IntBuildingInstallation)
        NODETYPE( BLDG, BuildingConstructiveElement)
        NODETYPE( BLDG, BuildingRoom)
            
        NODETYPE( BLDG, CityFurniture )
        NODETYPE( BLDG, Address)

        // CityFurniture
        NODETYPE( FRN, Class )
        NODETYPE( FRN, Function )
        NODETYPE( FRN, CityFurniture )
        NODETYPE( FRN, Lod1Geometry )
        NODETYPE( FRN, Lod2Geometry )
        NODETYPE( FRN, Lod3Geometry )
        NODETYPE( FRN, Lod4Geometry )
        NODETYPE( FRN, Lod1TerrainIntersection )
        NODETYPE( FRN, Lod2TerrainIntersection )
        NODETYPE( FRN, Lod3TerrainIntersection )
        NODETYPE( FRN, Lod4TerrainIntersection )
        NODETYPE( FRN, Lod1ImplicitRepresentation )
        NODETYPE( FRN, Lod2ImplicitRepresentation )
        NODETYPE( FRN, Lod3ImplicitRepresentation )
        NODETYPE( FRN, Lod4ImplicitRepresentation )

        // ADDRESS
        NODETYPE( XAL, AddressDetails )
        NODETYPE( XAL, Country )
        NODETYPE( XAL, CountryName )
        NODETYPE( XAL, CountryNameCode )
        NODETYPE( XAL, AdministrativeArea )
        NODETYPE( XAL, AdministrativeAreaName )
        NODETYPE( XAL, Locality )
        NODETYPE( XAL, LocalityName )
        NODETYPE( XAL, PostalCode )
        NODETYPE( XAL, PostalCodeNumber )
        NODETYPE( XAL, Thoroughfare )
        NODETYPE( XAL, ThoroughfareName )
        NODETYPE( XAL, ThoroughfareNumber )

        // WTR
        NODETYPE( WTR, WaterBody )
        NODETYPE( WTR, WaterSurface )
        NODETYPE( WTR, WaterGroundSurface )
        NODETYPE( WTR, WaterClosureSurface )
        NODETYPE( WTR, Class )
        NODETYPE( WTR, Function )
        NODETYPE( WTR, Usage )
        NODETYPE( WTR, WaterLevel )
        NODETYPE( WTR, Lod0MultiCurve )
        NODETYPE( WTR, Lod0MultiSurface )
        NODETYPE( WTR, Lod1MultiCurve )
        NODETYPE( WTR, Lod1MultiSurface )
        NODETYPE( WTR, Lod1Solid )
        NODETYPE( WTR, Lod2Solid )
        NODETYPE( WTR, Lod3Solid )
        NODETYPE( WTR, Lod4Solid )
        NODETYPE( WTR, Lod2Surface )
        NODETYPE( WTR, Lod3Surface )
        NODETYPE( WTR, Lod4Surface )
        NODETYPE( WTR, BoundedBy )

        // VEG
        NODETYPE( VEG, PlantCover )
        NODETYPE( VEG, SolitaryVegetationObject )
        NODETYPE( VEG, Lod1ImplicitRepresentation )
        NODETYPE( VEG, Lod2ImplicitRepresentation )
        NODETYPE( VEG, Lod3ImplicitRepresentation )
        NODETYPE( VEG, Lod4ImplicitRepresentation )

        NODETYPE( VEG, Class )
        NODETYPE( VEG, Function )
        NODETYPE( VEG, AverageHeight )
        NODETYPE( VEG, Species )
        NODETYPE( VEG, Height )
        NODETYPE( VEG, TrunkDiameter )
        NODETYPE( VEG, CrownDiameter )

        NODETYPE( VEG, Lod0Geometry )
        NODETYPE( VEG, Lod1Geometry )
        NODETYPE( VEG, Lod2Geometry )
        NODETYPE( VEG, Lod3Geometry )
        NODETYPE( VEG, Lod4Geometry )

        // TRANS
        NODETYPE( TRANS, TransportationComplex )
        NODETYPE( TRANS, TrafficArea )
        NODETYPE( TRANS, AuxiliaryTrafficArea )
        NODETYPE( TRANS, TrafficSpace )
        NODETYPE( TRANS, AuxiliaryTrafficSpace )
        NODETYPE( TRANS, Track )
        NODETYPE( TRANS, Road )
        NODETYPE( TRANS, Railway )
        NODETYPE( TRANS, Square )
        NODETYPE( TRANS, Intersection )
        NODETYPE( TRANS, Section )
        NODETYPE( TRANS, Waterway )

        NODETYPE( TRANS, Usage )
        NODETYPE( TRANS, Function )
        NODETYPE( TRANS, SurfaceMaterial )
        NODETYPE( TRANS, Granularity )

        NODETYPE( TRANS, Lod0Network )
        NODETYPE( TRANS, Lod1MultiSurface )
        NODETYPE( TRANS, Lod2MultiSurface )
        NODETYPE( TRANS, Lod3MultiSurface )
        NODETYPE( TRANS, Lod4MultiSurface )


        // LUSE
        NODETYPE( LUSE, LandUse )

        NODETYPE( LUSE, Class )
        NODETYPE( LUSE, Usage )
        NODETYPE( LUSE, Function )

        NODETYPE( LUSE, Lod1MultiSurface )
        NODETYPE( LUSE, Lod2MultiSurface )
        NODETYPE( LUSE, Lod3MultiSurface )
        NODETYPE( LUSE, Lod4MultiSurface )

        // DEM (Relief)
        NODETYPE( DEM, ReliefFeature )
        NODETYPE( DEM, TINRelief )
        NODETYPE( DEM, RasterRelief )
        NODETYPE( DEM, MassPointRelief )
        NODETYPE( DEM, BreaklineRelief )
        NODETYPE( DEM, Lod )
        NODETYPE( DEM, Extent )
        NODETYPE( DEM, ReliefComponent )
        NODETYPE( DEM, Tin )
        NODETYPE( DEM, Grid )
        NODETYPE( DEM, ReliefPoints )
        NODETYPE( DEM, RidgeOrValleyLines )
        NODETYPE( DEM, Breaklines )
        NODETYPE( DEM, Elevation )

        // SUB
        NODETYPE( SUB, Tunnel )
        NODETYPE( SUB, RelativeToTerrain )

        // BRID
        NODETYPE( BRID, Bridge )
        NODETYPE( BRID, BridgeConstructionElement )
        NODETYPE( BRID, BridgeInstallation )
        NODETYPE( BRID, BridgePart )
        NODETYPE( BRID, BoundedBy)
        NODETYPE( BRID, OuterBridgeConstruction )
        NODETYPE( BRID, OuterBridgeInstallation )
        NODETYPE( BRID, Lod1Geometry)
        NODETYPE( BRID, Lod2Geometry)
        NODETYPE( BRID, Lod3Geometry)
        NODETYPE( BRID, Lod4Geometry)

        // APP
        NODETYPE( APP, Appearance )
        NODETYPE( APP, AppearanceMember )
        NODETYPE( APP, SimpleTexture )
        NODETYPE( APP, ParameterizedTexture )
        NODETYPE( APP, GeoreferencedTexture )
        NODETYPE( APP, ImageURI )
        NODETYPE( APP, TextureMap )
        NODETYPE( APP, TextureParameterization )
        NODETYPE( APP, TextureAssociation )
        NODETYPE( APP, Target )
        NODETYPE( APP, TexCoordList )
        NODETYPE( APP, TexCoordGen )
        NODETYPE( APP, TextureCoordinates )
        NODETYPE( APP, Ring )
        NODETYPE( APP, WorldToTexture )
        NODETYPE( APP, TextureType )
        NODETYPE( APP, Repeat )
        NODETYPE( APP, WrapMode )
        NODETYPE( APP, BorderColor )
        NODETYPE( APP, PreferWorldFile )
        NODETYPE( APP, ReferencePoint)
        NODETYPE( APP, Orientation)
        NODETYPE( APP, isSmooth)

        NODETYPE( APP, X3DMaterial )
        NODETYPE( APP, Material )
        NODETYPE( APP, SurfaceDataMember )
        NODETYPE( APP, SurfaceData )
        NODETYPE( APP, Shininess )
        NODETYPE( APP, Transparency )
        NODETYPE( APP, SpecularColor )
        NODETYPE( APP, DiffuseColor )
        NODETYPE( APP, EmissiveColor )
        NODETYPE( APP, AmbientIntensity )
        NODETYPE( APP, IsFront )
        NODETYPE( APP, Theme )
        NODETYPE( APP, MimeType )

    private:
        static void initializeNodeTypes();

        static std::mutex initializedMutex;
        static bool nodesInitialized;
        static int typeCount;
        static std::unordered_map<std::string, XMLNode*> nodeNameTypeMap;
        static std::unordered_map<std::string, XMLNode*> nodeNameWithPrefixTypeMap;
    };

    std::ostream& operator<<( std::ostream& os, const NodeType::XMLNode& o );
}

namespace std {

template <>
struct hash<citygml::NodeType::XMLNode> {
    size_t operator()(const citygml::NodeType::XMLNode& node) const
    {
        return node.typeID();
    }
};

template <>
struct hash<const citygml::NodeType::XMLNode&> {
    size_t operator()(const citygml::NodeType::XMLNode& node) const
    {
        return node.typeID();
    }
};

}  // namespace std
