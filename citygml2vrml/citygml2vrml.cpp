/* -*-c++-*- citygml2vrml - Copyright (c) 2010 Joachim Pouderoux, BRGM
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

#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <citygml/citygml.h>
#include <citygml/cityobject.h>
#include <citygml/citymodel.h>
#include <citygml/geometry.h>
#include <citygml/material.h>
#include <citygml/texture.h>
#include <citygml/geometry.h>
#include <citygml/polygon.h>

// VRML97 Helper class to produce a hierarchy of VRML nodes with attributes
class VRML97Printer
{
public:
    VRML97Printer( std::shared_ptr<const citygml::CityModel> city ) : _cityModel( city ), _indentCount( 0 ) {}

    bool save( const std::string& outFilename );

private:
    void dumpCityObject(const citygml::CityObject& );

    void dumpGeometry(const citygml::CityObject&, const citygml::Geometry& );

    void dumpPolygon(const citygml::CityObject&, const citygml::Geometry&, const citygml::Polygon& );

protected:
    void addHeader() { _out << "#VRML V2.0 utf8" << std::endl; }

    void printIndent() { for ( int i = 0; i < _indentCount; i++ ) _out << "\t"; }

    void addComment(const std::string& cmt) {  printIndent(); _out << "# " << cmt << std::endl; }

    void beginNode( const std::string &node ) { printIndent(); _out << node << " {" << std::endl; _indentCount++; }

    void endNode() { _indentCount--; printIndent(); _out << "}" << std::endl; }

    void addAttribute( const std::string &attr ) { printIndent(); _out << attr << " "; }

    template<class T> void addAttributeValue( const std::string &attr, T val ) { printIndent(); _out << attr << " " << val << std::endl; }

    void addAttributeValue( const std::string &attr, const char* val ) { printIndent(); _out << attr << " " << val << std::endl; }

    void beginAttributeNode( const std::string &attr, const std::string &node ) { printIndent(); _out << attr << " " << node << " {" << std::endl; _indentCount++; }

    void beginAttributeArray( const std::string &attr ) { addAttribute( attr ); _out << " [" << std::endl; _indentCount++; }

    void endAttributeArray() { _indentCount--; printIndent(); _out << "]" << std::endl; }

    void beginGroup() { beginNode("Group"); beginAttributeArray( "children" ); }

    void endGroup() { endAttributeArray(); endNode(); }

private:
    std::shared_ptr<const citygml::CityModel> _cityModel;
    int _indentCount;
    std::ofstream _out;
};

bool g_comments = false;
bool g_center = false;

void usage()
{
    std::cout << std::endl << "This program converts CityGML files to a VRML97 representation" << std::endl;
    std::cout << "More info & updates on http://code.google.com/p/libcitygml" << std::endl;
    std::cout << "Version built on " << __DATE__ << " at " << __TIME__ << " with libcitygml v." << LIBCITYGML_VERSIONSTR << std::endl << std::endl;
    std::cout << " Usage: citygml2vrml [-options...] <input.gml> [output.wrl]" << std::endl;
    std::cout << " Options:" << std::endl;
    std::cout << "  -optimize       Merge geometries & polygons with similar properties to" << std::endl
        << "                  reduce file & scene size (recommended)" << std::endl;
    std::cout << "  -comments       Add comments about the object ids to the VRML file" << std::endl;
    std::cout << "  -center         Center the model around the first encountered point" << std::endl
        << "                  (may be used to reduce z-fighting artifacts)" << std::endl;
    std::cout << "  -filter <mask>  CityGML objects to parse (default:All)" << std::endl
        << "                  The mask is composed of:" << std::endl
        << "                   GenericCityObject, Building, Room," << std::endl
        << "                   BuildingInstallation, BuildingFurniture, Door, Window, " << std::endl
        << "                   CityFurniture, Track, Road, Railway, Square, PlantCover," << std::endl
        << "                   SolitaryVegetationObject, WaterBody, TINRelief, LandUse," << std::endl
        << "                   Tunnel, Bridge, BridgeConstructionElement," << std::endl
        << "                   BridgeInstallation, BridgePart, All" << std::endl
        << "                  and seperators |,&,~." << std::endl
        << "                  Examples:" << std::endl
        << "                  \"All&~Track&~Room\" to parse everything but tracks & rooms" << std::endl
        << "                  \"Road&Railway\" to parse only roads & railways" << std::endl;
    std::cout << "  -minLOD <level> Minimum LOD level to parse (default:0)" << std::endl;
    std::cout << "  -maxLOD <level> Maximum LOD level to parse (default:4)" << std::endl;
    std::cout << "  -destSRS <srs> Destination SRS (default: no transform)" << std::endl;
    exit( EXIT_FAILURE );
}

int main( int argc, char **argv )
{
    std::cout << "citygml2vrml v." << LIBCITYGML_VERSIONSTR << " (c) 2010 Joachim Pouderoux, BRGM" << std::endl;

    int fargc = 1;

    citygml::ParserParams params;

    for ( int i = 1; i < argc; i++ )
    {
        std::string param = std::string( argv[i] );
        std::transform( param.begin(), param.end(), param.begin(), tolower );
        if ( param == "-optimize" ) { params.optimize = true; fargc = i+1; }
        if ( param == "-comments" ) { g_comments = true; fargc = i+1; }
        if ( param == "-center" ) { g_center = true; fargc = i+1; }
        if ( param == "-minlod" ) { if ( i == argc - 1 ) usage(); params.minLOD = atoi( argv[i+1] ); i++; fargc = i+1; }
        if ( param == "-maxlod" ) { if ( i == argc - 1 ) usage(); params.maxLOD = atoi( argv[i+1] ); i++; fargc = i+1; }
        if ( param == "-destsrs" ) { if ( i == argc - 1 ) usage(); params.destSRS = argv[i+1]; i++; fargc = i+1; }
    }

    if ( argc - fargc < 1 ) usage();

    std::cout << "Parsing CityGML file " << argv[fargc] << "..." << std::endl;

    time_t start;
    time( &start );

    std::shared_ptr<const citygml::CityModel> city = citygml::load( argv[fargc], params );

    time_t end;
    time( &end );

    if ( !city ) return EXIT_FAILURE;

    std::cout << "Done in " << difftime( end, start ) << " seconds." << std::endl;

    std::cout << city->getRootCityObjects().size() << " root nodes" << std::endl;

    std::string outfile;
    if ( argc - fargc == 1 )
    {
        outfile = argv[ fargc ];
        outfile = outfile.substr( 0, outfile.find_last_of( "." ) ) + ".wrl";
    }
    else outfile = argv[ fargc + 1 ];

    std::cout << "Converting the city model to VRML97 file " << outfile << "..." << std::endl;

    VRML97Printer printer( city );

    if ( printer.save( outfile ) )
        std::cout << "Done." << std::endl;
    else
        std::cout << "Failed!" << std::endl;

    return EXIT_SUCCESS;
}

// VRML97 city converter

bool VRML97Printer::save( const std::string& outFilename )
{
    if ( !_cityModel ) return false;

    _out.open( outFilename.c_str() );

    if ( _out.fail() ) { std::cerr << "Unable to create file " << outFilename << "!" << std::endl; return false; }

    addHeader();

    addComment( "Converted from a CityGML model using citygml2vrml (http://code.google.com/p/libcitygml)\n" );

#define RECURSIVE_DUMP

#ifndef RECURSIVE_DUMP
    const citygml::CityObjectsMap& cityObjectsMap = _cityModel->getCityObjectsMap();

    citygml::CityObjectsMap::const_iterator it = cityObjectsMap.begin();

    for ( ; it != cityObjectsMap.end(); it++ )
    {
        const citygml::CityObjects& v = it->second;

        std::cout << " Creation of " << v.size() << " " << citygml::getCityObjectsClassName( it->first ) << ( ( v.size() > 1 ) ? "s" : "" ) << "..." << std::endl;

        addComment( "Object type: " + citygml::getCityObjectsClassName( it->first ) + ( ( v.size() > 1 ) ? "s" : "" ) );

        beginGroup();
        for ( unsigned int i = 0; i < v.size(); i++ ) dumpCityObject( v[i] );
        endGroup();
    }
#else
    citygml::ConstCityObjects roots = _cityModel->getRootCityObjects();

    for ( unsigned int i = 0; i < roots.size(); i++ ) dumpCityObject( *roots[i] );
#endif

    _out.close();

    return true;
}

void VRML97Printer::dumpCityObject( const citygml::CityObject& object )
{

    if ( g_comments ) addComment(  object.getType() + ": " + object.getId() );

    beginGroup();

    for ( unsigned int i = 0; i < object.getGeometriesCount(); i++ ) dumpGeometry( object, object.getGeometry( i ) );

#ifdef RECURSIVE_DUMP
    for ( unsigned int i = 0; i < object.getChildCityObjectsCount(); i++ ) dumpCityObject( object.getChildCityObject(i) );
#endif

    endGroup();
}

void VRML97Printer::dumpGeometry( const citygml::CityObject& object, const citygml::Geometry& g )
{

    if ( g_comments ) addComment( "Geometry: " + g.getId() );

    for ( unsigned int i = 0; i < g.getPolygonsCount(); i++ ) dumpPolygon( object, g, g.getPolygon(i) );
}

void VRML97Printer::dumpPolygon( const citygml::CityObject& object, const citygml::Geometry& g, const citygml::Polygon& p )
{
    static bool s_isFirstVert = true;
    static TVec3d s_firstVert;

    if ( p.getIndices().size() == 0 ) return;

    if ( g_comments )
    {
        std::stringstream ss;
        ss << "  " << p.getVertices().size() << " points, " << p.getIndices().size()/3 << " triangles, " << p.getNormals().size() << " normals. ";
        addComment( "Polygon: " + p.getId() + ss.str() );
    }

    beginNode( "Shape" );

    // Geometry management
    beginAttributeNode( "geometry", "IndexedFaceSet" );

    {
        const std::vector<TVec3d>& vertices = p.getVertices();
        beginAttributeNode( "coord", "Coordinate" );
        beginAttributeArray( "point" );
        printIndent();
        if ( !g_center )
        {
            for ( unsigned int k = 0; k < vertices.size(); k++ ) _out << vertices[k] << ", ";
        }
        else
        {
            if ( s_isFirstVert ) { s_firstVert = vertices[0]; s_isFirstVert = false; }
            for ( unsigned int k = 0; k < vertices.size(); k++ ) _out << ( vertices[k] - s_firstVert ) << ", ";
        }

        _out << std::endl;
        endAttributeArray();
        endNode();
    }

    {
        const std::vector<unsigned int>& indices = p.getIndices();
        beginAttributeArray( "coordIndex" );
        printIndent();
        for ( unsigned int k = 0 ; k < indices.size() / 3; k++ )
            _out << indices[ k * 3 + 0 ] << " " << indices[ k * 3 + 1 ] << " " << indices[ k * 3 + 2 ] << " -1, ";
        _out << std::endl;
        endAttributeArray();
    }

    // Normal management
    if ( p.getNormals().size() > 0 )
    {
        const std::vector<TVec3f>& normals = p.getNormals();
        beginAttributeNode( "normal", "Normal" );
        beginAttributeArray( "vector" );
        printIndent();
        for ( unsigned int k = 0 ; k < normals.size(); k++ ) _out << normals[k] << ", ";
        _out << std::endl;
        endAttributeArray();
        endNode();
        addAttributeValue( "normalPerVertex", "TRUE" );
    }

    addAttributeValue( "solid", "FALSE" ); //draw both sides of faces

    // Texture coordinates
//    if ( std::dynamic_pointer_cast<const citygml::Texture>( p.getAppearance() ) && p.getTexCoords().size() > 0 )
//    {
//        const citygml::TexCoords& texCoords = p.getTexCoords();
//        beginAttributeNode( "texCoord", "TextureCoordinate" );

//        beginAttributeArray( "point" );
//        printIndent();
//        for ( unsigned int k = 0; k < texCoords.size(); k++ ) _out << texCoords[k] << ", ";
//        _out << std::endl;
//        endAttributeArray();

//        endNode();
//    }

    endNode();

    // Material management
    {
        beginAttributeNode( "appearance", "Appearance" );

        bool colorset = false;

//        std::shared_ptr<const citygml::Appearance> mat = p.getAppearance();

//        if ( auto m = std::dynamic_pointer_cast<const citygml::Material>( mat ) )
//        {
//            beginAttributeNode( "material", "Material" );

//            addAttributeValue( "diffuseColor", m->getDiffuse() );
//            addAttributeValue( "ambientIntensity", m->getAmbientIntensity() );
//            addAttributeValue( "specularColor", m->getSpecular() );
//            addAttributeValue( "emissiveColor", m->getEmissive() );
//            addAttributeValue( "shininess", m->getShininess() );
//            addAttributeValue( "transparency", m->getTransparency() );

//            endNode();
//            colorset = true;
//        }

//        if ( auto t = std::dynamic_pointer_cast<const citygml::Texture>( mat ) )
//        {
//            beginAttributeNode( "texture", "ImageTexture" );
//            addAttributeValue( "url", "\"" + t->getUrl() + "\"" );
//            endNode();
//            colorset = true;
//        }

        if ( !colorset )
        {
            beginAttributeNode( "material", "Material" );

            TVec4f color = TVec4f( 0.9f, 0.1f, 0.1f, 1.f );
            TVec3f crgb( color.r, color.g, color.b );
            addAttributeValue( "diffuseColor", crgb );
            if ( color.a != 1.f  ) addAttributeValue( "transparency", 1.f - color.a );

            endNode();
        }

        endNode();
    }

    // That's it!
    endNode();
}
