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
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>

#ifdef LIBCITYGML_USE_OPENGL
#include <citygml/tesselator.h>
#endif //LIBCITYGML_USE_OPENGL

void analyzeObject( const citygml::CityObject&, unsigned int );

void usage()
{
    std::cout << "Usage: citygmltest [-options...] <filename>" << std::endl;
    std::cout << " Options:" << std::endl;
    std::cout << "  -log            Print some information during parsing" << std::endl;
    std::cout << "  -filter <mask>  CityGML objects to parse (default:All)" << std::endl
        << "                  The mask is composed of:" << std::endl
        << "                   GenericCityObject, Building, Room," << std::endl
        << "                   BuildingInstallation, BuildingFurniture, IntBuildingInstallation, Door, Window, " << std::endl
        << "                   CityFurniture, Track, Road, Railway, Square, PlantCover," << std::endl
        << "                   SolitaryVegetationObject, WaterBody, TINRelief, LandUse," << std::endl
        << "                   Tunnel, Bridge, BridgeConstructionElement," << std::endl
        << "                   BridgeInstallation, BridgePart, All" << std::endl
        << "                  and separators |,&,~." << std::endl
        << "                  Examples:" << std::endl
        << "                  \"All&~Track&~Room\" to parse everything but tracks & rooms" << std::endl
        << "                  \"Road&Railway\" to parse only roads & railways" << std::endl;
    std::cout << "  -destSRS <srs> Destination SRS (default: no transform)" << std::endl;
    exit( EXIT_FAILURE );
}

int main( int argc, char **argv )
{
    if ( argc < 2 ) usage();

    int fargc = 1;

    bool log = false;

    citygml::ParserParams params;

    for ( int i = 1; i < argc; i++ )
    {
        std::string param = std::string( argv[i] );
        std::transform( param.begin(), param.end(), param.begin(), tolower );
        if ( param == "-log" ) { log = true; fargc = i+1; }
        //if ( param == "-filter" ) { if ( i == argc - 1 ) usage(); params.objectsMask = argv[i+1]; i++; fargc = i+1; }
        if ( param == "-destsrs" ) { if ( i == argc - 1 ) usage(); params.destSRS = argv[i+1]; i++; fargc = i+1; }
        if ( param == "-srcsrs" ) { if ( i == argc - 1 ) usage(); params.srcSRS = argv[i+1]; i++; fargc = i+1; }
    }

    if ( argc - fargc < 1 ) usage();

    std::cout << "Parsing CityGML file " << argv[fargc] << " using libcitygml v." << LIBCITYGML_VERSIONSTR << "..." << std::endl;

    time_t start;
    time( &start );

#if 0
    std::ifstream file;
    file.open( argv[fargc], std::ifstream::in );
     std::shared_ptr<const citygml::CityModel> city = citygml::load( file, params );
#else

    std::shared_ptr<const citygml::CityModel> city;
#ifdef LIBCITYGML_USE_OPENGL
    std::unique_ptr<TesselatorBase> tesselator = std::unique_ptr<TesselatorBase>(new Tesselator(nullptr));
#else
    std::unique_ptr<TesselatorBase> tesselator = nullptr;
#endif // LIBCITYGML_USE_OPENGL
    try{
        city = citygml::load( argv[fargc], params, std::move(tesselator) );
    }catch(const std::runtime_error& e){
        
    }
#endif

    time_t end;
    time( &end );

    if ( !city ) return EXIT_FAILURE;

    std::cout << "Done in " << difftime( end, start ) << " seconds." << std::endl;

    
    std::cout << "Analyzing the city objects..." << std::endl;
    
    const auto& cityObjects = city->getRootCityObjects();

    citygml::ConstCityObjects::const_iterator it = cityObjects.begin();

    for ( ; it != cityObjects.end(); ++it )
    {
        const citygml::CityObject& v = **it;

        //std::cout << ( log ? " Analyzing " : " Found " ) << v.size() << " " << v.getTypeAsString() << ( ( v.size() > 1 ) ? "s" : "" ) << "..." << std::endl;

        // if ( log )
        // {
        //       std::cout << "  + found object " << v.getId();
        //       if ( v.getChildCityObjectsCount() > 0 ) std::cout << " with " << v.getChildCityObjectsCount() << " children";
        //       std::cout << " with " << v.getGeometriesCount() << " geometr" << ( ( v.getGeometriesCount() > 1 ) ? "ies" : "y" );
        //       std::cout << " with " << v.getImplicitGeometryCount() << " implicit geometr" << ( ( v.getImplicitGeometryCount() > 1 ) ? "ies" : "y" );
        //       std::cout << std::endl;
        // }
    }
    

    if ( log )
    {
        std::cout << std::endl << "Objects hierarchy:" << std::endl;
        const citygml::ConstCityObjects& roots = city->getRootCityObjects();
        for ( unsigned int i = 0; i < roots.size(); i++ ) analyzeObject( *(roots[ i ]), 2 );
    }

    std::cout << "Done." << std::endl;

    return EXIT_SUCCESS;
}

void printIndent(unsigned int indent) {
  for ( unsigned int i = 0; i < indent; i++ ) std::cout << " ";
}

void printGeometry( const citygml::Geometry& geometry, unsigned int indent ) 
{
  printIndent(indent);
  std::cout << "Geometry for LOD" << geometry.getLOD() << ", type: " << geometry.getTypeAsString() << std::endl;
  
  if(geometry.getLineStringCount() > 0)
  {
    printIndent(indent+1);
    std::cout << "Linestrings:" << geometry.getLineStringCount() << std::endl;
  }
  
  if(geometry.getPolygonsCount() > 0)
  {
    printIndent(indent+1);
    std::cout << "Polygons:" << geometry.getPolygonsCount() << std::endl;
  }
  
  if(geometry.getGeometriesCount() >0)
  {
    printIndent(indent+1);
    std::cout << "SubGeometries:" << std::endl;
    for( unsigned int i = 0; i < geometry.getGeometriesCount(); i++ )
    {
      printGeometry(geometry.getGeometry(i), indent+1);
    }
  }
}


void printImplicitGeometry( const citygml::ImplicitGeometry& implicitGeometry, unsigned int indent ) 
{
    printIndent(indent);
    std::cout << "Reference point " << implicitGeometry.getReferencePoint() << std::endl;
    for ( unsigned int i = 0; i < implicitGeometry.getGeometriesCount(); i++ ) 
    {
        printGeometry( implicitGeometry.getGeometry(i), indent+1 );
    }
}

void analyzeObject( const citygml::CityObject& object, unsigned int indent )
{
   printIndent(indent);
   std::cout << "Object " << object.getTypeAsString() << ": " << object.getId() << std::endl;
   
   
   for ( unsigned int i = 0; i < object.getGeometriesCount(); i++ ) 
   {
       printGeometry( object.getGeometry(i), indent+1 );
   }
   for ( unsigned int i = 0; i < object.getImplicitGeometryCount(); i++ ) 
   {
       printImplicitGeometry( object.getImplicitGeometry(i), indent+1 );
   }

   for ( unsigned int i = 0; i < object.getChildCityObjectsCount(); i++ )
       analyzeObject( object.getChildCityObject(i), indent+1 );
}
