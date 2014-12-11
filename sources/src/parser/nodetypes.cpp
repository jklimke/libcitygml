#include "parser/nodetypes.h"
#include "citygml/utils.h"

namespace citygml {

    int NodeType::typeCount = -1;
    std::unordered_map<std::string, NodeType::XMLNode*> NodeType::nodeNameTypeMap;
    std::unordered_map<std::string, NodeType::XMLNode*> NodeType::nodeNameWithPrefixTypeMap;

    NodeType::XMLNode::XMLNode(std::string prefix, std::string name ): m_name(toLower(name)), m_prefix(toLower(prefix))
    {
        NodeType::nodeNameTypeMap[m_name] = this;
        NodeType::nodeNameWithPrefixTypeMap[m_prefix + ":" + m_name] = this;
        m_typeID = NodeType::typeCount++;
    }

    const std::string&NodeType::XMLNode::name() const
    {
        return m_name;
    }

    const std::string&NodeType::XMLNode::prefix() const
    {
        return m_prefix;
    }

    int NodeType::XMLNode::typeID() const
    {
        return m_typeID;
    }

    bool NodeType::XMLNode::operator==(const NodeType::XMLNode& other) const
    {
        return typeID() == other.typeID();
    }

    bool NodeType::XMLNode::operator!=(const NodeType::XMLNode& other) const
    {
        return !(*this == other);
    }

    bool NodeType::XMLNode::valid() const
    {
        return !m_name.empty();
    }

    const NodeType::XMLNode&NodeType::getXMLNodeFor(const std::string& name)
    {
        std::string lowerName = toLower(name);
        {
            auto it = nodeNameWithPrefixTypeMap.find(lowerName);
            if (it != nodeNameWithPrefixTypeMap.end()) {
                return *it->second;
            }
        }

        std::string nodeName = lowerName;

        size_t pos = name.find_first_of( ":" );
        if ( pos != std::string::npos ) {
            nodeName = name.substr(pos);
        }

        auto it = nodeNameTypeMap.find(nodeName);

        if (it == nodeNameTypeMap.end()) {
            return InvalidNode;
        } else {
            return *it->second;
        }
    }

    std::ostream& operator<<(std::ostream& os, const NodeType::XMLNode& o)
    {
        if (!o.valid()) {
            os << "InvalidNode";
        } else {
            if (!o.prefix().empty()) {
                os << o.prefix() << ":";
            }
            os << o.name();
        }

        return os;
    }

    const NodeType::XMLNode NodeType::InvalidNode = XMLNode("", "");

#define DEFINE_NODE( prefix, elementname ) const NodeType::XMLNode NodeType::prefix ## _ ## elementname ## Node = XMLNode( #prefix , #elementname );

    // CORE
    DEFINE_NODE( CORE, CityModel )
    DEFINE_NODE( CORE, CityObjectMember )
    DEFINE_NODE( CORE, CreationDate )
    DEFINE_NODE( CORE, TerminationDate )
    DEFINE_NODE( CORE, ImplicitGeometry )
    DEFINE_NODE( CORE, RelativeGMLGeometry )
    DEFINE_NODE( CORE, TransformationMatrix )

    // GRP
    DEFINE_NODE( GRP, CityObjectGroup )
    DEFINE_NODE( GRP, GroupMember )

    // GEN
    DEFINE_NODE( GEN, GenericCityObject )
    DEFINE_NODE( GEN, StringAttribute )
    DEFINE_NODE( GEN, DoubleAttribute )
    DEFINE_NODE( GEN, IntAttribute )
    DEFINE_NODE( GEN, DateAttribute )
    DEFINE_NODE( GEN, UriAttribute )
    DEFINE_NODE( GEN, Value )

    DEFINE_NODE( GEN, Lod1Geometry )
    DEFINE_NODE( GEN, Lod2Geometry )
    DEFINE_NODE( GEN, Lod3Geometry )
    DEFINE_NODE( GEN, Lod4Geometry )
    DEFINE_NODE( GEN, Lod1TerrainIntersection )
    DEFINE_NODE( GEN, Lod2TerrainIntersection )
    DEFINE_NODE( GEN, Lod3TerrainIntersection )
    DEFINE_NODE( GEN, Lod4TerrainIntersection )

    // TEX
    // DEFINE_NODE( GML, TexturedSurface ) // Deprecated

