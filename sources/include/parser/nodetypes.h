#pragma once

#include <string>
#include <ostream>

#include <unordered_map>

namespace citygml {


    class NodeType {
    public:

        class XMLNode {
        public:
            XMLNode(std::string prefix, std::string name);

            const std::string& name() const;
            const std::string& prefix() const;
            int typeID() const;

            bool operator==(const XMLNode& other) const;

            bool operator!=(const XMLNode& other) const;

            bool valid() const;
        private:
            std::string m_name;
            std::string m_prefix;
            int m_typeID;
        };

        static const XMLNode InvalidNode;

        static const XMLNode& getXMLNodeFor(const std::string& name);

        #define NODETYPE( prefix, elementName ) static const XMLNode prefix ## _ ## elementName ## Node;

        // CORE
        NODETYPE( CORE, CityModel )
        NODETYPE( CORE, CityObjectMember )
        NODETYPE( CORE, CreationDate )
        NODETYPE( CORE, TerminationDate )
        NODETYPE( CORE, ImplicitGeometry )
        NODETYPE( CORE, RelativeGMLGeometry )
        NODETYPE( CORE, TransformationMatrix )

        // GRP
        NODETYPE( GRP, CityObjectGroup )
        NODETYPE( GRP, GroupMember )

        // GEN
        NODETYPE( GEN, GenericCityObject )
        NODETYPE( GEN, StringAttribute )
        NODETYPE( GEN, DoubleAttribute )
        NODETYPE( GEN, IntAttribute )
        NODETYPE( GEN, DateAttribute )
        NODETYPE( GEN, UriAttribute )
        NODETYPE( GEN, Value )

        // TEX
        // NODETYPE( GML, TexturedSurface ) // Deprecated

        // GML
        NODETYPE( GML, Description )
        NODETYPE( GML, Identifier )
        NODETYPE( GML, Name )
        NODETYPE( GML, DescriptionReference )
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

        NODETYPE( GML, CompositeCurve )
        NODETYPE( GML, CompositeSurface )
        NODETYPE( GML, CompositeSolid )

        NODETYPE( GML, ReferencePoint )
        NODETYPE( GML, Point )

        NODETYPE( GML, Interior )
        NODETYPE( GML, Exterior )

        // BLDG
        NODETYPE( BLDG, Building )
        NODETYPE( BLDG, BuildingPart )
        NODETYPE( BLDG, Room )
        NODETYPE( BLDG, Door )
        NODETYPE( BLDG, Window )
        NODETYPE( BLDG, BuildingInstallation )
        NODETYPE( BLDG, Address )
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
        NODETYPE( BLDG, ExternalReference)
        NODETYPE( BLDG, InformationSystem)
        NODETYPE( BLDG, ExternalObject)
        NODETYPE( BLDG, Uri)

        NODETYPE( BLDG, Lod1Solid )
        NODETYPE( BLDG, Lod2Solid )
        NODETYPE( BLDG, Lod3Solid )
        NODETYPE( BLDG, Lod4Solid )
        NODETYPE( BLDG, Lod1Geometry )
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

        // BoundarySurfaceType
        NODETYPE( BLDG, WallSurface )
        NODETYPE( BLDG, RoofSurface )
        NODETYPE( BLDG, GroundSurface )
        NODETYPE( BLDG, ClosureSurface )
        NODETYPE( BLDG, FloorSurface )
        NODETYPE( BLDG, InteriorWallSurface )
        NODETYPE( BLDG, CeilingSurface )
        NODETYPE( BLDG, BuildingFurniture )

        NODETYPE( BLDG, CityFurniture )

        // ADDRESS
        NODETYPE( ADDRESS, Administrativearea )
        NODETYPE( ADDRESS, Country )
        NODETYPE( ADDRESS, Code )
        NODETYPE( ADDRESS, Street )
        NODETYPE( ADDRESS, PostalCode )
        NODETYPE( ADDRESS, City )
        // WTR
        NODETYPE( WTR, WaterBody )

        // VEG
        NODETYPE( VEG, PlantCover )
        NODETYPE( VEG, SolitaryVegetationObject )
        NODETYPE( VEG, Species )
        NODETYPE( VEG, Lod1ImplicitRepresentation )
        NODETYPE( VEG, Lod2ImplicitRepresentation )
        NODETYPE( VEG, Lod3ImplicitRepresentation )
        NODETYPE( VEG, Lod4ImplicitRepresentation )

        // TRANS
        NODETYPE( TRANS, TrafficArea )
        NODETYPE( TRANS, AuxiliaryTrafficArea )
        NODETYPE( TRANS, Track )
        NODETYPE( TRANS, Road )
        NODETYPE( TRANS, Railway )
        NODETYPE( TRANS, Square )

        // LUSE
        NODETYPE( LUSE, LandUse )

        // dem
        NODETYPE( LUSE, Lod )
        NODETYPE( LUSE, TINRelief )

        // SUB
        NODETYPE( SUB, Tunnel )
        NODETYPE( SUB, RelativeToTerrain )

        // BRID
        NODETYPE( BRID, Bridge )
        NODETYPE( BRID, BridgeConstructionElement )
        NODETYPE( BRID, BridgeInstallation )
        NODETYPE( BRID, BridgePart )

        // APP
        NODETYPE( APP, Appearance )
        NODETYPE( APP, AppearanceMember )
        NODETYPE( APP, SimpleTexture )
        NODETYPE( APP, ParameterizedTexture )
        NODETYPE( APP, GeoreferencedTexture )
        NODETYPE( APP, ImageURI )
        NODETYPE( APP, TextureMap )
        NODETYPE( APP, Target )
        NODETYPE( APP, TexCoordList )
        NODETYPE( APP, TextureCoordinates )
        NODETYPE( APP, TextureType )
        NODETYPE( APP, Repeat )
        NODETYPE( APP, WrapMode )
        NODETYPE( APP, BorderColor )
        NODETYPE( APP, PreferWorldFile )

        NODETYPE( APP, X3DMaterial )
        NODETYPE( APP, Material )
        NODETYPE( APP, SurfaceDataMember )
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
        static int typeCount;
        static std::unordered_map<std::string, XMLNode*> nodeNameTypeMap;
        static std::unordered_map<std::string, XMLNode*> nodeNameWithPrefixTypeMap;
    };

    std::ostream& operator<<( std::ostream& os, const NodeType::XMLNode& o );
}
