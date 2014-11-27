/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
*
* This file is part of libcitygml library
* http://code.google.com/p/libcitygml
*
* libcitygml is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2.1 of the License, or
* (at your option) any later version.
*
* libcitygml is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*/

// libcitygml implements a SAX Parser for CityGML v0.3 - v1.0 file format
// See schemas at:
//  http://www.citygml.org/citygml/1/0/0/CityGML.xsd
//  http://www.citygml.org/fileadmin/citygml/docs/CityGML_1_0_0_UML_diagrams.pdf

#include "parser/parser.h"
#include "parser/transform.h"
#include "citygml/utils.h"
#include "citygml/citymodel.h"
#include "citygml/citygmlfactory.h"
#include "citygml/cityobjectdeclerations.hpp"
#include "citygml/landuse.h"
#include "citygml/implictgeometry.h"
#include "citygml/appearancemanager.h"
#include "citygml/appearance.h"

#ifndef MSVC
    #include <typeinfo>
#endif

using namespace citygml;

std::map<std::string, CityGMLNodeType> CityGMLHandler::s_cityGMLNodeTypeMap;
std::vector< std::string > CityGMLHandler::s_knownNamespace;

CityGMLHandler::CityGMLHandler( const ParserParams& params, std::shared_ptr<CityGMLLogger> logger)
: _params( params ), _model( nullptr ), _currentCityObject( nullptr ), _currentObject( nullptr ),
  _currentGeometry( nullptr ), _currentComposite( nullptr ), _currentImplicitGeometry(nullptr),
  _currentPolygon( nullptr ), _currentRing( nullptr ),  _currentGeometryType( Geometry::GeometryType::GT_Unknown ),
_currentAppearance( nullptr ), _currentLOD( params.minLOD ), _currentTheme(""),
_filterNodeType( false ), _filterDepth( 0 ), _exterior( true ), _geoTransform( nullptr ),
  _referencePoint( false ), _logger(logger), _isRelativeGeometry(false)
{
    _objectsMask = _params.objectsMask;
    initNodes();

    if (_logger == nullptr) {
        std::runtime_error("CityGMLHandler: expected a valid CityGMLLogger object for parameter 'logger' but got nullptr.");
    }
}

