/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
 *
 * Contributors:
 *  - Manuel Garnier, BRGM - better normal computation
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

#include "tesselator.h"
#include "citygml.h"
#include "utils.h"
#include <float.h>
#include <string.h>

#include <iterator>
#include <set>

#ifndef min
#	define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

///////////////////////////////////////////////////////////////////////////////

namespace citygml
{
	std::ostream& operator<<( std::ostream& os, const Envelope& e ) 
	{
		return os << e.getLowerBound() << " " << e.getUpperBound();
	}

	std::ostream& operator<<( std::ostream& os, const Object& o ) 
	{ 
		return os << o.getId();
	}

	std::ostream& operator<<( std::ostream& os, const citygml::Geometry& s )
	{
		unsigned int count = 0;
		for ( unsigned int i = 0; i < s.size(); i++ )
		{
			os << *s[i];
			count += s[i]->getVertices().size();
		}

		os << "  @ " << s._polygons.size() << " polys [" << count << " vertices]" << std::endl;

		return os;
	}

	std::ostream& operator<<( std::ostream& os, const CityObject& o ) 
	{
		os << o.getType() << ": " << o.getId() << std::endl;
		os << "  Envelope: " << o.getEnvelope() << std::endl;

		AttributesMap::const_iterator it = o._attributes.begin();
		while ( it != o._attributes.end() )
		{
			os << "  + " << it->first << ": " << it->second << std::endl;
			it++;
		}

		std::vector< Geometry* >::const_iterator itp = o._geometries.begin();
		for ( ; itp != o._geometries.end(); itp++ ) 
			os << **itp;

		os << "  * " << o._geometries.size() << " geometries." << std::endl;

		return os;
	}

	std::ostream& operator<<( std::ostream& out, const CityModel& model ) 
	{
		const CityObjectsMap& cityObjectsMap = model.getCityObjectsMap();

		CityObjectsMap::const_iterator it = cityObjectsMap.begin();

		for ( ; it != cityObjectsMap.end(); ++it )

			for ( unsigned int i = 0; i < it->second.size(); i++ ) out << *(it->second[i]);

		out << model.size() << " city objects." << std::endl;	

		return out;
	}

	///////////////////////////////////////////////////////////////////////////////

	TVec3d LinearRing::computeNormal( void ) const
	{
		unsigned int len = size();
		if ( len < 3 ) return TVec3d();

		// Tampieri, F. 1992. Newell's method for computing the plane equation of a polygon. In Graphics Gems III, pp.231-232. 
		TVec3d n( 0., 0., 0. );
		for ( unsigned int i = 0; i < len; i++ )
		{
			const TVec3d& current = _vertices[i];
			const TVec3d& next = _vertices[ ( i + 1 ) % len];

			n.x += ( current.y - next.y ) * ( current.z + next.z );
			n.y += ( current.z - next.z ) * ( current.x + next.x );
			n.z += ( current.x - next.x ) * ( current.y + next.y );
		}
		return n.normal();
	}

