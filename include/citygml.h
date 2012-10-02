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

#ifndef __CITYGML_H__
#define __CITYGML_H__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "vecs.h"

#define LIBCITYGML_VERSION_MAJOR 0
#define LIBCITYGML_VERSION_MINOR 1
#define LIBCITYGML_VERSION_REVISION 4

#define LIBCITYGML_VERSIONSTR "0.1.4"

#if defined( _MSC_VER ) && defined( LIBCITYGML_DYNAMIC )
#	ifdef LIBCITYGML_BUILD
#		define LIBCITYGML_EXPORT __declspec( dllexport )
#	else
#		define LIBCITYGML_EXPORT __declspec( dllimport )
#	endif
#else
#	define LIBCITYGML_EXPORT
#endif

class Tesselator;

namespace citygml 
{
	class CityModel;

	enum CityObjectsType {
		COT_GenericCityObject           = 1 << 0,
		COT_Building                    = 1 << 1,
		COT_Room                        = 1 << 2,
		COT_BuildingInstallation        = 1 << 3,
		COT_BuildingFurniture           = 1 << 4,
		COT_Door                        = 1 << 5,
		COT_Window                      = 1 << 6,
		COT_CityFurniture               = 1 << 7,
		COT_Track                       = 1 << 8,
		COT_Road                        = 1 << 9,
		COT_Railway                     = 1 << 10,
		COT_Square                      = 1 << 11,
		COT_PlantCover                  = 1 << 12,
		COT_SolitaryVegetationObject    = 1 << 13,
		COT_WaterBody                   = 1 << 14,
		COT_TINRelief                   = 1 << 15,
		COT_LandUse                     = 1 << 16,
		COT_Tunnel						= 1 << 17,
		COT_Bridge						= 1 << 18,
		COT_BridgeConstructionElement	= 1 << 19,
		COT_BridgeInstallation			= 1 << 20, 
		COT_BridgePart                  = 1 << 21,
		COT_BuildingPart                = 1 << 22,

		COT_WallSurface                 = 1 << 23,
		COT_RoofSurface                 = 1 << 24,
		COT_GroundSurface               = 1 << 25,
		COT_ClosureSurface              = 1 << 26,
		COT_FloorSurface                = 1 << 27,
		COT_InteriorWallSurface         = 1 << 28,
		COT_CeilingSurface              = 1 << 29,

		COT_All                         = 0xFFFFFFFF
	};

	typedef unsigned int CityObjectsTypeMask;

	///////////////////////////////////////////////////////////////////////////////
	// Parsing routines

	// Parameters:
	// objectsMask: a string describing the objects types that must or must not be parsed
	//    examples: "All&~LandUse&~TINRelief" to parse everything but landuses and TIN reliefs
	//              "Road&Railway" to parse only roads & railways
	// minLOD: the minimal LOD that will be parsed
	// maxLOD: the maximal LOD that will be parsed
	// optimize: merge geometries & polygons that share the same appearance in the same object in order to reduce the global hierarchy
	// pruneEmptyObjects: remove the objects which do not contains any geometrical entity
	// tesselate: convert the interior & exteriors polygons to triangles
	// destSRS: the SRS (WKT, EPSG, OGC URN, etc.) where the coordinates must be transformed, default ("") is no transformation

	class ParserParams
	{
	public:
		ParserParams( void ) : objectsMask( "All" ), minLOD( 0 ), maxLOD( 4 ), optimize( false ), pruneEmptyObjects( false ), tesselate( true ), destSRS( "" ) { }

	public:
		std::string objectsMask; 
		unsigned int minLOD; 
		unsigned int maxLOD;
		bool optimize; 
		bool pruneEmptyObjects; 
		bool tesselate;
		std::string destSRS;
	};

	LIBCITYGML_EXPORT CityModel* load( std::istream& stream, const ParserParams& params );

	LIBCITYGML_EXPORT CityModel* load( const std::string& fileName, const ParserParams& params );

	///////////////////////////////////////////////////////////////////////////////

	class Envelope
	{
		friend class CityGMLHandler;
	public:
		Envelope( void ) {}

		Envelope( const TVec3d& lowerBound, const TVec3d& upperBound )
		{ 
			_lowerBound = lowerBound; 
			_upperBound = upperBound; 
		}

