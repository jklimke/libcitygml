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

#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "citygml/citygml.h"
#ifdef USE_GDAL
#	include "ogrsf_frmts.h"
#endif

class GeoTransform
{
public:
    GeoTransform( const std::string& sourceURN, const std::string& destURN, std::shared_ptr<citygml::CityGMLLogger> logger ) : _sourceURN( sourceURN ), _destURN( destURN )
    {
#ifdef USE_GDAL
        _sourceSRS = getProjection( _sourceURN, logger );
        _destSRS = getProjection( _destURN, logger );
        _trans = ( _sourceSRS && _destSRS ) ? OGRCreateCoordinateTransformation( (OGRSpatialReference*)_sourceSRS, (OGRSpatialReference*)_destSRS ) : 0;
#else
        CITYGML_LOG_WARN(logger, "GeoTransform: Trying to create a Coordinate Tranformation without GDAL support enabled. The data will stay at its source coordinate system.");
        _sourceSRS = 0;
        _destSRS = 0;
        _trans = 0;
#endif
    }

    ~GeoTransform()
    {
#ifdef USE_GDAL
        if ( _sourceSRS ) OSRDestroySpatialReference( _sourceSRS );
        if ( _destSRS ) OSRDestroySpatialReference( _destSRS );
        if ( _trans ) OCTDestroyCoordinateTransformation( _trans );
#endif
    }

#ifdef USE_GDAL
    void transform( TVec3d &p ) const
    {
        if ( _trans ) ((OGRCoordinateTransformation*)_trans)->Transform( 1, &p.x, &p.y );
#else
    void transform( TVec3d & ) const
    {
#endif
    }

#ifdef USE_GDAL
    void transform( TVec2d &p ) const
    {
        if ( _trans ) ((OGRCoordinateTransformation*)_trans)->Transform( 1, &p.x, &p.y );
#else
    void transform( TVec2d & ) const
    {
#endif
    }

    inline const std::string& getSourceURN() const { return _sourceURN; }

    inline const std::string& getDestURN() const { return _destURN; }

#ifdef USE_GDAL
    static void* getProjection( const std::string &str, std::shared_ptr<citygml::CityGMLLogger> logger )
    {
        OGRSpatialReference* proj = (OGRSpatialReference*)OSRNewSpatialReference(0);
        OGRErr err = proj->SetFromUserInput( str.c_str() );
        if ( err == OGRERR_NONE ) return proj;

        delete proj;
        CITYGML_LOG_ERROR(logger, "GeoTransform: Unable to create projection from definition " << str << " (error code: " << err << ")... Did you correctly set the GDAL_DATA env. var?");
        return 0;
    }
#endif

private:
    std::string _sourceURN;
    std::string _destURN;
    void* _sourceSRS;
    void* _destSRS;
    void* _trans;
};

#endif // __TRANSFORM_H__
