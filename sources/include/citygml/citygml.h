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

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <memory>
#include <functional>
#include <unordered_set>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <citygml/citygml_api.h>
#include <citygml/vecs.hpp>
#include <citygml/cityobject.h>
#include <citygml/envelope.h>


class Tesselator;

namespace citygml
{
    class CityModel;
    class CityGMLLogger;
    class Appearance;
    class Texture;
    class Material;
    class AppearanceManager;

    typedef EnumClassBitmask<CityObject::CityObjectsType> CityObjectsTypeMask;


    ///////////////////////////////////////////////////////////////////////////////
    // Parsing routines

    // Parameters:
    // objectsMask: a bit mask that defines which CityObjectsTypes are parsed
    //    examples: CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Room <- parses only Building and Room objects"
    // minLOD: the minimal LOD that will be parsed
    // maxLOD: the maximal LOD that will be parsed
    // optimize: merge geometries & polygons that share the same appearance in the same object in order to reduce the global hierarchy
    // pruneEmptyObjects: remove the objects which do not contains any geometrical entity
    // tesselate: convert the interior & exteriors polygons to triangles
    // destSRS: the SRS (WKT, EPSG, OGC URN, etc.) where the coordinates must be transformed, default ("") is no transformation
    // srsSRS: the SRS (WKT, EPSG, OGC URN, etc.) to overrride the SRS in the CityGML data (if any), default ("") means no override or use included SRS

    class LIBCITYGML_EXPORT ParserParams
    {
    public:
        ParserParams()
            : objectsMask(CityObject::CityObjectsType::COT_All)
            , minLOD( 0 )
            , maxLOD( 4 )
            , optimize( false )
            , pruneEmptyObjects( false )
            , destSRS( "" )
            , srcSRS( "" )
            , keepVertices ( false )
        { }

    public:
        CityObjectsTypeMask objectsMask;
        unsigned int minLOD;
        unsigned int maxLOD;
        bool optimize;
        bool pruneEmptyObjects;
        bool tesselate;
        bool keepVertices;
        std::string destSRS;
        std::string srcSRS;
    };

    LIBCITYGML_EXPORT std::shared_ptr<const CityModel> load( std::istream& stream, const ParserParams& params, std::shared_ptr<CityGMLLogger> logger = nullptr);

    LIBCITYGML_EXPORT std::shared_ptr<const CityModel> load( const std::string& fileName, const ParserParams& params, std::shared_ptr<CityGMLLogger> logger = nullptr);

}