		inline const TVec3d& getLowerBound( void ) const { return _lowerBound; }
		inline const TVec3d& getUpperBound( void ) const { return _upperBound; }

	protected:
		TVec3d _lowerBound;
		TVec3d _upperBound;
	};

	typedef std::map< std::string, std::string > AttributesMap;
	///////////////////////////////////////////////////////////////////////////////
	// Base object associated with an unique id and a set of attributes (key-value pairs)
	class Object 
	{
		friend class CityGMLHandler;
		friend std::ostream& operator<<( std::ostream&, const Object & );
	public:
		Object( const std::string& id ) : _id( id ) { if ( _id == "" ) { std::stringstream ss; ss << "PtrId_" << this; _id = ss.str(); } }

		virtual ~Object( void ) {}

		inline const std::string& getId( void ) const { return _id; }

		inline std::string getAttribute( const std::string& name ) const
		{
			std::map< std::string, std::string >::const_iterator elt = _attributes.find( name );
			return elt != _attributes.end() ? elt->second : "";
		}

		inline const AttributesMap& getAttributes() const { return _attributes; }

		inline AttributesMap& getAttributes() { return _attributes; }

	protected:
		inline void setAttribute( const std::string& name, const std::string& value, bool forceOnExist = true )
		{
			if ( !forceOnExist )
			{
				std::map< std::string, std::string >::const_iterator elt = _attributes.find( name );
				if ( elt != _attributes.end() ) return;
			}
			_attributes[ name ] = value;				
		}

	protected:
		std::string _id;

		AttributesMap _attributes;
	};

	///////////////////////////////////////////////////////////////////////////////

	class Appearance : public Object
	{
		friend class CityGMLHandler;
	public:
		Appearance( const std::string& id, const std::string& typeString ) : Object( id ), _typeString( typeString ), _isFront(true) {}

		virtual ~Appearance( void ) {}

		inline std::string getType( void ) const { return _typeString; }
		inline bool getIsFront( void ) const { return _isFront; }

		virtual std::string toString( void ) const { return _typeString + " " + _id; }

	protected:
		std::string _typeString;
		bool _isFront;
	};


	///////////////////////////////////////////////////////////////////////////////

	class Texture : virtual public Appearance
	{
		friend class CityGMLHandler;
		
	public:
		typedef enum WrapMode 
		{
			WM_NONE = 0,	// the resulting color is fully transparent
			WM_WRAP,		// the texture is repeated
			WM_MIRROR,		// the texture is repeated and mirrored
			WM_CLAMP,		// the texture is clamped to its edges
			WM_BORDER		// the resulting color is specified by the borderColor element (RGBA)
		} WrapMode;

		Texture( const std::string& id ) : Appearance( id, "Texture" ), _repeat( false ), _wrapMode( WM_NONE ) {}

		inline std::string getUrl( void ) const { return _url; }

		inline bool getRepeat( void ) const { return _repeat; }

		inline WrapMode getWrapMode( void ) const { return _wrapMode; }

		inline TVec4f getBorderColor( void ) const { return _borderColor; }

		inline std::string toString( void ) const { return Appearance::toString() + " (url: " + _url + ")"; }

	protected:
		std::string _url;
		bool _repeat;
		WrapMode _wrapMode;
		TVec4f _borderColor;
	};

	class GeoreferencedTexture : public Texture
	{
		friend class CityGMLHandler;

	public:
		GeoreferencedTexture( const std::string& id ) : Appearance( id, "GeoreferencedTexture" ), Texture( id ), _preferWorldFile(true) {}

		inline bool getPreferWorldFile( void ) const { return _preferWorldFile; }

		// TODO support referencePoint and orientation

	protected:
		bool _preferWorldFile;
	};

	///////////////////////////////////////////////////////////////////////////////

	class Material : virtual public Appearance
	{
		friend class CityGMLHandler;
	public:
		Material( const std::string& id ) : Appearance( id, "Material" ), _ambientIntensity( 0.f ), _shininess( 0.f ), _transparency( 0.f ) {}

