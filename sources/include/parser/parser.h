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
#pragma once

#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <fstream>
#include <set>
#include <memory>

#include <citygml/citygml.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>


class GeoTransform;

namespace citygml
{

    class AppearanceManager;
    class CityGMLLogger;
    class CityGMLFactory;

    class AppearanceTarget;
    class ImplictGeometry;
    class Polygon;
    class LinearRing;

    class Texture;
    class GeoreferencedTexture;
    class Material;

    class MaterialTargetDefinition;
    class TextureTargetDefinition;



    #define NODETYPE(_t_) CG_ ## _t_

    // CityGML node types
    enum CityGMLNodeType
    {
        NODETYPE( Unknown ) = 0,

        // core
        NODETYPE( CityModel ),
        NODETYPE( cityObjectMember ),
        NODETYPE( creationDate ),
        NODETYPE( terminationDate ),
        NODETYPE( ImplicitGeometry ),
        NODETYPE( relativeGMLGeometry ),
        NODETYPE( transformationMatrix ),

        // grp
        NODETYPE( CityObjectGroup ),
        NODETYPE( groupMember ),

        // gen
        NODETYPE( GenericCityObject ),
        NODETYPE( stringAttribute ),
        NODETYPE( doubleAttribute ),
        NODETYPE( intAttribute ),
        NODETYPE( dateAttribute ),
        NODETYPE( uriAttribute ),
        NODETYPE( value ),

        // gml
        NODETYPE( description ),
        NODETYPE( name ),
        NODETYPE( coordinates ),
        NODETYPE( pos ),
        NODETYPE( boundedBy ),
        NODETYPE( Envelope ),
        NODETYPE( lowerCorner ),
        NODETYPE( upperCorner ),
        NODETYPE( Solid ),
        NODETYPE( surfaceMember ),
        NODETYPE( TriangulatedSurface ),
        NODETYPE( TexturedSurface ),
        NODETYPE( Triangle ),
        NODETYPE( Polygon ),
        NODETYPE( posList ),
        NODETYPE( OrientableSurface ),
        NODETYPE( LinearRing ),

        NODETYPE( lod1Solid ),
        NODETYPE( lod2Solid ),
        NODETYPE( lod3Solid ),
        NODETYPE( lod4Solid ),
        NODETYPE( lod1Geometry ),
        NODETYPE( lod2Geometry ),
        NODETYPE( lod3Geometry ),
        NODETYPE( lod4Geometry ),
        NODETYPE( lod1MultiCurve ),
        NODETYPE( lod2MultiCurve ),
        NODETYPE( lod3MultiCurve ),
        NODETYPE( lod4MultiCurve ),
        NODETYPE( lod1MultiSurface ),
        NODETYPE( lod2MultiSurface ),
        NODETYPE( lod3MultiSurface ),
        NODETYPE( lod4MultiSurface ),
        NODETYPE( lod1TerrainIntersection ),
        NODETYPE( lod2TerrainIntersection ),
        NODETYPE( lod3TerrainIntersection ),
        NODETYPE( lod4TerrainIntersection ),

        NODETYPE( MultiPoint ),
        NODETYPE( MultiCurve ),
        NODETYPE( MultiSurface ),
        NODETYPE( MultiSolid ),

        NODETYPE( CompositeCurve ),
        NODETYPE( CompositeSurface ),
        NODETYPE( CompositeSolid ),

        NODETYPE( referencePoint ),
        NODETYPE( Point ),

        // bldg
        NODETYPE( Building ),
        NODETYPE( BuildingPart ),
        NODETYPE( Room ),
        NODETYPE( Door ),
        NODETYPE( Window ),
        NODETYPE( BuildingInstallation ),
        NODETYPE( address ),
        NODETYPE( measuredHeight ),
        NODETYPE( class ),
        NODETYPE( type ),
        NODETYPE( function ),
        NODETYPE( usage ),
        NODETYPE( yearOfConstruction ),
        NODETYPE( yearOfDemolition ),
        NODETYPE( storeysAboveGround ),
        NODETYPE( storeysBelowGround ),
        NODETYPE( storeyHeightsAboveGround ),
        NODETYPE( storeyHeightsBelowGround ),

        // address
        NODETYPE( administrativearea ),
        NODETYPE( country ),
        NODETYPE( code ),
        NODETYPE( street ),
        NODETYPE( postalCode ),
        NODETYPE( city ),

        // BoundarySurfaceType
        NODETYPE( WallSurface ),
        NODETYPE( RoofSurface ),
        NODETYPE( GroundSurface ),
        NODETYPE( ClosureSurface ),
        NODETYPE( FloorSurface ),
        NODETYPE( InteriorWallSurface ),
        NODETYPE( CeilingSurface ),
        NODETYPE( BuildingFurniture ),

        NODETYPE( CityFurniture ),

        NODETYPE( interior ),
        NODETYPE( exterior ),

        // wtr
        NODETYPE( WaterBody ),

        // veg
        NODETYPE( PlantCover ),
        NODETYPE( SolitaryVegetationObject ),
        NODETYPE( species ),
        NODETYPE( lod1ImplicitRepresentation ),
        NODETYPE( lod2ImplicitRepresentation ),
        NODETYPE( lod3ImplicitRepresentation ),
        NODETYPE( lod4ImplicitRepresentation ),

        // trans
        NODETYPE( TrafficArea ),
        NODETYPE( AuxiliaryTrafficArea ),
        NODETYPE( Track ),
        NODETYPE( Road ),
        NODETYPE( Railway ),
        NODETYPE( Square ),