    // GML
    DEFINE_NODE( GML, Description )
    DEFINE_NODE( GML, Identifier )
    DEFINE_NODE( GML, Name )
    DEFINE_NODE( GML, DescriptionReference )
    DEFINE_NODE( GML, Coordinates )
    DEFINE_NODE( GML, Pos )
    DEFINE_NODE( GML, BoundedBy )
    DEFINE_NODE( GML, Envelope )
    DEFINE_NODE( GML, LowerCorner )
    DEFINE_NODE( GML, UpperCorner )
    DEFINE_NODE( GML, Solid )
    DEFINE_NODE( GML, SurfaceMember )
    DEFINE_NODE( GML, BaseSurface )
    DEFINE_NODE( GML, Patches )
    DEFINE_NODE( GML, TrianglePatches )
    DEFINE_NODE( GML, SolidMember )
    DEFINE_NODE( GML, TriangulatedSurface )
    DEFINE_NODE( GML, Triangle )
    DEFINE_NODE( GML, Polygon )
    DEFINE_NODE( GML, Rectangle )
    DEFINE_NODE( GML, PosList )
    DEFINE_NODE( GML, OrientableSurface )
    DEFINE_NODE( GML, LinearRing )

    DEFINE_NODE( BLDG, Lod1Solid )
    DEFINE_NODE( BLDG, Lod2Solid )
    DEFINE_NODE( BLDG, Lod3Solid )
    DEFINE_NODE( BLDG, Lod4Solid )
    DEFINE_NODE( BLDG, Lod1Geometry )
    DEFINE_NODE( BLDG, Lod2Geometry )
    DEFINE_NODE( BLDG, Lod3Geometry )
    DEFINE_NODE( BLDG, Lod4Geometry )
    DEFINE_NODE( BLDG, Lod1MultiCurve )
    DEFINE_NODE( BLDG, Lod2MultiCurve )
    DEFINE_NODE( BLDG, Lod3MultiCurve )
    DEFINE_NODE( BLDG, Lod4MultiCurve )
    DEFINE_NODE( BLDG, Lod1MultiSurface )
    DEFINE_NODE( BLDG, Lod2MultiSurface )
    DEFINE_NODE( BLDG, Lod3MultiSurface )
    DEFINE_NODE( BLDG, Lod4MultiSurface )
    DEFINE_NODE( BLDG, Lod1TerrainIntersection )
    DEFINE_NODE( BLDG, Lod2TerrainIntersection )
    DEFINE_NODE( BLDG, Lod3TerrainIntersection )
    DEFINE_NODE( BLDG, Lod4TerrainIntersection )

    DEFINE_NODE( GML, MultiPoint )
    DEFINE_NODE( GML, MultiCurve )
    DEFINE_NODE( GML, MultiSurface )
    DEFINE_NODE( GML, MultiSolid )

    DEFINE_NODE( GML, CompositeCurve )
    DEFINE_NODE( GML, CompositeSurface )
    DEFINE_NODE( GML, CompositeSolid )

    DEFINE_NODE( GML, ReferencePoint )
    DEFINE_NODE( GML, Point )

    DEFINE_NODE( GML, Interior )
    DEFINE_NODE( GML, Exterior )

    // BLDG
    DEFINE_NODE( BLDG, Building )
    DEFINE_NODE( BLDG, BuildingPart )
    DEFINE_NODE( BLDG, Room )
    DEFINE_NODE( BLDG, Door )
    DEFINE_NODE( BLDG, Window )
    DEFINE_NODE( BLDG, BuildingInstallation )
    DEFINE_NODE( BLDG, Address )
    DEFINE_NODE( BLDG, MeasuredHeight )
    DEFINE_NODE( BLDG, Class )
    DEFINE_NODE( BLDG, Type )
    DEFINE_NODE( BLDG, Function )
    DEFINE_NODE( BLDG, Usage )
    DEFINE_NODE( BLDG, YearOfConstruction )
    DEFINE_NODE( BLDG, YearOfDemolition )
    DEFINE_NODE( BLDG, StoreysAboveGround )
    DEFINE_NODE( BLDG, StoreysBelowGround )
    DEFINE_NODE( BLDG, StoreyHeightsAboveGround )
    DEFINE_NODE( BLDG, StoreyHeightsBelowGround )
    DEFINE_NODE( BLDG, BoundedBy )
    DEFINE_NODE( BLDG, OuterBuildingInstallation)
    DEFINE_NODE( BLDG, InteriorBuildingInstallation)
    DEFINE_NODE( BLDG, InteriorRoom)
    DEFINE_NODE( BLDG, InteriorFurniture)
    DEFINE_NODE( BLDG, RoomInstallation)
    DEFINE_NODE( BLDG, Opening)
    DEFINE_NODE( BLDG, ConsistsOfBuildingPart )

    // CityFurniture
    DEFINE_NODE( FRN, CityFurniture )
    DEFINE_NODE( FRN, Lod1Geometry )
    DEFINE_NODE( FRN, Lod2Geometry )
    DEFINE_NODE( FRN, Lod3Geometry )
    DEFINE_NODE( FRN, Lod4Geometry )
    DEFINE_NODE( FRN, Lod1TerrainIntersection )
    DEFINE_NODE( FRN, Lod2TerrainIntersection )
    DEFINE_NODE( FRN, Lod3TerrainIntersection )
    DEFINE_NODE( FRN, Lod4TerrainIntersection )
    DEFINE_NODE( FRN, Lod1ImplicitRepresentation )
    DEFINE_NODE( FRN, Lod2ImplicitRepresentation )
    DEFINE_NODE( FRN, Lod3ImplicitRepresentation )
    DEFINE_NODE( FRN, Lod4ImplicitRepresentation )