		inline TVec3f getDiffuse( void ) const { return _diffuse; }
		inline TVec3f getEmissive( void ) const { return _emissive; }
		inline TVec3f getSpecular( void ) const { return _specular; }
		inline float getAmbientIntensity( void ) const { return _ambientIntensity; }
		inline float getShininess( void ) const { return _shininess; }
		inline float getTransparency( void ) const { return _transparency; }

	protected:
		TVec3f _diffuse;
		TVec3f _emissive;
		TVec3f _specular;
		float _ambientIntensity;
		float _shininess;
		float _transparency;
	};

	///////////////////////////////////////////////////////////////////////////////

	typedef std::vector<TVec2f> TexCoords;

	class AppearanceManager 
	{
		friend class CityGMLHandler;
		friend class CityModel;
	public:
		AppearanceManager( void );

		~AppearanceManager( void );

		typedef enum ForSide 
		{
			FS_ANY = 0,	// appearance for any side
			FS_FRONT,	// appearance for front side
			FS_BACK		// appearance for back side
		};

		inline Appearance* getAppearance( const std::string& nodeid ) const
		// Deprecated, use getMaterial and getTexture instead.
		{
			return getAppearance< Appearance* >( nodeid );
		}
		inline Material* getMaterial( const std::string& nodeid ) const
		{
			return getAppearance< Material* >( nodeid );
		}

		inline Texture* getTexture( const std::string& nodeid ) const
		{
			return getAppearance< Texture* >( nodeid );
		}

		// Getter for the front&back material if there is any.
		inline Material* getMaterialFront( const std::string& nodeid ) const
		{
			return getAppearance< Material* >( nodeid, FS_FRONT );
		}
		inline Material* getMaterialBack( const std::string& nodeid ) const
		{
			return getAppearance< Material* >( nodeid, FS_BACK );
		}

		inline bool getTexCoords( const std::string& nodeid, TexCoords &texCoords) const
		{
			texCoords.clear();
			std::map<std::string, TexCoords*>::const_iterator it = _texCoordsMap.find( nodeid );
			if ( it == _texCoordsMap.end() || !it->second ) return false;
			texCoords = *it->second;
			return true;
		}

		inline Tesselator* getTesselator( void ) { return _tesselator; }

	protected:
		void refresh( void );

		template < typename AppType > AppType getAppearance( const std::string& nodeid, ForSide side = FS_ANY ) const;
		void addAppearance( Appearance* );
		void assignNode( const std::string& nodeid );
		bool assignTexCoords( TexCoords* );

		void finish( void );

	protected:
		std::string _lastId;
		TexCoords* _lastCoords;

		std::vector< Appearance* > _appearances;

		std::map< std::string, std::vector< Appearance* > > _appearancesMap;

		std::map<std::string, TexCoords*> _texCoordsMap;
        std::vector<TexCoords*> _obsoleteTexCoords;

		Tesselator* _tesselator;
	};

	///////////////////////////////////////////////////////////////////////////////

	class LinearRing : public Object
	{
		friend class CityGMLHandler;
		friend class Polygon;
	public:
		LinearRing( const std::string& id, bool isExterior ) : Object( id ), _exterior( isExterior ) {}

		bool isExterior( void ) const { return _exterior; }

		inline unsigned int size( void ) const { return _vertices.size(); }

		inline const std::vector<TVec3d>& getVertices( void ) const { return _vertices; }

		inline void addVertex( const TVec3d& v ) { _vertices.push_back( v ); }

		LIBCITYGML_EXPORT TVec3d computeNormal( void ) const;

	protected:
		inline std::vector<TVec3d>& getVertices( void ) { return _vertices; }

		void finish( TexCoords* );

	protected:		
		bool _exterior;

		std::vector<TVec3d> _vertices;
	};

	///////////////////////////////////////////////////////////////////////////////
	// Polygon Class

	class Geometry;

	class Polygon : public Object
	{
		friend class CityGMLHandler;
		friend class Geometry;
		friend class Tesseletor;
		friend class CityModel;
	public:
		enum AppearanceSide {
			FRONT = 0,
			BACK,
			_NUMBER_OF_SIDES
		};

		Polygon( const std::string& id ) : 
		  Object( id ), _appearance( 0 ), _texture( 0 ), _exteriorRing( 0 ), _negNormal( false ), _geometry( 0 ) 
		  {
			  _materials[ FRONT ] = 0;
			  _materials[ BACK ] = 0;
		  }