void CityGMLHandler::initNodes()
{
    if ( s_cityGMLNodeTypeMap.size() != 0 ) return;

#define INSERTNODETYPE(_t_) s_cityGMLNodeTypeMap[ #_t_ ] = CG_ ## _t_;

    // core
    INSERTNODETYPE( CityModel );
    INSERTNODETYPE( cityObjectMember );
    INSERTNODETYPE( creationDate );
    INSERTNODETYPE( terminationDate );
    INSERTNODETYPE( ImplicitGeometry );
    INSERTNODETYPE( relativeGMLGeometry );
    INSERTNODETYPE( transformationMatrix );

    // grp
    INSERTNODETYPE( CityObjectGroup );
    INSERTNODETYPE( groupMember );

    // gen
    INSERTNODETYPE( GenericCityObject );
    INSERTNODETYPE( stringAttribute );
    INSERTNODETYPE( doubleAttribute );
    INSERTNODETYPE( intAttribute );
    INSERTNODETYPE( dateAttribute );
    INSERTNODETYPE( uriAttribute );
    INSERTNODETYPE( value );

    // gml
    INSERTNODETYPE( name );
    INSERTNODETYPE( pos );
    INSERTNODETYPE( coordinates );
    INSERTNODETYPE( description );
    INSERTNODETYPE( boundedBy );
    INSERTNODETYPE( Envelope );
    INSERTNODETYPE( lowerCorner );
    INSERTNODETYPE( upperCorner );
    INSERTNODETYPE( Solid );
    INSERTNODETYPE( surfaceMember );
    INSERTNODETYPE( TriangulatedSurface );
    INSERTNODETYPE( TexturedSurface );
    INSERTNODETYPE( Triangle );
    INSERTNODETYPE( Polygon );
    INSERTNODETYPE( posList );
    INSERTNODETYPE( OrientableSurface );
    INSERTNODETYPE( LinearRing );

    INSERTNODETYPE( lod1Solid );
    INSERTNODETYPE( lod2Solid );
    INSERTNODETYPE( lod3Solid );
    INSERTNODETYPE( lod4Solid );
    INSERTNODETYPE( lod1Geometry );
    INSERTNODETYPE( lod2Geometry );
    INSERTNODETYPE( lod3Geometry );
    INSERTNODETYPE( lod4Geometry );
    INSERTNODETYPE( lod1MultiCurve );
    INSERTNODETYPE( lod2MultiCurve );
    INSERTNODETYPE( lod3MultiCurve );
    INSERTNODETYPE( lod4MultiCurve );
    INSERTNODETYPE( lod1MultiSurface );
    INSERTNODETYPE( lod2MultiSurface );
    INSERTNODETYPE( lod3MultiSurface );
    INSERTNODETYPE( lod4MultiSurface );
    INSERTNODETYPE( lod1TerrainIntersection );
    INSERTNODETYPE( lod2TerrainIntersection );
    INSERTNODETYPE( lod3TerrainIntersection );
    INSERTNODETYPE( lod4TerrainIntersection );

    INSERTNODETYPE( MultiPoint );
    INSERTNODETYPE( MultiCurve );
    INSERTNODETYPE( MultiSurface );
    INSERTNODETYPE( MultiSolid );

    INSERTNODETYPE( CompositeCurve );
    INSERTNODETYPE( CompositeSurface );
    INSERTNODETYPE( CompositeSolid );

    INSERTNODETYPE( referencePoint );
    INSERTNODETYPE( Point );

    // bldg
    INSERTNODETYPE( Building );
    INSERTNODETYPE( BuildingPart );
    INSERTNODETYPE( Room );
    INSERTNODETYPE( Door );
    INSERTNODETYPE( Window );
    INSERTNODETYPE( BuildingInstallation );
    INSERTNODETYPE( address );
    INSERTNODETYPE( measuredHeight );
    INSERTNODETYPE( class );
    INSERTNODETYPE( type );
    INSERTNODETYPE( function );
    INSERTNODETYPE( usage );
    INSERTNODETYPE( yearOfConstruction );
    INSERTNODETYPE( yearOfDemolition );
    INSERTNODETYPE( storeysAboveGround );
    INSERTNODETYPE( storeysBelowGround );
    INSERTNODETYPE( storeyHeightsAboveGround );
    INSERTNODETYPE( storeyHeightsBelowGround );

    // address
    INSERTNODETYPE( administrativearea );
    INSERTNODETYPE( country );
    INSERTNODETYPE( code );
    INSERTNODETYPE( street );
    INSERTNODETYPE( postalCode );
    INSERTNODETYPE( city );

    // BoundarySurfaceType
    INSERTNODETYPE( WallSurface );
    INSERTNODETYPE( RoofSurface );
    INSERTNODETYPE( GroundSurface );
    INSERTNODETYPE( ClosureSurface );
    INSERTNODETYPE( FloorSurface );
    INSERTNODETYPE( InteriorWallSurface );
    INSERTNODETYPE( CeilingSurface );
    INSERTNODETYPE( BuildingFurniture );

    INSERTNODETYPE( CityFurniture );

    INSERTNODETYPE( interior );
    INSERTNODETYPE( exterior );

    // wtr
    INSERTNODETYPE( WaterBody );

    // veg
    INSERTNODETYPE( PlantCover );
    INSERTNODETYPE( SolitaryVegetationObject );
    INSERTNODETYPE( species );

    INSERTNODETYPE( lod1ImplicitRepresentation );
    INSERTNODETYPE( lod2ImplicitRepresentation );
    INSERTNODETYPE( lod3ImplicitRepresentation );
    INSERTNODETYPE( lod4ImplicitRepresentation );

    // trans
    INSERTNODETYPE( TrafficArea );
    INSERTNODETYPE( AuxiliaryTrafficArea );
    INSERTNODETYPE( Track );
    INSERTNODETYPE( Road );
    INSERTNODETYPE( Railway );
    INSERTNODETYPE( Square );

    // luse
    INSERTNODETYPE( LandUse );

    // dem
    INSERTNODETYPE( lod );
    INSERTNODETYPE( TINRelief );

    // sub
    INSERTNODETYPE( Tunnel );
    INSERTNODETYPE( relativeToTerrain );

    // brid
    INSERTNODETYPE( Bridge );
    INSERTNODETYPE( BridgeConstructionElement );
    INSERTNODETYPE( BridgeInstallation );
    INSERTNODETYPE( BridgePart );

    // app
    INSERTNODETYPE( SimpleTexture );
    INSERTNODETYPE( ParameterizedTexture );
    INSERTNODETYPE( GeoreferencedTexture );
    INSERTNODETYPE( imageURI );
    INSERTNODETYPE( textureMap );
    INSERTNODETYPE( target );
    INSERTNODETYPE( textureCoordinates );
    INSERTNODETYPE( textureType );
    INSERTNODETYPE( repeat );
    INSERTNODETYPE( wrapMode );
    INSERTNODETYPE( borderColor );
    INSERTNODETYPE( preferWorldFile );

    INSERTNODETYPE( X3DMaterial );
    INSERTNODETYPE( Material );
    INSERTNODETYPE( appearanceMember );
    INSERTNODETYPE( surfaceDataMember );
    INSERTNODETYPE( shininess );
    INSERTNODETYPE( transparency );
    INSERTNODETYPE( specularColor );
    INSERTNODETYPE( diffuseColor );
    INSERTNODETYPE( emissiveColor );
    INSERTNODETYPE( ambientIntensity );
    INSERTNODETYPE( isFront );
    INSERTNODETYPE( theme );

    // Set the known namespaces

#define INSERTKNOWNNAMESPACE(_t_) s_knownNamespace.push_back( #_t_ );

    INSERTKNOWNNAMESPACE( gml );
    INSERTKNOWNNAMESPACE( citygml );
    INSERTKNOWNNAMESPACE( core );
    INSERTKNOWNNAMESPACE( app );
    INSERTKNOWNNAMESPACE( bldg );
    INSERTKNOWNNAMESPACE( frn );
    INSERTKNOWNNAMESPACE( grp );
    INSERTKNOWNNAMESPACE( gen );
    INSERTKNOWNNAMESPACE( luse );
    INSERTKNOWNNAMESPACE( dem );
    INSERTKNOWNNAMESPACE( tran );
    INSERTKNOWNNAMESPACE( trans );
    INSERTKNOWNNAMESPACE( veg );
    INSERTKNOWNNAMESPACE( wtr );
    INSERTKNOWNNAMESPACE( tex );
    INSERTKNOWNNAMESPACE( sub );
    INSERTKNOWNNAMESPACE( brid );
}