        // luse
        NODETYPE( LandUse ),

        // dem
        NODETYPE( lod ),
        NODETYPE( TINRelief ),

        // sub
        NODETYPE( Tunnel ),
        NODETYPE( relativeToTerrain ),

        // brid
        NODETYPE( Bridge ),
        NODETYPE( BridgeConstructionElement ),
        NODETYPE( BridgeInstallation ),
        NODETYPE( BridgePart ),

        // app
        NODETYPE( SimpleTexture ),
        NODETYPE( ParameterizedTexture ),
        NODETYPE( GeoreferencedTexture ),
        NODETYPE( imageURI ),
        NODETYPE( textureMap ),
        NODETYPE( target ),
        NODETYPE( textureCoordinates ),
        NODETYPE( textureType ),
        NODETYPE( repeat ),
        NODETYPE( wrapMode ),
        NODETYPE( borderColor ),
        NODETYPE( preferWorldFile ),

        NODETYPE( X3DMaterial ),
        NODETYPE( Material ),
        NODETYPE( appearanceMember ),
        NODETYPE( surfaceDataMember ),
        NODETYPE( shininess ),
        NODETYPE( transparency ),
        NODETYPE( specularColor ),
        NODETYPE( diffuseColor ),
        NODETYPE( emissiveColor ),
        NODETYPE( ambientIntensity ),
        NODETYPE( isFront ),
        NODETYPE( theme )
    };

    // CityGML SAX parsing handler
    class CityGMLHandler
    {
    public:

        CityGMLHandler( const ParserParams& params, std::shared_ptr<CityGMLLogger> logger );

        virtual void startDocument() {}

        virtual void endDocument() {}

        virtual void startElement( const std::string&, void* );

        virtual void endElement( const std::string& );

        inline CityModel* getModel() { return _model; }

    protected:

        inline int searchInNodePath( const std::string& name ) const
        {
            for ( int i = _nodePath.size() - 1; i >= 0; i-- )
                if ( _nodePath[i] == name ) return i;
            return -1;
        }

        inline std::string getFullPath() const
        {
            std::stringstream ss;
            for ( unsigned int i = 0; i < _nodePath.size(); i++ )
                ss << _nodePath[i] << "/";
            return ss.str();
        }

        std::string getPrevNode() const { return _nodePath.size() > 2 ? _nodePath[ _nodePath.size() - 2 ] : ""; }

        unsigned int getPathDepth() const { return _nodePath.size(); }

        CityGMLNodeType getPrevNodeType() const { return getNodeTypeFromName( getPrevNode() ); }

        void clearBuffer() { _buff.str(""); _buff.clear(); }

        void pushCityObject( CityObject* object );

        void popCityObject()
        {
            _currentCityObject = 0;
            if ( _cityObjectStack.empty() ) return;
            _currentCityObject = _cityObjectStack.top();
            _cityObjectStack.pop();
        }

        void pushObject( Object* object )
        {
            _objectStack.push( object );
            _currentObject = object;
        }

        void popObject();

        virtual std::string getAttribute( void* attributes, const std::string& attname, const std::string& defvalue = "" ) = 0;

        inline std::string getGmlIdAttribute( void* attributes ) { return getAttribute( attributes, "gml:id", "" ); }

        void createGeoTransform( std::string );

        static void initNodes();

        static std::string getNodeName( const std::string& );

        static CityGMLNodeType getNodeTypeFromName( const std::string& );

        AppearanceManager& getApperanceManagerForCurrentTheme();

        std::shared_ptr<Material> currentAppearanceAsMaterial();
        std::shared_ptr<Texture> currentAppearanceAsTexture();
        std::shared_ptr<GeoreferencedTexture> currentAppearanceAsGeoreferencedTexture();

        static std::map< std::string, CityGMLNodeType > s_cityGMLNodeTypeMap;
        static std::vector< std::string > s_knownNamespace;

        std::vector< std::string > _nodePath;

        std::stringstream _buff;

        ParserParams _params;

        CityModel* _model;

        TVec3d _translate;

        CityObject* _currentCityObject;
        std::stack<CityObject*> _cityObjectStack;

        Object* _currentObject;
        std::stack<Object*> _objectStack;

        std::vector<std::string> _cityGMLidStack; // Stores the citygml id of all elements in the current hierarchy

        Geometry* _currentGeometry;
        std::set<Geometry*> _geometries;
        std::stack<Geometry*> _geometryStack;

        ImplicitGeometry*           _currentImplicitGeometry;
        std::map<std::string, std::shared_ptr<Geometry>> _relativeGeometries;
        bool _isRelativeGeometry;

        Polygon* _currentPolygon;

        LinearRing* _currentRing;

        std::shared_ptr<Appearance> _currentAppearance;

        std::string _currentTheme;

        CityObjectsTypeMask _objectsMask;

        std::string _attributeName;

        int _currentLOD;

        bool _filterNodeType;
        unsigned int _filterDepth;

        std::vector<TVec3d> _points;

        int _srsDimension;

        char _orientation;

        bool _exterior;

        bool _appearanceAssigned;

        bool _referencePoint;

        Geometry::GeometryType _currentGeometryType;

        GeoTransform* _geoTransform;

        std::shared_ptr<CityGMLLogger> _logger;
        CityGMLFactory* _citygmlFactory;
        AppearanceManager* _appearanceManager;
    };
}