		LIBCITYGML_EXPORT ~Polygon( void );

		// Get the vertices
		inline const std::vector<TVec3d>& getVertices( void ) const { return _vertices; }

		// Get the indices
		inline const std::vector<unsigned int>& getIndices( void ) const { return _indices; }

		// Get the normals
		inline const std::vector<TVec3f>& getNormals( void ) const { return _normals; }

		// Get texture coordinates
		inline const TexCoords& getTexCoords( void ) const { return _texCoords; }

		// Get the appearance
		inline const Appearance* getAppearance( void ) const { return _appearance; } // Deprecated! Use getMaterial and getTexture instead
		inline const Material* getMaterial( void ) const 
		{ 
			if ( _materials[ FRONT ] ) { return _materials[ FRONT ]; }
			else { return _materials[ BACK ]; }
		}
		inline const Texture* getTexture( void ) const { return _texture; }
		inline const Material* getMaterialFront( void ) const { return _materials[ FRONT ]; }
		inline const Material* getMaterialBack( void ) const { return _materials[ BACK ]; }

	protected:
		void finish( AppearanceManager&, bool doTesselate );
		void finish( AppearanceManager&, Appearance*,  bool doTesselate );

		void addRing( LinearRing* );

		void tesselate( AppearanceManager &, const TVec3d& );
		void mergeRings( AppearanceManager & );
		void clearRings( void );

		TVec3d computeNormal( void );

		bool merge( Polygon* );

	protected:
		std::vector<TVec3d> _vertices;
		std::vector<TVec3f> _normals;
		std::vector<unsigned int> _indices;

		Appearance* _appearance;
		Material* _materials[ _NUMBER_OF_SIDES ];
		Texture* _texture;

		TexCoords _texCoords; 

		LinearRing* _exteriorRing;
		std::vector<LinearRing*> _interiorRings;

		bool _negNormal;

		Geometry *_geometry;
	};

	///////////////////////////////////////////////////////////////////////////////
	// Geometry Class

	enum GeometryType 
	{
		GT_Unknown = 0,
		GT_Roof,
		GT_Wall,
		GT_Ground,
		GT_Closure,
		GT_Floor,
		GT_InteriorWall,
		GT_Ceiling,
	};

	class Geometry : public Object
	{
		friend class CityGMLHandler;
		friend class CityObject;
		friend class Composite;
		friend std::ostream& operator<<( std::ostream&, const citygml::Geometry& );
	public:
		Geometry( const std::string& id, GeometryType type = GT_Unknown, unsigned int lod = 0 ) : Object( id ), _type( type ), _lod( lod ), _composite( 0 ) {}

		LIBCITYGML_EXPORT ~Geometry();

		// Get the geometry LOD
		inline unsigned int getLOD( void ) const { return _lod; }

		// Get the polygons
		inline unsigned int size( void ) const { return _polygons.size(); }
		inline Polygon* operator[]( unsigned int i ) { return _polygons[i]; }
		inline const Polygon* operator[]( unsigned int i ) const { return _polygons[i]; }

		inline GeometryType getType( void ) const { return _type; }

	protected:
		void addPolygon( Polygon* );

		void finish( AppearanceManager&, Appearance*, const ParserParams& );

		bool merge( Geometry* );

	protected:
		GeometryType _type;

		unsigned int _lod;

		std::vector< Polygon* > _polygons;

		Composite *_composite;
	};

	///////////////////////////////////////////////////////////////////////////////
	// Composite Class

	class Composite : public Object
	{
		friend class CityGMLHandler;
		friend class CityObject;
		friend std::ostream& operator<<( std::ostream&, const citygml::Composite& );
	public:
		Composite( const std::string& id, unsigned int lod = 0 ) : Object( id ), _lod( lod) {}

		LIBCITYGML_EXPORT ~Composite();

		// Get the composite lod
		inline unsigned int getLOD( void ) const { return _lod; }

		// Get the geometries
		inline unsigned int size( void ) const { return _geometries.size(); }
		inline Geometry* operator[]( unsigned int i ) { return _geometries[i]; }
		inline const Geometry* operator[]( unsigned int i ) const { return _geometries[i]; }

	protected:
		void addGeometry( Geometry* );