CityGMLNodeType CityGMLHandler::getNodeTypeFromName( const std::string& name )
{
    std::map<std::string, CityGMLNodeType>::const_iterator elt = s_cityGMLNodeTypeMap.find( name );

    if ( elt == s_cityGMLNodeTypeMap.end() ) return CG_Unknown;

    return elt->second;
}

///////////////////////////////////////////////////////////////////////////////
// Helpers

template<class T> void parseValue( std::stringstream &s, T &v, std::shared_ptr<CityGMLLogger>)
{
    if ( !s.eof() ) s >> v;
}

void parseMatrixValue( std::stringstream &s , double* m)
{
    for (size_t i = 0; i < 16; ++i)
    {
        if(s.eof())
            break;

        s >> m[i];
    }
}

template<> void parseValue( std::stringstream &s, bool &v, std::shared_ptr<CityGMLLogger> logger )
{
    // parsing a bool is special because "true" and "1" are true while "false" and "0" are false
    std::string value = s.str();
    if (value == "1" || value == "true")
        v = true;
    else if (value == "0" || value == "false")
        v = false;
    else
        CITYGML_LOG_ERROR(logger, "Boolean expected, got " << value);
}

template<class T> void parseValue( std::stringstream &s, T &v, GeoTransform* transform, const TVec3d &translate, std::shared_ptr<CityGMLLogger> logger )
{
    parseValue( s, v, logger );

    if ( transform ) transform->transform( v );

    // Translate based on bounding box of whole model
    v[0] -= translate[0];
    v[1] -= translate[1];
    v[2] -= translate[2];
}

template<class T> void parseVecList( std::stringstream &s, std::vector<T> &vec, std::shared_ptr<CityGMLLogger> logger )
{
    T v;
    unsigned int oldSize( vec.size() );
    while ( s >> v )
        vec.push_back( v );
    if ( !s.eof() )
    {
        CITYGML_LOG_ERROR(logger, "Mismatch type: " << typeid(T).name() << " expected. Ring/Polygon discarded!");
        vec.resize( oldSize );
    }
}

template<class T> void parseVecList( std::stringstream &s, std::vector<T> &vec, GeoTransform* transform, const TVec3d &translate, std::shared_ptr<CityGMLLogger> logger)
{
    T v;
    unsigned int oldSize( vec.size() );
    while ( s >> v )
    {
        if ( transform ) transform->transform( v );

        // Translate based on bounding box of whole model
        v[0] -= translate[0];
        v[1] -= translate[1];
        v[2] -= translate[2];

        vec.push_back( v );
    }
    if ( !s.eof() )
    {
        CITYGML_LOG_ERROR(logger, "Mismatch type: " << typeid(T).name() << " expected. Ring/Polygon discarded!");
        vec.resize( oldSize );
    }
}

///////////////////////////////////////////////////////////////////////////////

std::string CityGMLHandler::getNodeName( const std::string& name )
{
    // remove the known namespace if it exists

    size_t pos = name.find_first_of( ":" );
    if ( pos == std::string::npos ) return name;

    std::string nspace = name.substr( 0, pos );

    for ( int i = s_knownNamespace.size() - 1; i >= 0; i-- )
        if ( nspace == s_knownNamespace[i] )
            return name.substr( s_knownNamespace[i].length() + 1 );

    return name;
}