	void LinearRing::finish( TexCoords* texCoords )
	{
		// Remove duplicated vertex
		unsigned int len = _vertices.size();
		if ( len < 2 ) return;

		for ( unsigned int i = 0; i < len; i++ )
		{
            if ( ( _vertices[i] - _vertices[ ( i + 1 ) % len ] ).sqrLength() <= DBL_EPSILON )
			{
				_vertices.erase( _vertices.begin() + i );
				if ( texCoords && texCoords->size() > i) texCoords->erase( texCoords->begin() + i );
				finish( texCoords );
				return;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	AppearanceManager::AppearanceManager( void ) : _lastId( "" ), _lastCoords( 0 ) 
	{
		_tesselator = new ::Tesselator();
	}

	AppearanceManager::~AppearanceManager( void ) 
	{
		for ( unsigned int i = 0; i < _appearances.size(); i++ ) delete _appearances[i];

		std::set<TexCoords*> texCoords;
		for ( std::map<std::string, TexCoords*>::iterator it = _texCoordsMap.begin(); it != _texCoordsMap.end(); ++it )
		{
			if ( it->second && texCoords.find(it->second) == texCoords.end() ) 
			{
				texCoords.insert(it->second);
				delete it->second;
			}
		}

		for ( std::vector<TexCoords*>::iterator it = _obsoleteTexCoords.begin(); it != _obsoleteTexCoords.end(); it++ )
			if ( texCoords.find(*it) == texCoords.end() )
				delete *it;

		delete _tesselator;
	}

	void AppearanceManager::refresh( void )
	{
		_lastCoords = 0;
		_lastId = "";
	}

	template <typename AppType>
	AppType AppearanceManager::getAppearance( const std::string& nodeid, ForSide side /*= FS_ANY*/ ) const
	{
        std::map< std::string, std::vector< Appearance* > >::const_iterator map_iterator = _appearancesMap.find( nodeid );
		if ( map_iterator == _appearancesMap.end() ) return 0;

		std::vector< Appearance* >::const_iterator vector_iterator = ( map_iterator->second ).begin();
        for( ; vector_iterator != ( map_iterator->second ).end(); ++vector_iterator ) {
			if ( AppType appType = dynamic_cast< AppType >( *vector_iterator ) ) {
				if ( side == FS_ANY || 
					( side == FS_FRONT && appType->getIsFront() ) ||
					( side == FS_BACK && !appType->getIsFront() ) )
				{
					return appType;
				}
			}
		}

		return 0;
	}

	void AppearanceManager::addAppearance( Appearance* app ) 
	{ 
		if ( app ) _appearances.push_back( app ); 
	}

	void AppearanceManager::assignNode( const std::string& nodeid )
	{ 
        _lastId = nodeid;

        if ( !getAppearance< Appearance * >( nodeid ) )
            _appearancesMap[ nodeid ] = std::vector< Appearance* >(0);

		Appearance* currentAppearance = _appearances[ _appearances.size() - 1 ];
		ForSide side = currentAppearance->getIsFront() ? FS_FRONT : FS_BACK;
		if ( dynamic_cast< Texture* >( currentAppearance ) && !getAppearance< Texture* >( nodeid, side ) ||
			 dynamic_cast< Material* >( currentAppearance ) && !getAppearance< Material* >( nodeid, side ) )
		{
            _appearancesMap[ nodeid ].push_back( currentAppearance );
			if ( _lastCoords ) { assignTexCoords( _lastCoords ); _lastId = ""; }
		}
	}

	bool AppearanceManager::assignTexCoords( TexCoords* tex ) 
	{ 
		_lastCoords = tex;
        if ( _lastId == "" )
		{
            _obsoleteTexCoords.push_back( tex );   
            return false;
        }
		_texCoordsMap[ _lastId ] = tex; 
		_lastCoords = 0;
		_lastId = "";
		return true;
	}

    void AppearanceManager::finish(void)
    {
        std::set<TexCoords*> useLessTexCoords;

		for ( std::map<std::string, TexCoords*>::iterator it = _texCoordsMap.begin(); it != _texCoordsMap.end(); ++it )
		{
			if ( it->second && useLessTexCoords.find( it->second ) == useLessTexCoords.end() )
			{
				useLessTexCoords.insert( it->second );
				delete it->second;
			}
		}

        for ( std::vector<TexCoords*>::iterator it = _obsoleteTexCoords.begin(); it != _obsoleteTexCoords.end(); it++ )
            if ( useLessTexCoords.find( *it ) == useLessTexCoords.end() )
                delete *it;

		_appearancesMap.clear();
        _texCoordsMap.clear();
        _obsoleteTexCoords.clear();
    }
		
	///////////////////////////////////////////////////////////////////////////////
	// Polygon handling

	Polygon::~Polygon( void ) 
	{ 
		delete _exteriorRing;
		std::vector< LinearRing* >::const_iterator it = _interiorRings.begin();
		for ( ; it != _interiorRings.end(); ++it ) delete *it;
	}

	TVec3d Polygon::computeNormal( void ) 
	{
		if ( !_exteriorRing ) return TVec3d();

		TVec3d normal = _exteriorRing->computeNormal();

		return _negNormal ? -normal : normal;
	}

	void Polygon::tesselate( AppearanceManager &appearanceManager, const TVec3d& normal )
	{
		_indices.clear();

		if ( !_exteriorRing || _exteriorRing->size() < 3 )
		{ 
			mergeRings( appearanceManager );
			return;
		}

		TexCoords texCoords;
		bool t = appearanceManager.getTexCoords( _exteriorRing->getId(), texCoords );
		_exteriorRing->finish( t ? &texCoords : &_texCoords );
		if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );

		for ( unsigned int i = 0; i < _interiorRings.size(); i++ ) {
			TexCoords texCoords;
			bool t = appearanceManager.getTexCoords( _interiorRings[i]->getId(), texCoords );
			_interiorRings[i]->finish( t ? &texCoords : &_texCoords );
			if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );
		}

		// Compute the total number of vertices
		unsigned int vsize = _exteriorRing->size();
		for ( unsigned int i = 0; i < _interiorRings.size(); i++ )
			vsize += _interiorRings[i]->size();

		Tesselator* tess = appearanceManager.getTesselator();
		tess->init( vsize, normal );

		tess->addContour( _exteriorRing->getVertices(), texCoords );

		for ( unsigned int i = 0; i < _interiorRings.size(); i++ )
			tess->addContour( _interiorRings[i]->getVertices(), texCoords ); 

		tess->compute();
		_vertices.reserve( tess->getVertices().size() );
		std::copy( tess->getVertices().begin(), tess->getVertices().end(), std::back_inserter( _vertices ) );

		unsigned int indicesSize = tess->getIndices().size();
		if ( indicesSize > 0 ) 
		{
			_indices.resize( indicesSize );
			memcpy( &_indices[0], &tess->getIndices()[0], indicesSize * sizeof(unsigned int) );
		}
		clearRings();
	}

	void Polygon::mergeRings( AppearanceManager &appearanceManager )
	{
        _vertices.reserve( _vertices.size() + _exteriorRing->size() );
        TexCoords texCoords;
        bool t = appearanceManager.getTexCoords( _exteriorRing->getId(), texCoords );
        _exteriorRing->finish( t ? &texCoords : &_texCoords );
        if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );

        std::copy( _exteriorRing->getVertices().begin(), _exteriorRing->getVertices().end(), std::back_inserter( _vertices ) );

		for ( unsigned int i = 0; i < _interiorRings.size(); i++ )
		{
			TexCoords texCoords;
			bool t = appearanceManager.getTexCoords( _interiorRings[i]->getId(), texCoords );
			_interiorRings[i]->finish( t ? &texCoords : &_texCoords );
			if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );

			_vertices.reserve( _vertices.size() + _interiorRings[i]->size() );

			std::copy( _interiorRings[i]->getVertices().begin(), _interiorRings[i]->getVertices().end(), std::back_inserter( _vertices ) );
		}
		clearRings();
		_indices.clear();