    // BoundarySurfaceType
    DEFINE_NODE( BLDG, WallSurface )
    DEFINE_NODE( BLDG, RoofSurface )
    DEFINE_NODE( BLDG, GroundSurface )
    DEFINE_NODE( BLDG, ClosureSurface )
    DEFINE_NODE( BLDG, FloorSurface )
    DEFINE_NODE( BLDG, InteriorWallSurface )
    DEFINE_NODE( BLDG, CeilingSurface )
    DEFINE_NODE( BLDG, BuildingFurniture )
    DEFINE_NODE( BLDG, RoofType)
    DEFINE_NODE( BLDG, ExternalReference)
    DEFINE_NODE( BLDG, InformationSystem)
    DEFINE_NODE( BLDG, ExternalObject)
    DEFINE_NODE( BLDG, Uri)

    DEFINE_NODE( BLDG, CityFurniture )

    // ADDRESS
    DEFINE_NODE( XAL, XalAddress )
    DEFINE_NODE( XAL, Administrativearea )
    DEFINE_NODE( XAL, Country )
    DEFINE_NODE( XAL, CountryName )
    DEFINE_NODE( XAL, Code )
    DEFINE_NODE( XAL, Street )
    DEFINE_NODE( XAL, PostalCode )
    DEFINE_NODE( XAL, City )
    DEFINE_NODE( XAL, LocalityName )
    DEFINE_NODE( XAL, Thoroughfare )
    DEFINE_NODE( XAL, ThoroughfareNumber )
    DEFINE_NODE( XAL, ThoroughfareName )
    DEFINE_NODE( XAL, Locality )
    DEFINE_NODE( XAL, AddressDetails )
    DEFINE_NODE( XAL, DependentLocalityName )
    // WTR
    DEFINE_NODE( WTR, WaterBody )

    // VEG
    DEFINE_NODE( VEG, PlantCover )
    DEFINE_NODE( VEG, SolitaryVegetationObject )
    DEFINE_NODE( VEG, Species )
    DEFINE_NODE( VEG, Lod1ImplicitRepresentation )
    DEFINE_NODE( VEG, Lod2ImplicitRepresentation )
    DEFINE_NODE( VEG, Lod3ImplicitRepresentation )
    DEFINE_NODE( VEG, Lod4ImplicitRepresentation )

    // TRANS
    DEFINE_NODE( TRANS, TrafficArea )
    DEFINE_NODE( TRANS, AuxiliaryTrafficArea )
    DEFINE_NODE( TRANS, Track )
    DEFINE_NODE( TRANS, Road )
    DEFINE_NODE( TRANS, Railway )
    DEFINE_NODE( TRANS, Square )

    // LUSE
    DEFINE_NODE( LUSE, LandUse )

    // dem
    DEFINE_NODE( LUSE, Lod )
    DEFINE_NODE( LUSE, TINRelief )

    // SUB
    DEFINE_NODE( SUB, Tunnel )
    DEFINE_NODE( SUB, RelativeToTerrain )

    // BRID
    DEFINE_NODE( BRID, Bridge )
    DEFINE_NODE( BRID, BridgeConstructionElement )
    DEFINE_NODE( BRID, BridgeInstallation )
    DEFINE_NODE( BRID, BridgePart )

    // APP
    DEFINE_NODE( APP, Appearance )
    DEFINE_NODE( APP, SimpleTexture )
    DEFINE_NODE( APP, ParameterizedTexture )
    DEFINE_NODE( APP, GeoreferencedTexture )
    DEFINE_NODE( APP, ImageURI )
    DEFINE_NODE( APP, TextureMap )
    DEFINE_NODE( APP, Target )
    DEFINE_NODE( APP, TexCoordList )
    DEFINE_NODE( APP, TextureCoordinates )
    DEFINE_NODE( APP, TextureType )
    DEFINE_NODE( APP, Repeat )
    DEFINE_NODE( APP, WrapMode )
    DEFINE_NODE( APP, BorderColor )
    DEFINE_NODE( APP, PreferWorldFile )

    DEFINE_NODE( APP, X3DMaterial )
    DEFINE_NODE( APP, Material )
    DEFINE_NODE( APP, AppearanceMember )
    DEFINE_NODE( APP, SurfaceDataMember )
    DEFINE_NODE( APP, Shininess )
    DEFINE_NODE( APP, Transparency )
    DEFINE_NODE( APP, SpecularColor )
    DEFINE_NODE( APP, DiffuseColor )
    DEFINE_NODE( APP, EmissiveColor )
    DEFINE_NODE( APP, AmbientIntensity )
    DEFINE_NODE( APP, IsFront )
    DEFINE_NODE( APP, Theme )
    DEFINE_NODE( APP, MimeType )

}