void CityGMLHandler::startElement( const std::string& name, void* attributes )
{
    std::string localname = getNodeName( name );

    _nodePath.push_back( localname );

    CityGMLNodeType nodeType = getNodeTypeFromName( localname );

    std::string currentGMLID = getGmlIdAttribute(attributes);

    _cityGMLidStack.push_back(currentGMLID);


    // get the LOD level if node name starts with 'lod'
    if ( localname.length() > 3 && localname.find( "lod" ) == 0 )
        _currentLOD = localname[3] - '0';

#define LOD_FILTER() if ( _currentLOD < (int)_params.minLOD || _currentLOD > (int)_params.maxLOD ) break;

#define NODETYPE_FILTER() ( _filterNodeType && getPathDepth() > _filterDepth )

#define MODEL_FILTER() if ( !_model ) break;

    if ( NODETYPE_FILTER() ) return;

    switch ( nodeType )
    {
    case NODETYPE( CityModel ):
        _model = _citygmlFactory->createCityModel(currentGMLID);
        pushObject( _model );
        break;

        // City objects management
#define MANAGE_OBJECT( _t_ )\
    case CG_ ## _t_ :\
        if ( _objectsMask & CityObject::COT_ ## _t_ ) { \
             pushCityObject( _citygmlFactory->createCityObject<_t_>( currentGMLID ) ); \
             pushObject( _currentCityObject ); \
        }\
        else {\
            pushCityObject( 0 ); \
            _filterNodeType = true; \
            _filterDepth = getPathDepth();\
        }\
    break;

        MANAGE_OBJECT( GenericCityObject );
        MANAGE_OBJECT( Building );
        MANAGE_OBJECT( BuildingPart );
        MANAGE_OBJECT( Room );
        MANAGE_OBJECT( BuildingInstallation );
        MANAGE_OBJECT( BuildingFurniture );
        MANAGE_OBJECT( Door );
        MANAGE_OBJECT( Window );
        MANAGE_OBJECT( CityFurniture );
        MANAGE_OBJECT( Track );
        MANAGE_OBJECT( Road );
        MANAGE_OBJECT( Railway );
        MANAGE_OBJECT( Square );
        MANAGE_OBJECT( PlantCover );
        MANAGE_OBJECT( SolitaryVegetationObject );
        MANAGE_OBJECT( WaterBody );
        MANAGE_OBJECT( TINRelief );
        MANAGE_OBJECT( LandUse );
        MANAGE_OBJECT( Tunnel );
        MANAGE_OBJECT( Bridge );
        MANAGE_OBJECT( BridgeConstructionElement );
        MANAGE_OBJECT( BridgeInstallation );
        MANAGE_OBJECT( BridgePart );
#undef MANAGE_OBJECT

        // BoundarySurfaceType
#define MANAGE_SURFACETYPE( _t_ ) \
    case CG_ ## _t_ ## Surface : \
        _currentGeometryType = Geometry::GeometryType::GT_ ## _t_;\
        if ( _objectsMask & CityObject::CityObjectsType::COT_ ## _t_ ## Surface ) {\
            pushCityObject( _citygmlFactory->createCityObject<_t_ ## Surface >( currentGMLID ) ); \
            pushObject( _currentCityObject ); }\
        else { \
            pushCityObject( 0 );\
            _filterNodeType = true;\
            _filterDepth = getPathDepth();\
        }\
    break;

        MANAGE_SURFACETYPE( Wall );
        MANAGE_SURFACETYPE( Roof );
        MANAGE_SURFACETYPE( Ground );
        MANAGE_SURFACETYPE( Closure );
        MANAGE_SURFACETYPE( Floor );
        MANAGE_SURFACETYPE( InteriorWall );
        MANAGE_SURFACETYPE( Ceiling );
#undef MANAGE_SURFACETYPE
        // Geometry management

    case NODETYPE( ImplicitGeometry ):
        _currentImplicitGeometry = _citygmlFactory->createImplictGeometry(currentGMLID);
        break;

    case NODETYPE( referencePoint ):
        _referencePoint = true;
        break;
    case NODETYPE( TexturedSurface ):
    case NODETYPE( OrientableSurface ):
        _orientation = getAttribute( attributes, "orientation", "+" )[0];
        // Fall through intended
    case NODETYPE( MultiSurface ):
    case NODETYPE( CompositeSurface ):
    case NODETYPE( TriangulatedSurface ):
        Geometry* parentGeometry = _currentGeometry;
        _currentGeometry = _citygmlFactory->createGeometry(currentGMLID, _currentGeometryType, _currentLOD );

        if (parent != nullptr) {
           parentGeometry->addGeometry(_currentGeometry);
        } else if (_isRelativeGeometry) {
            if (_currentImplicitGeometry != nullptr) {
                std::shared_ptr<Geometry> sharedGeometry = std::make_shared<Geometry>(_currentGeometry);
                _relativeGeometries[_currentGeometry->getId()] = sharedGeometry;
                _currentImplicitGeometry->addGeometry(sharedGeometry);
            } else {
                CITYGML_LOG_WARN(_logger, "Found relative Geometry with id '" << _currentGeometry->getId() << "' outside of an ImpilicitGeometry... possible memory leak");
            }
        }

        _geometryStack.push(_currentGeometry);
        pushObject( _currentGeometry );

        break;
    case NODETYPE( surfaceDataMember ):
    {
        // Surface Data Members are the children of app::Appearance objects and contain appearance surfacedata (e.g.  app::ParameterizedTexture)
        std::string relativeID = getAttribute(attributes, "xlink:href", "");
        if (relativeID != "") {

            // The surface data member references a surfacedata object that was defined in another app::Appearance object

            if ( relativeID[0] == '#' ) {
                relativeID = relativeID.substr( 1 );
            }

            std::shared_ptr<Appearance> appearance = _appearanceManager->getAppearanceByID(relativeID);
            if (appearance != nullptr) {
                appearance->addToTheme(_currentTheme);
            } else {
                CITYGML_LOG_WARN(_logger, "surfaceDataMember with invalid xlink reference to non existing object with id " << relativeID);
            }
        }
        break;
    }
    case NODETYPE( surfaceMember ):
        LOD_FILTER();
        _orientation = '+';
        break;

    case NODETYPE( relativeGMLGeometry ):
        {
            _isRelativeGeometry = true;
            std::string id = getAttribute(attributes, "xlink:href", "");
            if (_currentImplicitGeometry && id != "") {
                auto it = _relativeGeometries.find(id);

                if (it != _relativeGeometries.end()) {
                    _currentImplicitGeometry->addGeometry(it->second);
                } else {
                    CITYGML_LOG_WARN(_logger, "Relative Geometry with id '" << id << "' not found.")
                }

            }
        }
        break;
    case NODETYPE( Triangle ):
    case NODETYPE( Polygon ):
        LOD_FILTER();
        _currentPolygon = new Polygon( getGmlIdAttribute( attributes ) );

        pushObject( _currentPolygon );
        break;

    case NODETYPE( Envelope ):
        createGeoTransform( getAttribute( attributes, "srsName", "" ) );
        break;

    case NODETYPE( posList ):
        LOD_FILTER();
        _srsDimension = atoi( getAttribute( attributes, "srsDimension", "3" ).c_str() );
        if ( _srsDimension != 3 )
            CITYGML_LOG_WARN(_logger, "srsDimension of gml:posList not set to 3!");

        createGeoTransform( getAttribute( attributes, "srsName", "" ) );
        if (_currentImplicitGeometry)
            _currentImplicitGeometry->_srsName = _geoTransform->getSourceURN();

        break;

    case NODETYPE( interior ): _exterior = false; break;
    case NODETYPE( exterior ): _exterior = true;  break;

    case NODETYPE( LinearRing ):
        LOD_FILTER();
        _currentRing = new LinearRing( getGmlIdAttribute( attributes ), _exterior );
        pushObject( _currentRing );
        break;

        // Material management

    case NODETYPE( target ):
        if ( _currentAppearance )
        {
            std::string uri = getAttribute( attributes, "uri" );
            if ( uri != "" )
            {
                if ( uri.length() > 0 && uri[0] == '#' ) uri = uri.substr( 1 );
                getApperanceManagerForCurrentTheme().assignNode( uri );
                _appearanceAssigned = true;
            }
        }
        break;

    case NODETYPE( textureCoordinates ):
        MODEL_FILTER();
        if ( std::dynamic_pointer_cast<Texture>( _currentAppearance ) )
        {
            std::string ring = getAttribute( attributes, "ring" );
            if ( ring != "" )
            {
                if ( ring.length() > 0 && ring[0] == '#' ) ring = ring.substr( 1 );
                getApperanceManagerForCurrentTheme().assignNode( ring );
            }
        }
        break;

    case NODETYPE( SimpleTexture ):
    case NODETYPE( ParameterizedTexture ):
        _currentAppearance = std::make_shared<Texture>( getGmlIdAttribute( attributes ) );
        getApperanceManagerForCurrentTheme().addAppearance( _currentAppearance );
        _appearanceAssigned = false;
        pushObject( _currentAppearance.get() );
        break;

    case NODETYPE( GeoreferencedTexture ):
        _currentAppearance = std::make_shared<GeoreferencedTexture>( getGmlIdAttribute( attributes ) );
        getApperanceManagerForCurrentTheme().addAppearance( _currentAppearance );
        _appearanceAssigned = false;
        pushObject( _currentAppearance.get() );
        break;

    case NODETYPE( Material ):
    case NODETYPE( X3DMaterial ):
        _currentAppearance = std::make_shared<Material>( getGmlIdAttribute( attributes ) );
        getApperanceManagerForCurrentTheme().addAppearance( _currentAppearance );
        _appearanceAssigned = false;
        pushObject( _currentAppearance.get() );
        break;

    case NODETYPE( stringAttribute ):
    case NODETYPE( doubleAttribute ):
    case NODETYPE( intAttribute ):
    case NODETYPE( dateAttribute ):
    case NODETYPE( uriAttribute ):
        _attributeName = getAttribute( attributes, "name", "" );
        break;
    default:
        CITYGML_LOG_INFO(_logger, "Ignored start tag of element " << localname);
        break;
    };
}

void CityGMLHandler::endElement( const std::string& name )
{
    std::string localname = getNodeName( name );

    _nodePath.pop_back();
    _cityGMLidStack.pop_back();

    CityGMLNodeType nodeType = getNodeTypeFromName( localname );

    if ( NODETYPE_FILTER() ) { clearBuffer(); return; }

    if ( nodeType == NODETYPE( Unknown ) ) // unknown node ? skip now to avoid the buffer triming pass
    {
        clearBuffer();
        return;
    }

    // Trim the char buffer
    std::string valueStr = trim(_buff.str());
    std::stringstream buffer;
    buffer << trim( _buff.str() );

    // set the LOD level if node name starts with 'lod'
    if ( localname.find( "lod" ) == 0 ) _currentLOD = _params.minLOD;

    switch ( nodeType )
    {

    // core:transformationMatrix
    case NODETYPE( transformationMatrix ):
        if (_currentImplicitGeometry)
        {
            double elements[16];
            parseMatrixValue( buffer, elements );
            _currentImplicitGeometry->setMatrix(TransformationMatrix(elements));
        }
        break;
    case NODETYPE( referencePoint ):
        _referencePoint = false;
        break;
    case NODETYPE( CityModel ):
        MODEL_FILTER();
        _model->finish( _params, _logger );
        if ( _geoTransform )
        {
            CITYGML_LOG_INFO(_logger, "The coordinates were transformed from " << _model->_srsName << " to " << _geoTransform->getDestURN());
            _model->_srsName = _geoTransform->getDestURN();
        }
        if ( _model->_srsName == "" )
        {
            _model->_srsName = _params.destSRS;
            CITYGML_LOG_WARN(_logger, "No SRS was set in the file. The model SRS has been set without transformation to " << _params.destSRS);
        }

        _model->_translation = _translate;
        CITYGML_LOG_INFO(_logger, std::fixed << "The model coordinates were translated by x:" << _translate.x << " y:" << _translate.y << " z:" << _translate.z);

        popObject();
        break;

        // City objects management

    case NODETYPE( GenericCityObject ):
    case NODETYPE( Building ):
    case NODETYPE( BuildingPart ):
    case NODETYPE( Room ):
    case NODETYPE( BuildingInstallation ):
    case NODETYPE( BuildingFurniture ):
    case NODETYPE( Door ):
    case NODETYPE( Window ):
    case NODETYPE( CityFurniture ):
    case NODETYPE( Track ):
    case NODETYPE( Road ):
    case NODETYPE( Railway ):
    case NODETYPE( Square ):
    case NODETYPE( PlantCover ):
    case NODETYPE( SolitaryVegetationObject ):
    case NODETYPE( WaterBody ):
    case NODETYPE( TINRelief ):
    case NODETYPE( LandUse ):
    case NODETYPE( Tunnel ):
    case NODETYPE( Bridge ):
    case NODETYPE( BridgeConstructionElement ):
    case NODETYPE( BridgeInstallation ):
    case NODETYPE( BridgePart ):
    case NODETYPE( WallSurface ):
    case NODETYPE( RoofSurface ):
    case NODETYPE( GroundSurface ):
    case NODETYPE( ClosureSurface ):
    case NODETYPE( FloorSurface ):
    case NODETYPE( InteriorWallSurface ):
    case NODETYPE( CeilingSurface ):
        MODEL_FILTER();
        if ( _currentCityObject && ( _currentCityObject->size() > 0 || _currentCityObject->getChildCount() > 0 || !_params.pruneEmptyObjects ) )
        {	// Prune empty objects
            _model->addCityObject( _currentCityObject );
            if ( _cityObjectStack.size() == 1 ) _model->addCityObjectAsRoot( _currentCityObject );
        }
        else delete _currentCityObject;
        popCityObject();
        popObject();
        _filterNodeType = false;
        _currentGeometryType = GT_Unknown;
        break;

    case NODETYPE( Envelope ):
        MODEL_FILTER();
        if ( _points.size() >= 2 )
        {
            if ( getPathDepth() == 2 ) // CityModel envelope
            {
                _model->_envelope = Envelope(_points[0], _points[1]);
                /*
                // Translation works only if model as an envelope on CityModel set
                // It is assumed that the envelope is correct and valid
                // If there is no envelope set the translation parameters are zero
                // and no translation of the model will be applied -> but also no
                // correct tesselation will be possible
                _translate[0] = _model->_envelope._lowerBound.x;
                _translate[1] = _model->_envelope._lowerBound.y;
                _translate[2] = _model->_envelope._lowerBound.z;

                // Possible optimization 1: Make a first scan through whole document
                // and compute correct envelope if no one is present. Afterwards
                // start the real parsing.

                // Possible optimization 2: Implement scaling so the model coordinates
                // are between 0 and 1.

                // Currently not implemented: The Citygml object model should have its
                // real coordinates in dest SRS after parsing. Currently the translation
                // parameters are applied to the coordinates in dest SRS. What needs still
                // to be implemented is that after tesselation the coordinates are translated
                // back. Only for visualisation e.g. via OSG the saved translation parameters
                // should be applied before creation of OSG geometry.

                // The envelope is already transformed to destination SRS
                // so we now translation parameters in dest SRS and recompute envelope
                _model->_envelope._lowerBound = _model->_envelope._lowerBound - _translate;
                _model->_envelope._upperBound = _model->_envelope._upperBound - _translate;*/
            }
            else if ( _currentCityObject )
            {
                _currentCityObject->_envelope = Envelope(_points[0], _points[1]);
            }
        }
        _points.clear();
        break;

    case NODETYPE( lowerCorner ):
    case NODETYPE( upperCorner ):
        {
            TVec3d p;
            parseValue( buffer, p, _geoTransform, _translate, _logger );
            if ( nodeType == NODETYPE( lowerCorner ) )
                _points.insert( _points.begin(), p );
            else
                _points.push_back( p );
        }
        break;

    case NODETYPE( lod ):
        parseValue( buffer, _currentLOD, _logger );
        break;

    case NODETYPE( name ):
    case NODETYPE( description ):
        if ( _currentCityObject ) _currentCityObject->setAttribute( localname, buffer.str() );
        else if ( _model && getPathDepth() == 1 ) _model->setAttribute( localname, buffer.str() );
        break;

    case NODETYPE( class ):
    case NODETYPE( type ):
    case NODETYPE( function ):
    case NODETYPE( usage ):
    case NODETYPE( yearOfConstruction ):
    case NODETYPE( yearOfDemolition ):
    case NODETYPE( storeysAboveGround ):
    case NODETYPE( storeysBelowGround ):
    case NODETYPE( storeyHeightsAboveGround ):
    case NODETYPE( storeyHeightsBelowGround ):
    case NODETYPE( administrativearea ):
    case NODETYPE( country ):
    case NODETYPE( code ):
    case NODETYPE( street ):
    case NODETYPE( postalCode ):
    case NODETYPE( city ):
    case NODETYPE( measuredHeight ):
    case NODETYPE( creationDate ):
    case NODETYPE( terminationDate ):
        if ( _currentObject ) _currentObject->setAttribute( localname, buffer.str(), false );
        break;

    case NODETYPE( value ):
        if ( _attributeName != "" && _currentObject )
        {
            if ( _currentObject ) _currentObject->setAttribute( _attributeName, buffer.str(), false );
            else if ( _model && getPathDepth() == 1 ) _model->setAttribute( _attributeName, buffer.str(), false );
        }
        break;

        // Geometry management

    case NODETYPE( ImplicitGeometry ):
        _insideImplicitGeometry = false; // This is save because ImplicitGeometries can not be nested
        if ( _currentCityObject && _currentImplicitGeometry )
        {
            _currentCityObject->_implicitGeometries.push_back( _currentImplicitGeometry );
            _implicitGeometries[_currentImplicitGeometry->getId()] = _currentImplicitGeometry;
        } else
            delete _currentImplicitGeometry;
        _currentImplicitGeometry = 0;
        popObject();
        break;

    case NODETYPE( relativeGMLGeometry ):
        _isRelativeGeometry = false;
        break;

    case NODETYPE( surfaceMember ):
    case NODETYPE( TriangulatedSurface ):
        if ( _currentCityObject && _currentGeometry )
        {
            if ( _currentComposite )
                _currentGeometry->_composite = _currentComposite;

            if ( _currentImplicitGeometry )
                _currentImplicitGeometry->_geometries.push_back( _currentGeometry );
            else
                _currentCityObject->_geometries.push_back( _currentGeometry );
        }
        else
            delete _currentGeometry;
        _geometries.erase( _currentGeometry );
        _currentGeometry = 0;
        popObject();
        break;

    case NODETYPE( CompositeSurface ):
        if ( _currentCityObject && _currentComposite )
            _currentCityObject->_composites.push_back( _currentComposite );
        else
            delete _currentComposite;
        _composites.erase( _currentComposite );
        _currentComposite = 0;
        popObject();
        break;

    case NODETYPE( Triangle ):
    case NODETYPE( Polygon ):
        if ( _currentGeometry && _currentPolygon )
        {
            //_currentPolygon->finish( ( nodeType == NODETYPE( Triangle ) ) ? false : _params.tesselate );
            _currentGeometry->addPolygon( _currentPolygon );
            // Assign the apperances that are currenty assigned to parent elements of this polygon to it
            _model->reassignNodeToAllAppearances(_currentPolygon, _cityGMLidStack);
        }

        _currentPolygon = 0;
        popObject();
        break;

    case NODETYPE( pos ):
        if ( _currentCityObject )
        {
            TVec3d p;
            parseValue( buffer, p, _geoTransform, _translate, _logger );
            if (_referencePoint && _currentImplicitGeometry)
                _currentImplicitGeometry->setReferencePoint(p);
            else if ( !_currentPolygon )
                _points.push_back( p );
            else if ( _currentRing )
                _currentRing->addVertex( p );
        }
        break;

    case NODETYPE( coordinates ):
    case NODETYPE( posList ):
        if ( !_currentPolygon ) { parseVecList( buffer, _points, _geoTransform, _translate, _logger ); break; }
        _currentPolygon->_negNormal = ( _orientation != '+' );
        if (_currentImplicitGeometry && _currentRing)
            parseVecList( buffer, _currentRing->getVertices(), _logger);
        else if ( _currentRing )
            parseVecList( buffer, _currentRing->getVertices(), _geoTransform, _translate, _logger );
        break;

    case NODETYPE( interior ):
    case NODETYPE( exterior ): _exterior = true; break;

    case NODETYPE( LinearRing ):
        if ( _currentPolygon && _currentRing )
            _currentPolygon->addRing( _currentRing );
        _currentRing = 0;
        break;

        // Material management

    case NODETYPE( textureMap ):
    case NODETYPE( imageURI ):
        if ( std::shared_ptr<Texture> texture = currentAppearanceAsTexture() )
        {
            std::string url;
            std::replace( url.begin(), url.end(), '\\', '/' );
            texture->setUrl(url);

        }
        break;

    case NODETYPE( target ):
        MODEL_FILTER();
        if ( _currentAppearance && !_appearanceAssigned )
        {
            std::string uri = buffer.str();
            if ( uri != "" )
            {
                if ( uri.length() > 0 && uri[0] == '#' ) uri = uri.substr( 1 );
                getApperanceManagerForCurrentTheme().assignNode( uri );
            }
        }
        break;

    case NODETYPE( textureCoordinates ):
        MODEL_FILTER();
        if ( auto texture = currentAppearanceAsTexture() )
        {
            TexCoords *vec = new TexCoords();
            parseVecList( buffer, *vec, _logger );
            getApperanceManagerForCurrentTheme().assignTexCoords( vec );
        }
        break;

    case NODETYPE( isFront ):
        if ( _currentAppearance )
        {
            bool val;
            parseValue( buffer, val, _logger );
            _currentAppearance->setIsFront(val);
        }
        break;

    case NODETYPE( SimpleTexture ):
    case NODETYPE( ParameterizedTexture ):
    case NODETYPE( GeoreferencedTexture ):
    case NODETYPE( Material ):
    case NODETYPE( X3DMaterial ):
        if ( _currentAppearance && _currentGeometry && !_appearanceAssigned )
            getApperanceManagerForCurrentTheme().assignNode( _currentGeometry->getId() );
        getApperanceManagerForCurrentTheme().refresh();
        _currentAppearance = 0;
        popObject();
        break;

    case NODETYPE( diffuseColor ):
    case NODETYPE( emissiveColor ):
    case NODETYPE( specularColor ):
        if ( std::shared_ptr<Material> mat = currentAppearanceAsMaterial() )
        {
            TVec3f col;
            parseValue( buffer, col, _logger );
            if ( nodeType == NODETYPE( diffuseColor ) ) mat->setDiffuse(col);
            else if ( nodeType == NODETYPE( emissiveColor ) ) mat->setEmissive(col);
            else if ( nodeType == NODETYPE( specularColor ) ) mat->setSpecular(col);
        }
        break;

    case NODETYPE( ambientIntensity ):
    case NODETYPE( shininess ):
    case NODETYPE( transparency ):
        if ( std::shared_ptr<Material> mat = currentAppearanceAsMaterial() )
        {
            float val;
            parseValue( buffer, val, _logger );
            if ( nodeType == NODETYPE( shininess ) ) mat->setShininess(val);
            else if ( nodeType == NODETYPE( transparency ) ) mat->setTransparency(val);
            else if ( nodeType == NODETYPE( ambientIntensity ) ) mat->setAmbientIntensity(val);
        }
        break;

    case NODETYPE( wrapMode ):
        if ( std::shared_ptr<Texture> texture = currentAppearanceAsTexture() )
        {
            std::string s( buffer.str() );
            if ( ci_string_compare( s, "wrap" ) ) texture->setWrapMode(Texture::WrapMode::WM_WRAP);
            else if ( ci_string_compare( s, "mirror" ) ) texture->setWrapMode(Texture::WrapMode::WM_MIRROR);
            else if ( ci_string_compare( s, "clamp" ) ) texture->setWrapMode(Texture::WrapMode::WM_CLAMP);
            else if ( ci_string_compare( s, "border" ) ) texture->setWrapMode(Texture::WrapMode::WM_BORDER);
        }
        break;

    case NODETYPE( borderColor ):
        if ( std::shared_ptr<Texture> texture = currentAppearanceAsTexture() )
        {
            std::vector<float> col;
            parseVecList( buffer, col, _logger );
            col.push_back( 1.f ); // if 3 values are given, the fourth (A = opacity) is set to 1.0 by default
            if ( col.size() >= 4 ) {
                texture->setBorderColor(TVec4f(col[0], col[1], col[2], col[3]));
            }
        }
        break;

    case NODETYPE( preferWorldFile ):
        if ( std::shared_ptr<GeoreferencedTexture> geoRefTexture = currentAppearanceAsGeoreferencedTexture() )
        {
            bool value;
            parseValue( buffer, value, _logger );
            geoRefTexture->setPreferWorldFile(value);
        }
        break;
    case NODETYPE( theme ):
        _currentTheme = valueStr;
        break;
    default:
        CITYGML_LOG_INFO(_logger, "Ignored start end of element " << localname);
        break;
    };

    clearBuffer();
}

void CityGMLHandler::createGeoTransform( std::string srsName )
{
    if ( srsName == "" ) return;

    // Support SRS pattern like:
    //	urn:EPSG:geographicCRS:4326
    // 	urn:ogc:def:crs:EPSG:6.6:4326
    //	http://www.opengis.net/gml/srs/epsg.xml#4326
    //	http://www.epsg.org/6.11.2/4326
    //	EPSG:4326

    bool latlon = false;

    std::string proj4Name = srsName;

    if ( srsName.find( "urn:" ) == 0 )
    {
        // Manage URN composition but retain only the first SRS
        // ie. transform: urn:ogc:def:crs,crs:EPSG:6.12:3068,crs:EPSG:6.12:5783
        // to urn:ogc:def:crs:EPSG:6.12:3068
        std::vector<std::string> tokens = tokenize( srsName, "," );
        if ( tokens.size() > 1 )
        {
            std::string::size_type p = tokens[1].find( ':' );
            proj4Name = ( p != std::string::npos ) ? tokens[0] + tokens[1].substr( p ) : srsName = tokens[0] + tokens[1];
        }
        latlon = true;
    }
    else if ( srsName.find( "http://www.opengis.net/gml/srs/epsg.xml#" ) != std::string::npos )
        proj4Name = "EPSG:" + srsName.substr( srsName.find_last_of( '#' ) + 1 );

    else if ( srsName.find( "http://www.epsg.org/" ) != std::string::npos )
        proj4Name = "EPSG:" + srsName.substr( srsName.find_last_of( '/' ) + 1 );

    if ( _model->_srsName == "" ) _model->_srsName = srsName;

    if ( srsName != _model->_srsName ) {
        CITYGML_LOG_WARN(_logger, "More than one SRS is defined. The SRS " << srsName << " is declared while the scene SRS has been set to " << _model->_srsName);
    }



    if ( _params.destSRS == "" ) return;
    if(_geoTransform) delete _geoTransform;
    _geoTransform = new GeoTransform( proj4Name, _params.destSRS, _logger );
}

AppearanceManager& CityGMLHandler::getApperanceManagerForCurrentTheme() {
    return _model->getOrCreateAppearanceManager(_currentTheme, _logger);
}

std::shared_ptr<Material> CityGMLHandler::currentAppearanceAsMaterial() {
    return std::dynamic_pointer_cast<Material>( _currentAppearance );
}

std::shared_ptr<Texture> CityGMLHandler::currentAppearanceAsTexture() {
    return std::dynamic_pointer_cast<Texture>( _currentAppearance );
}

std::shared_ptr<GeoreferencedTexture> CityGMLHandler::currentAppearanceAsGeoreferencedTexture() {
    return std::dynamic_pointer_cast<GeoreferencedTexture>( _currentAppearance );
}

void CityGMLHandler::pushCityObject(CityObject *object) {
    if ( _currentCityObject && object ) _currentCityObject->addChildCityObject(object);
    _cityObjectStack.push( _currentCityObject );
    _currentCityObject = object;
}

void CityGMLHandler::popCityObject() {
    _currentObject = 0;
    if ( _objectStack.empty() ) return;
    _objectStack.pop();
    _currentObject = _objectStack.empty() ? 0 : _objectStack.top();
}