		void finish( AppearanceManager&, Appearance*, const ParserParams& );

		bool merge( Composite* );

		unsigned int _lod;
		std::vector< Geometry* > _geometries;
	};

	///////////////////////////////////////////////////////////////////////////////

	LIBCITYGML_EXPORT std::string getCityObjectsClassName( CityObjectsTypeMask mask );

	LIBCITYGML_EXPORT CityObjectsTypeMask getCityObjectsTypeMaskFromString( const std::string& stringMask );

	class CityObject : public Object 
	{
		friend class CityGMLHandler;
		friend class CityModel;
		friend std::ostream& operator<<( std::ostream&, const CityObject & );
	public:
		CityObject( const std::string& id, CityObjectsType type ) : Object( id ), _type( type ) {}

		virtual ~CityObject()
		{ 
			std::vector< Geometry* >::const_iterator itGeom = _geometries.begin();
			for ( ; itGeom != _geometries.end(); ++itGeom ) delete *itGeom;
			std::vector< Composite* >::const_iterator itComp = _composites.begin();
			for ( ; itComp != _composites.end(); ++itComp ) delete *itComp;
		}

		// Get the object type
		inline CityObjectsType getType( void ) const { return _type; }

		inline std::string getTypeAsString( void ) const { return getCityObjectsClassName( _type ); }

		// Return the envelope (ie. the bounding box) of the object
		inline const Envelope& getEnvelope( void ) const { return _envelope; }

		// Get the default diffuse color of this object class
		virtual TVec4f getDefaultColor( void ) const = 0;

		// Get the number of geometries contains in the object
		inline unsigned int size( void ) const { return _geometries.size(); }

		// Access the geometries
		inline const Geometry* getGeometry( unsigned int i ) const { return _geometries[i]; }

		// Access the composites
		inline const Composite* getComposite( unsigned int i ) const { return _composites[i]; }

		// Access the children
		inline unsigned int getChildCount( void ) const { return _children.size(); }

		inline const CityObject* getChild( unsigned int i ) const { return ( i < getChildCount() ) ? _children[i] : 0; }

		inline CityObject* getChild( unsigned int i ) { return ( i < getChildCount() ) ? _children[i] : 0; }

		inline const std::vector< CityObject* >& getChildren( void ) const { return _children; }

		inline std::vector< CityObject* >& getChildren( void ) { return _children; }

	protected:
		void finish( AppearanceManager&, const ParserParams& );

	protected:
		CityObjectsType _type;

		Envelope _envelope;

		std::vector< Geometry* > _geometries;
		std::vector< Composite* > _composites;		
		std::vector< CityObject* > _children;
	};

#define MAKE_RGBA( _r_, _g_, _b_, _a_ ) TVec4f( _r_/255.f, _g_/255.f, _b_/255.f, _a_/255.f )
#define MAKE_RGB( _r_, _g_, _b_ ) MAKE_RGBA( _r_, _g_, _b_, 255 )

	// Helper macro to declare a new CityObject type from its name & default color
#define DECLARE_SIMPLE_OBJECT_CLASS( _name_, _defcolor_ ) \
	class _name_ : public CityObject \
	{\
	public:\
	_name_( const std::string& id ) : CityObject( id, COT_ ## _name_ ) {}\
	inline TVec4f getDefaultColor( void ) const { return _defcolor_; }\
	};

	///////////////////////////////////////////////////////////////////////////////

