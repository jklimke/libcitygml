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

#include "citygml/tesselator.h"
#ifndef WIN32
#	include <stdint.h>
#endif

#include <citygml/citygml.h>
#include <citygml/citygmllogger.h>

Tesselator::Tesselator(std::shared_ptr<citygml::CityGMLLogger> logger )
{
    _logger = logger;
    _tobj = gluNewTess();

    gluTessCallback( _tobj, GLU_TESS_VERTEX_DATA, (GLU_TESS_CALLBACK)&vertexCallback );
    gluTessCallback( _tobj, GLU_TESS_BEGIN_DATA, (GLU_TESS_CALLBACK)&beginCallback );
    gluTessCallback( _tobj, GLU_TESS_END_DATA, (GLU_TESS_CALLBACK)&endCallback );
    gluTessCallback( _tobj, GLU_TESS_COMBINE_DATA, (GLU_TESS_CALLBACK)&combineCallback );
    gluTessCallback( _tobj, GLU_TESS_ERROR_DATA, (GLU_TESS_CALLBACK)&errorCallback );
}

void Tesselator::init( unsigned int verticesCount, const TVec3d& normal, GLenum winding_rule )
{
    gluTessBeginPolygon( _tobj, this );

    gluTessProperty( _tobj, GLU_TESS_WINDING_RULE, winding_rule );
    gluTessNormal( _tobj, normal.x, normal.y, normal.z );

    _vertices.clear();
    _vertices.reserve( verticesCount );
    _indices.clear();
    _curIndices.clear();
}

Tesselator::~Tesselator()
{
    gluDeleteTess( _tobj );
}

void Tesselator::compute()
{
    gluTessEndPolygon( _tobj );
}

void Tesselator::addContour( const std::vector<TVec3d>& pts )
{
    unsigned int len = pts.size();
    if ( len < 3 ) return;

    unsigned int pos = _vertices.size();

    gluTessBeginContour( _tobj );

    for ( unsigned int i = 0; i < len; i++ )
    {
        _vertices.push_back( pts[i] );

        gluTessVertex( _tobj, &(_vertices[pos+i][0]), (void*)(pos+i) );
    }

    gluTessEndContour( _tobj );
}

void CALLBACK Tesselator::beginCallback( GLenum which, void* userData )
{
    Tesselator* tess = static_cast<Tesselator*>(userData);
    tess->_curMode = which;
}

void CALLBACK Tesselator::vertexCallback( GLvoid *data, void* userData )
{
    Tesselator *tess = static_cast<Tesselator*>(userData);
    tess->_curIndices.push_back( (intptr_t)data );
}

void CALLBACK Tesselator::combineCallback( GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** outData, void* userData )
{
    Tesselator *tess = static_cast<Tesselator*>(userData);
    unsigned int npoint = tess->_vertices.size();
    tess->_vertices.push_back( TVec3d( coords[0], coords[1], coords[2] ) );

    *outData = (void*)npoint;
}

void CALLBACK Tesselator::endCallback( void* userData )
{
    Tesselator *tess = static_cast<Tesselator*>(userData);

    unsigned int len = tess->_curIndices.size();

    switch ( tess->_curMode )
    {
    case GL_TRIANGLES:
        for ( unsigned int i = 0; i < len; i++ ) tess->_indices.push_back( tess->_curIndices[i] );
        break;
    case GL_TRIANGLE_FAN:
    case GL_TRIANGLE_STRIP:
        {
            unsigned int first = tess->_curIndices[0];
            unsigned int prev = tess->_curIndices[1];

            for ( unsigned int i = 2; i < len; i++ )
            {
                if ( tess->_curMode == GL_TRIANGLE_FAN || i%2 == 0 ) tess->_indices.push_back( first );
                tess->_indices.push_back( prev );
                if ( tess->_curMode == GL_TRIANGLE_STRIP )
                {
                    if ( i%2 == 1) tess->_indices.push_back( first );
                    first = prev;
                }
                prev = tess->_curIndices[i];
                tess->_indices.push_back( prev );
            }
        }
        break;
    default:
        CITYGML_LOG_WARN(tess->_logger, "Tesselator: non-supported GLU tesselator primitive " << tess->_curMode);
    }
    tess->_curIndices.clear();
}

void CALLBACK Tesselator::errorCallback( GLenum errorCode, void* userData )
{
    Tesselator *tess = static_cast<Tesselator*>(userData);
    CITYGML_LOG_ERROR(tess->_logger, "Tesselator error: " << gluErrorString( errorCode ));
}
