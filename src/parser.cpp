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

#include "parser.h"
#include "transform.h"
#include "utils.h"

#ifndef MSVC
	#include <typeinfo>
#endif

using namespace citygml;

std::map<std::string, CityGMLNodeType> CityGMLHandler::s_cityGMLNodeTypeMap;
std::vector< std::string > CityGMLHandler::s_knownNamespace;

CityGMLHandler::CityGMLHandler( const ParserParams& params ) 
: _params( params ), _model( 0 ), _currentCityObject( 0 ), _currentObject( 0 ),
  _currentGeometry( 0 ), _currentComposite( 0 ), _currentImplicitGeometry(0),
  _currentPolygon( 0 ), _currentRing( 0 ),  _currentGeometryType( GT_Unknown ),
_currentAppearance( 0 ), _currentLOD( params.minLOD ), 
_filterNodeType( false ), _filterDepth( 0 ), _exterior( true ), _geoTransform( 0 )
{ 
	_objectsMask = getCityObjectsTypeMaskFromString( _params.objectsMask );
	initNodes(); 
}

CityGMLHandler::~CityGMLHandler( void ) 
{
    for ( std::set<Geometry*>::iterator it = _geometries.begin(); it != _geometries.end(); it++ )
        delete *it;
}

void CityGMLHandler::initNodes( void ) 
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

template<class T> inline void parseValue( std::stringstream &s, T &v ) 
{
	if ( !s.eof() ) s >> v;
}

inline void parseMatrixValue( std::stringstream &s , double* m)
{
    for (size_t i = 0; i < 16; ++i)
    {
        if(s.eof())
            break;

        s >> m[i];
    }
}

template<> inline void parseValue( std::stringstream &s, bool &v ) 
{
	// parsing a bool is special because "true" and "1" are true while "false" and "0" are false
	std::string value = s.str();
	if (value == "1" || value == "true")
		v = true;
	else if (value == "0" || value == "false")
		v = false;
	else
		std::cerr << "Error ! Boolean expected, got " << value << std::endl;
}

template<class T> inline void parseValue( std::stringstream &s, T &v, GeoTransform* transform, const TVec3d &translate ) 
{
	parseValue( s, v );
	
	if ( transform ) transform->transform( v );
	
	// Translate based on bounding box of whole model
	v[0] -= translate[0];
	v[1] -= translate[1];
	v[2] -= translate[2];
}

template<class T> inline void parseVecList( std::stringstream &s, std::vector<T> &vec ) 
{
	T v;
	unsigned int oldSize( vec.size() );
	while ( s >> v )
		vec.push_back( v );
	if ( !s.eof() )
	{
		std::cerr << "Error ! Mismatch type: " << typeid(T).name() << " expected. Ring/Polygon discarded!" << std::endl;
		vec.resize( oldSize );
	}
}

template<class T> inline void parseVecList( std::stringstream &s, std::vector<T> &vec, GeoTransform* transform, const TVec3d &translate ) 
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
		std::cerr << "Error ! Mismatch type: " << typeid(T).name() << " expected. Ring/Polygon discarded!" << std::endl;
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
		_model = new CityModel();
		pushObject( _model );
		break;

		// City objects management