	DECLARE_SIMPLE_OBJECT_CLASS( Building, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( BuildingPart, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Room, MAKE_RGB( 181, 180, 163 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Door, MAKE_RGB( 145, 53, 13 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Window, MAKE_RGBA( 147, 170, 209, 60 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( BuildingInstallation, MAKE_RGB( 186, 186, 177 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( BuildingFurniture, MAKE_RGB( 227, 225, 157 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( CityFurniture, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( WaterBody, MAKE_RGB( 48, 133, 187 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( PlantCover, MAKE_RGB( 0, 184, 0 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( SolitaryVegetationObject, MAKE_RGB( 10, 184, 10 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Track, MAKE_RGB( 171, 131, 46 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Road, MAKE_RGB( 159, 159, 159 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Railway, MAKE_RGB( 180, 180, 180 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Square, MAKE_RGB( 159, 159, 159 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( TINRelief, MAKE_RGB( 100, 230, 10 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Tunnel, MAKE_RGB( 180, 180, 150 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( Bridge, MAKE_RGB( 245, 30, 30 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( BridgeConstructionElement, MAKE_RGB( 245, 20, 20 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( BridgeInstallation, MAKE_RGB( 245, 80, 80 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( BridgePart, MAKE_RGB( 245, 50, 50 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( GenericCityObject, MAKE_RGB( 100, 130, 0 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( WallSurface, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( RoofSurface, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( GroundSurface, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( ClosureSurface, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( FloorSurface, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( InteriorWallSurface, MAKE_RGB( 186, 184, 135 ) );

	DECLARE_SIMPLE_OBJECT_CLASS( CeilingSurface, MAKE_RGB( 186, 184, 135 ) );


	class LandUse : public CityObject 
	{
	public:
		LandUse( const std::string& id ) : CityObject( id, COT_LandUse ) {}

		inline TVec4f getDefaultColor( void ) const
		{ 
			std::string c = getAttribute( "class" );
			if ( c != "" )
			{
				int cl = atoi( c.c_str() );
				switch ( cl ) 
				{
				case 1000: return MAKE_RGB( 150, 143, 134 );	// Settlement Area
				case 1100: return MAKE_RGB( 133, 83, 101 );		// Undeveloped Area
				case 2000: return MAKE_RGB( 159, 159, 159 );	// Traffic
				case 3000: return MAKE_RGB( 79, 212, 53 );		// Vegetation
				case 4000: return MAKE_RGB( 67, 109, 247 );		// Water
				}
			}
			return MAKE_RGB( 10, 230, 1 ); 
		}
	};

	///////////////////////////////////////////////////////////////////////////////

	typedef std::vector< CityObject* > CityObjects;
	typedef std::map< CityObjectsType, CityObjects > CityObjectsMap;

	class CityModel : public Object
	{
		friend class CityGMLHandler;
	public:
		CityModel( const std::string& id = "CityModel" ) : Object( id ) {} 

		LIBCITYGML_EXPORT ~CityModel( void );

		// Return the envelope (ie. the bounding box) of the model
		inline const Envelope& getEnvelope( void ) const { return _envelope; }

		// Return the translation parameters of the model
		inline const TVec3d& getTranslationParameters( void ) const { return _translation; }

		// Get the number of city objects 
		inline unsigned int size( void ) const
		{ 
			unsigned int count = 0;
			CityObjectsMap::const_iterator it = _cityObjectsMap.begin();
			for ( ; it != _cityObjectsMap.end(); ++it ) count += it->second.size();
			return count; 
		}

		inline const CityObjectsMap& getCityObjectsMap( void ) const { return _cityObjectsMap; }

		inline const CityObjects* getCityObjectsByType( CityObjectsType type ) const
		{ 
			CityObjectsMap::const_iterator it = _cityObjectsMap.find( type );
			return ( it != _cityObjectsMap.end() ) ? &it->second : 0;
		}

		// Return the roots elements of the model. You can then navigate the hierarchy using object->getChildren().
		inline const CityObjects& getCityObjectsRoots( void ) const { return _roots; }

		inline const std::string& getSRSName( void ) const { return _srsName; }

	protected:
		void addCityObject( CityObject* o );

		inline void addCityObjectAsRoot( CityObject* o ) { if ( o ) _roots.push_back( o ); }

		void finish( const ParserParams& );

	protected:
		Envelope _envelope;

		CityObjects _roots;

		CityObjectsMap _cityObjectsMap;

		AppearanceManager _appearanceManager;
		
		std::string _srsName;
		
		TVec3d _translation;
	};

	///////////////////////////////////////////////////////////////////////////////

	std::ostream& operator<<( std::ostream&, const citygml::Envelope& );
	std::ostream& operator<<( std::ostream&, const citygml::Object& );
	std::ostream& operator<<( std::ostream&, const citygml::Geometry& );
	std::ostream& operator<<( std::ostream&, const citygml::Composite& );
	std::ostream& operator<<( std::ostream&, const citygml::CityObject& );
	std::ostream& operator<<( std::ostream&, const citygml::CityModel & );
}

#endif // __CITYGML_H__