		if ( _vertices.size() < 3 ) return;

		// Create triangles' indices
		int indicesSize = 3 * ( _vertices.size() - 2 );
		if ( indicesSize < 3 ) return;
		_indices.resize( indicesSize );
		for ( int i = 0, p = 0; p < indicesSize - 2; i++, p += 3 )
			for ( unsigned int j = 0; j < 3; j++ )
				_indices[ p + j ] = i + j;
	}

	void Polygon::clearRings( void )
	{
		delete _exteriorRing;
		_exteriorRing = 0;
		for ( unsigned int i = 0; i < _interiorRings.size(); i++ ) delete _interiorRings[i]; 
		_interiorRings.clear();
	}

	// Merge polygon p into the current polygon
	bool Polygon::merge( Polygon* p )
	{
		if ( !p ) return false;

		if ( p->getAppearance() != getAppearance() ) return false;

		if ( p->getVertices().size() == 0 ) return true;

		// merge vertices
		unsigned int oldVSize = _vertices.size();
		unsigned int pVSize = p->_vertices.size();
		_vertices.resize( oldVSize + pVSize );
		for ( unsigned int i = 0; i < pVSize; i++ )
			_vertices[ oldVSize + i ] = p->_vertices[i];
		p->_vertices.clear();
		
		// merge indices
		{
			unsigned int oldSize = _indices.size();
			unsigned int pSize = p->_indices.size();
			_indices.resize( oldSize + pSize );
			for ( unsigned int i = 0; i < pSize; i++ )
				_indices[ oldSize + i ] = oldVSize + p->_indices[i];
			p->_indices.clear();
		}

		// merge normals
		{
			unsigned int oldSize = _normals.size();
			unsigned int pSize = p->_normals.size();
			_normals.resize( oldSize + pSize );
			for ( unsigned int i = 0; i < pSize; i++ )
				_normals[ oldSize + i ] = p->_normals[i];
			p->_normals.clear();
		}

		// merge texcoords
		{
			unsigned int oldSize = min( _texCoords.size(), oldVSize );
			unsigned int pSize = min( p->_texCoords.size(), pVSize );
			_texCoords.resize( oldSize + pSize );
			for ( unsigned int i = 0; i < pSize; i++ )
				_texCoords[ oldSize + i ] = p->_texCoords[i];
			p->_texCoords.clear();
		}

		// merge ids
		_id += "+" + p->_id;

		return true;
	}

	void Polygon::finish( AppearanceManager& appearanceManager, bool doTesselate ) 
	{
		TVec3d normal = computeNormal();
		if ( doTesselate ) tesselate( appearanceManager, normal );	else mergeRings( appearanceManager );

		// Create the normal per point field
		_normals.resize( _vertices.size() );
		for ( unsigned int i = 0; i < _vertices.size(); i++ )
			_normals[i] = TVec3f( (float)normal.x, (float)normal.y, (float)normal.z );
	}

	void Polygon::finish( AppearanceManager& appearanceManager, Appearance* defAppearance, bool doTesselate )
	{	
		if ( !appearanceManager.getTexCoords( getId(), _texCoords ) ) 
			appearanceManager.getTexCoords( _geometry->getId(), _texCoords );
				
		finish( appearanceManager, doTesselate );
		
		_texCoords.resize( _vertices.size() );
		
        const std::string id = getId();
        _appearance = appearanceManager.getAppearance( id );
		if ( !_appearance ) _appearance = defAppearance;

        _materials[ FRONT ] = appearanceManager.getMaterialFront( id );
        _materials[ BACK ] = appearanceManager.getMaterialBack( id );
 		if ( !_materials[ FRONT ]  && !_materials[ BACK ])
 			_materials[ FRONT ] = dynamic_cast< Material * >( defAppearance );

        _texture = appearanceManager.getTexture( id );
		if ( !_texture ) _texture = dynamic_cast< Texture * >( defAppearance );
	}

	void Polygon::addRing( LinearRing* ring ) 
	{
		if ( ring->isExterior() ) _exteriorRing = ring;
		else _interiorRings.push_back( ring );
	}

	///////////////////////////////////////////////////////////////////////////////
	// Geometry handling

	Geometry::~Geometry() 
	{ 
		std::vector< Polygon* >::const_iterator it = _polygons.begin();
		for ( ; it != _polygons.end(); ++it ) delete *it;
	}

	void Geometry::addPolygon( Polygon* p ) 
	{ 
		p->_geometry = this;
		_polygons.push_back( p ); 
	}

	void Geometry::finish( AppearanceManager& appearanceManager, Appearance* defAppearance,  const ParserParams& params )
	{
        // only need to finish geometry once
        if (_finished)
            return;

        Appearance* myappearance = appearanceManager.getAppearance( getId() );
		std::vector< Polygon* >::const_iterator it = _polygons.begin();
		for ( ; it != _polygons.end(); ++it ) (*it)->finish( appearanceManager, myappearance ? myappearance : defAppearance, params.tesselate );

		bool finish = false;
		while ( !finish && params.optimize ) 
		{			
			finish = true;
			int len = (int)_polygons.size();			
			for ( int i = 0; finish && i < len - 1; i++ ) 
			{
				for ( int j = i+1; finish && j < len - 1; j++ ) 
				{
					if ( !_polygons[i]->merge( _polygons[j] ) ) continue;
					delete _polygons[j];
					_polygons.erase( _polygons.begin() + j );
					finish = false;		
				}
			}
		}
        _finished = true;
	}

	bool Geometry::merge( Geometry* g ) 
	{
		if ( !g || g->_lod != _lod || g->_type != _type ) return false;

		unsigned int pSize = g->_polygons.size();
		for ( unsigned int i = 0; i < pSize; i++ )
			_polygons.push_back( g->_polygons[i] );

		g->_polygons.clear();

		_id += "+" + g->_id;

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	std::string getCityObjectsClassName( CityObjectsTypeMask mask )
	{
#define GETCITYNAME( _t_ ) if ( mask & COT_ ## _t_ ) ss << # _t_ << "|";
		std::stringstream ss;
		GETCITYNAME( GenericCityObject );
        GETCITYNAME( Building );
		GETCITYNAME( BuildingPart );
		GETCITYNAME( Room );
		GETCITYNAME( BuildingInstallation );
		GETCITYNAME( BuildingFurniture );
		GETCITYNAME( Door );
		GETCITYNAME( Window );
		GETCITYNAME( CityFurniture );
		GETCITYNAME( Track );
		GETCITYNAME( Road );
		GETCITYNAME( Railway );
		GETCITYNAME( Square );
		GETCITYNAME( PlantCover );
		GETCITYNAME( SolitaryVegetationObject );
		GETCITYNAME( WaterBody );
		GETCITYNAME( TINRelief );
		GETCITYNAME( LandUse );
		GETCITYNAME( Tunnel );
		GETCITYNAME( Bridge );
		GETCITYNAME( BridgeConstructionElement );
		GETCITYNAME( BridgeInstallation );
		GETCITYNAME( BridgePart );
		GETCITYNAME( RoofSurface );
		GETCITYNAME( WallSurface );
		GETCITYNAME( GroundSurface );
		GETCITYNAME( ClosureSurface );      
		GETCITYNAME( FloorSurface );        
		GETCITYNAME( InteriorWallSurface );
		GETCITYNAME( CeilingSurface );
#undef GETCITYNAME
		std::string s = ss.str();
		if ( s != "" ) s.erase( s.length() - 1, 1 ); // remove the last | char
		return s;
	};

	CityObjectsTypeMask getCityObjectsTypeMaskFromString( const std::string& stringMask ) 
	{
		CityObjectsTypeMask mask = 0;

		std::vector<std::string> tokens = tokenize( stringMask );

#define COMPARECITYNAMEMASK( _t_ ) {\
	bool neg = ( tokens[i][0] == '~' || tokens[i][0] == '!' );\
	if ( ci_string_compare( #_t_, neg ? tokens[i].substr(1) : tokens[i] ) ) { mask = neg ? ( mask & (~ COT_ ## _t_ )) : ( mask | COT_ ## _t_ );}\
	}

		for ( unsigned int i = 0; i < tokens.size(); i++ ) 
		{
			if ( tokens[i].length() == 0 ) continue;

			COMPARECITYNAMEMASK( GenericCityObject );
			COMPARECITYNAMEMASK( Building );
			COMPARECITYNAMEMASK( BuildingPart );
			COMPARECITYNAMEMASK( Room );
			COMPARECITYNAMEMASK( BuildingInstallation );
			COMPARECITYNAMEMASK( BuildingFurniture );
			COMPARECITYNAMEMASK( Door );
			COMPARECITYNAMEMASK( Window );
			COMPARECITYNAMEMASK( CityFurniture );
			COMPARECITYNAMEMASK( Track );				 
			COMPARECITYNAMEMASK( Road );				 
			COMPARECITYNAMEMASK( Railway );
			COMPARECITYNAMEMASK( Square	);				 
			COMPARECITYNAMEMASK( PlantCover	);
			COMPARECITYNAMEMASK( SolitaryVegetationObject );
			COMPARECITYNAMEMASK( WaterBody );
			COMPARECITYNAMEMASK( TINRelief );					 
			COMPARECITYNAMEMASK( LandUse );
			COMPARECITYNAMEMASK( Tunnel );
			COMPARECITYNAMEMASK( Bridge );
			COMPARECITYNAMEMASK( BridgeConstructionElement );
			COMPARECITYNAMEMASK( BridgeInstallation );
			COMPARECITYNAMEMASK( BridgePart );
			COMPARECITYNAMEMASK( GenericCityObject );

			COMPARECITYNAMEMASK( WallSurface );
			COMPARECITYNAMEMASK( RoofSurface );         
			COMPARECITYNAMEMASK( GroundSurface );        
			COMPARECITYNAMEMASK( ClosureSurface );      
			COMPARECITYNAMEMASK( FloorSurface );        
			COMPARECITYNAMEMASK( InteriorWallSurface );
			COMPARECITYNAMEMASK( CeilingSurface );
			COMPARECITYNAMEMASK( All );					
		}
#undef COMPARECITYNAMEMASK
		return mask;
	}

	void CityObject::finish( AppearanceManager& appearanceManager, const ParserParams& params ) 
	{
		Appearance* myappearance = appearanceManager.getAppearance( getId() );

        std::vector< Geometry* >::const_iterator itGeom = _geometries.begin();
        for ( ; itGeom != _geometries.end(); ++itGeom ) {
            if ( !( (*itGeom)->getComposite() ) ) {
                (*itGeom)->finish( appearanceManager, myappearance ? myappearance : 0, params );
            } else {
                Composite* geomComposite = (*itGeom)->getComposite();
                myappearance = appearanceManager.getAppearance( geomComposite->getId() );
                (*itGeom)->finish( appearanceManager, myappearance ? myappearance : 0, params );
            }
        }

        std::vector< ImplicitGeometry* >::const_iterator itImplGeom = _implicitGeometries.begin();
        for( ; itImplGeom != _implicitGeometries.end(); ++itImplGeom) {
            for(std::vector< Geometry* >::const_iterator it = (*itImplGeom)->_geometries.begin(); it != (*itImplGeom)->_geometries.end(); ++it) {
                if ( !( (*it)->getComposite() ) ) {
                    (*it)->finish( appearanceManager, myappearance ? myappearance : 0, params );
                } else {
                    Composite* geomComposite = (*it)->getComposite();
                    myappearance = appearanceManager.getAppearance( geomComposite->getId() );
                    (*it)->finish( appearanceManager, myappearance ? myappearance : 0, params );
                }
            }
        }

        bool finish = false;
        while ( !finish && params.optimize )
        {
            finish = true;
            int len = _geometries.size();
            for ( int i = 0; finish && i < len - 2; i++ )
            {
                for ( int j = i+1; finish && j < len - 1; j++ )
                {
                    if ( !_geometries[i]->merge( _geometries[j] ) ) continue;
                    delete _geometries[j];
                    _geometries.erase( _geometries.begin() + j );
                    finish = false;
                }
            }
        }
	}

	///////////////////////////////////////////////////////////////////////////////

	CityModel::~CityModel( void ) 
	{ 	
		CityObjectsMap::const_iterator it = _cityObjectsMap.begin();
		for ( ; it != _cityObjectsMap.end(); ++it ) 
			for ( unsigned int i = 0; i < it->second.size(); i++ )
				delete it->second[i];
	}

	void CityModel::addCityObject( CityObject* o )
	{
		CityObjectsMap::iterator it = _cityObjectsMap.find( o->getType() );
		if ( it == _cityObjectsMap.end() )
		{
			CityObjects v;
			v.push_back( o );
			_cityObjectsMap[ o->getType() ] = v;
		}
		else
			it->second.push_back( o );
	}

	void CityModel::finish( const ParserParams& params ) 
	{
		// Assign appearances to cityobjects => geometries => polygons
		CityObjectsMap::const_iterator it = _cityObjectsMap.begin();
		for ( ; it != _cityObjectsMap.end(); ++it ) 
			for ( unsigned int i = 0; i < it->second.size(); i++ )
				it->second[i]->finish( _appearanceManager, params );

		_appearanceManager.finish();
	}
}