#define MANAGE_OBJECT( _t_ )\
	case CG_ ## _t_ :\
	if ( _objectsMask & COT_ ## _t_ )\
		{ pushCityObject( new _t_( getGmlIdAttribute( attributes ) ) ); pushObject( _currentCityObject ); /*std::cout << "new "<< #_t_ " - " << _currentCityObject->getId() << std::endl;*/ }\
	else { pushCityObject( 0 ); _filterNodeType = true; _filterDepth = getPathDepth(); }\
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
#define MANAGE_SURFACETYPE( _t_ ) case CG_ ## _t_ ## Surface : _currentGeometryType = GT_ ## _t_;\
									if ( _objectsMask & COT_ ## _t_ ## Surface )\
		{ pushCityObject( new _t_ ## Surface( getGmlIdAttribute( attributes ) ) ); pushObject( _currentCityObject ); /*std::cout << "new "<< #_t_ " - " << _currentCityObject->getId() << std::endl;*/ }\
	else { pushCityObject( 0 ); _filterNodeType = true; _filterDepth = getPathDepth(); }\
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
        _currentImplicitGeometry = new ImplicitGeometry("");
        pushObject( _currentImplicitGeometry );

        break;
	case NODETYPE( TexturedSurface ):
	case NODETYPE( OrientableSurface ):
		_orientation = getAttribute( attributes, "orientation", "+" )[0];
		break;

    case NODETYPE( MultiSurface ):
        if (_currentImplicitGeometry)
        {
            _currentImplicitGeometry->_id = getGmlIdAttribute( attributes);
        }
        break;
	case NODETYPE( surfaceMember ):
	case NODETYPE( TriangulatedSurface ):
		LOD_FILTER();
		//_orientation = getAttribute( attributes, "orientation", "+" )[0];
		_orientation = '+';
		_currentGeometry = new Geometry( getGmlIdAttribute( attributes ), _currentGeometryType, _currentLOD );
        _geometries.insert( _currentGeometry );
        pushObject( _currentGeometry );
		break;

	case NODETYPE( CompositeSurface ):
		LOD_FILTER();
		_currentComposite = new Composite( getGmlIdAttribute( attributes ), _currentLOD );
		_composites.insert( _currentComposite );
		pushObject( _currentComposite );
		break;
    case NODETYPE( relativeGMLGeometry ):
        {
            std::string id = getAttribute(attributes, "xlink:href", "");
            if (_currentImplicitGeometry && id != "")
            {
                id = id.substr(1);  // cut of # at the beginning
                std::map<std::string, ImplicitGeometry*>::iterator it = _implicitGeometries.find(id);

                if (it != _implicitGeometries.end())
                {
                    // copy geometry from other implicite geometry

                    ImplicitGeometry* implGeometry = it->second;
                    for (size_t i = 0; i < implGeometry->size(); ++i)
                    {
                        _currentImplicitGeometry->_geometries.push_back(implGeometry->getGeometry(i));
                    }
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
			std::cerr << "Warning ! srsDimension of gml:posList not set to 3!" << std::endl;

		createGeoTransform( getAttribute( attributes, "srsName", "" ) );		
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
				_model->_appearanceManager.assignNode( uri );
				_appearanceAssigned = true;
			}
		}
		break;

	case NODETYPE( textureCoordinates ):
		MODEL_FILTER();
		if ( Texture* texture = dynamic_cast<Texture*>( _currentAppearance ) ) 
		{			
			std::string ring = getAttribute( attributes, "ring" );
			if ( ring != "" )
			{
				if ( ring.length() > 0 && ring[0] == '#' ) ring = ring.substr( 1 );
				_model->_appearanceManager.assignNode( ring );
			}
		}
		break;

	case NODETYPE( SimpleTexture ):
	case NODETYPE( ParameterizedTexture ):
		_currentAppearance = new Texture( getGmlIdAttribute( attributes ) );
		_model->_appearanceManager.addAppearance( _currentAppearance );
		_appearanceAssigned = false;
		pushObject( _currentAppearance );
		break;

	case NODETYPE( GeoreferencedTexture ):
		_currentAppearance = new GeoreferencedTexture( getGmlIdAttribute( attributes ) );
		_model->_appearanceManager.addAppearance( _currentAppearance );
		_appearanceAssigned = false;
		pushObject( _currentAppearance );
		break;

	case NODETYPE( Material ):
	case NODETYPE( X3DMaterial ):
		_currentAppearance = new Material( getGmlIdAttribute( attributes ) );
		_model->_appearanceManager.addAppearance( _currentAppearance );
		_appearanceAssigned = false;
		pushObject( _currentAppearance );
		break;

	case NODETYPE( stringAttribute ):
	case NODETYPE( doubleAttribute ):
	case NODETYPE( intAttribute ):
	case NODETYPE( dateAttribute ):
	case NODETYPE( uriAttribute ):
		_attributeName = getAttribute( attributes, "name", "" );
		break;

	default:
		break;
	};
}

void CityGMLHandler::endElement( const std::string& name ) 
{
	std::string localname = getNodeName( name );

	_nodePath.pop_back();

	CityGMLNodeType nodeType = getNodeTypeFromName( localname );

	if ( NODETYPE_FILTER() ) { clearBuffer(); return; }

	if ( nodeType == NODETYPE( Unknown ) ) // unknown node ? skip now to avoid the buffer triming pass
	{
		clearBuffer();
		return; 
	}

	// Trim the char buffer  
	std::stringstream buffer;
	buffer << trim( _buff.str() );

	// set the LOD level if node name starts with 'lod'
	if ( localname.find( "lod" ) == 0 ) _currentLOD = _params.minLOD;

	switch ( nodeType ) 
	{

    // core:transformationMatrix
    case NODETYPE( transformationMatrix ):
        double elements[16];
        parseMatrixValue( buffer, elements );

        if (_currentImplicitGeometry)
            _currentImplicitGeometry->setMatrix(TransformationMatrix(elements));
        break;
	case NODETYPE( CityModel ):
		MODEL_FILTER();
		_model->finish( _params );
		if ( _geoTransform )
		{
			std::cout << "The coordinates were transformed from " << _model->_srsName << " to "
								<< ((GeoTransform*)_geoTransform)->getDestURN() << std::endl;
			_model->_srsName = ((GeoTransform*)_geoTransform)->getDestURN();
		}
		if ( _model->_srsName == "" )
		{
			_model->_srsName = _params.destSRS;
			std::cerr << "Warning: No SRS was set in the file. The model SRS has been set "
									"without transformation to " << _params.destSRS << std::endl;
		}
		
		_model->_translation = _translate;
		std::cout << std::fixed << "The model coordinates were translated by x:" << _translate.x
				      << " y:" << _translate.y << " z:" << _translate.z << std::endl;
		
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
				_model->_envelope._lowerBound = _points[0];
				_model->_envelope._upperBound = _points[1];
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
				_currentCityObject->_envelope._lowerBound = _points[0];
				_currentCityObject->_envelope._upperBound = _points[1];
			}
		}
		_points.clear();
		break;

	case NODETYPE( lowerCorner ):
	case NODETYPE( upperCorner ):
		{
			TVec3d p;
			parseValue( buffer, p, (GeoTransform*)_geoTransform, _translate );
			if ( nodeType == NODETYPE( lowerCorner ) )
				_points.insert( _points.begin(), p );
			else
				_points.push_back( p );
		}
		break;

	case NODETYPE( lod ):
		parseValue( buffer, _currentLOD );
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
        if ( _currentCityObject && _currentImplicitGeometry )
        {
            _currentCityObject->_implicitGeometries.push_back( _currentImplicitGeometry );
            _implicitGeometries[_currentImplicitGeometry->getId()] = _currentImplicitGeometry;
        } else
            delete _currentImplicitGeometry;
        _currentImplicitGeometry = 0;
        popObject();
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
		}
		_currentPolygon = 0;
		popObject();
		break;

	case NODETYPE( pos ):
		if ( _currentCityObject )
		{
			TVec3d p;
			parseValue( buffer, p, (GeoTransform*)_geoTransform, _translate );
			if ( !_currentPolygon )
				_points.push_back( p );
			else if ( _currentRing )
				_currentRing->addVertex( p );
		}
		break;

	case NODETYPE( coordinates ):
	case NODETYPE( posList ):
		if ( !_currentPolygon ) { parseVecList( buffer, _points, (GeoTransform*)_geoTransform, _translate ); break; }
		_currentPolygon->_negNormal = ( _orientation != '+' );
		if ( _currentRing ) 
			parseVecList( buffer, _currentRing->getVertices(), (GeoTransform*)_geoTransform, _translate );
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
		if ( Texture* texture = dynamic_cast<Texture*>( _currentAppearance ) ) 
		{
			texture->_url = buffer.str();
			std::replace( texture->_url.begin(), texture->_url.end(), '\\', '/' );
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
				_model->_appearanceManager.assignNode( uri );
			}
		}
		break;

	case NODETYPE( textureCoordinates ):
		MODEL_FILTER();
		if ( Texture* texture = dynamic_cast<Texture*>( _currentAppearance ) ) 
		{            
			TexCoords *vec = new TexCoords();
			parseVecList( buffer, *vec );			
			_model->_appearanceManager.assignTexCoords( vec );
		}
		break;

	case NODETYPE( isFront ):
		if ( _currentAppearance )  
		{
			bool val;
			parseValue( buffer, val );
			_currentAppearance->_isFront = val;
		}
		break;

	case NODETYPE( SimpleTexture ):
	case NODETYPE( ParameterizedTexture ):
	case NODETYPE( GeoreferencedTexture ):
	case NODETYPE( Material ):
	case NODETYPE( X3DMaterial ):
		if ( _currentAppearance && _currentGeometry && !_appearanceAssigned )
			_model->_appearanceManager.assignNode( _currentGeometry->getId() );
		_model->_appearanceManager.refresh();
		_currentAppearance = 0;
		popObject();
		break;

	case NODETYPE( diffuseColor ):
	case NODETYPE( emissiveColor ):
	case NODETYPE( specularColor ):
		if ( Material* mat = dynamic_cast<Material*>( _currentAppearance ) ) 
		{
			TVec3f col;
			parseValue( buffer, col );	
			if ( nodeType == NODETYPE( diffuseColor ) ) mat->_diffuse = col;
			else if ( nodeType == NODETYPE( emissiveColor ) ) mat->_emissive = col;
			else if ( nodeType == NODETYPE( specularColor ) ) mat->_specular = col;
		}
		break;

	case NODETYPE( ambientIntensity ):
	case NODETYPE( shininess ):
	case NODETYPE( transparency ):
		if ( Material* mat = dynamic_cast<Material*>( _currentAppearance ) ) 
		{
			float val;
			parseValue( buffer, val );	
			if ( nodeType == NODETYPE( shininess ) ) mat->_shininess = val;
			else if ( nodeType == NODETYPE( transparency ) ) mat->_transparency = val;
			else if ( nodeType == NODETYPE( ambientIntensity ) ) mat->_ambientIntensity = val;
		}
		break;

	case NODETYPE( wrapMode ):
		if ( Texture* texture = dynamic_cast<Texture*>( _currentAppearance ) )             
		{
			std::string s( buffer.str() );
			if ( ci_string_compare( s, "wrap" ) ) texture->_wrapMode = Texture::WM_WRAP;
			else if ( ci_string_compare( s, "mirror" ) ) texture->_wrapMode = Texture::WM_MIRROR;
			else if ( ci_string_compare( s, "clamp" ) ) texture->_wrapMode = Texture::WM_CLAMP;
			else if ( ci_string_compare( s, "border" ) ) texture->_wrapMode = Texture::WM_BORDER;
		}
		break;

	case NODETYPE( borderColor ):
		if ( Texture* texture = dynamic_cast<Texture*>( _currentAppearance ) )  
		{
			std::vector<float> col;
			parseVecList( buffer, col );
			col.push_back( 1.f ); // if 3 values are given, the fourth (A = opacity) is set to 1.0 by default
			if ( col.size() >= 4 )
				memcpy( &texture->_borderColor.r, &col[0], 4 * sizeof(float) );
		}
		break;

	case NODETYPE( preferWorldFile ):
		if ( GeoreferencedTexture* geoRefTexture = dynamic_cast<GeoreferencedTexture*>( _currentAppearance ) )  
		{
			parseValue( buffer, geoRefTexture->_preferWorldFile );
		}
		break;
	default:
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

	if ( srsName != _model->_srsName ) { std::cerr << "Warning: More than one SRS is defined. The SRS " << srsName << " is declared while the scene SRS has been set to " << _model->_srsName << std::endl; /*return;*/ }

	if ( _params.destSRS == "" ) return;
	
	delete (GeoTransform*)_geoTransform;
	_geoTransform = new GeoTransform( proj4Name, _params.destSRS );
}
