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

#ifdef WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#endif

#include <citygml/tesselatorbase.h>
#ifndef WIN32
#	include <stdint.h>
#endif

#include <citygml/citygml.h>
#include <citygml/citygmllogger.h>

#include <iostream>
#include <assert.h>
#include <algorithm>

TesselatorBase::TesselatorBase(std::shared_ptr<citygml::CityGMLLogger> logger )
{
    _logger = logger;
    _keepVertices = false;
}

void TesselatorBase::init( const TVec3d& normal)
{
    _vertices.clear();
    _indices.clear();
    _curIndices.clear();
    _texCoordsLists.clear();
    _outIndices.clear();
}

TesselatorBase::~TesselatorBase()
{
}

void TesselatorBase::setLogger(std::shared_ptr<citygml::CityGMLLogger> logger)
{
    _logger = logger;
}

const std::vector<TVec3d>& TesselatorBase::getVertices() const
{
    return _vertices;
}

const std::vector<unsigned int>& TesselatorBase::getIndices() const
{
    return _outIndices;
}

void TesselatorBase::setKeepVertices(bool value)
{
    _keepVertices = value;
}

bool TesselatorBase::keepVertices() const
{
    return _keepVertices;
}

void TesselatorBase::addContour(const std::vector<TVec3d>& pts, std::vector<std::vector<TVec2f> > textureCoordinatesLists )
{
    unsigned int len = static_cast<unsigned int>(pts.size());
    if ( len < 3 ) return;

    for (size_t i = 0; i < textureCoordinatesLists.size(); i++) {

        std::vector<TVec2f>& texCoords = textureCoordinatesLists.at(i);



        if (texCoords.size() != pts.size()) {
            if (!texCoords.empty()) {
                CITYGML_LOG_ERROR(_logger, "Invalid call to 'addContour'. The number of texture coordinates in list " << i << " (" << texCoords.size() << ") "
                             "does not match the number of vertices (" << pts.size() << "). The texture coordinates list will be resized which may cause invalid texture coordinates.");
            }

            texCoords.resize(pts.size(), TVec2f(0.f, 0.f));
        }
    }

    for (size_t i = 0; i < std::max(_texCoordsLists.size(), textureCoordinatesLists.size()); i++) {

        if (i >= _texCoordsLists.size()) {
            std::vector<TVec2f> texCoords(_vertices.size(), TVec2f(0.f, 0.f));
            texCoords.insert(texCoords.end(), textureCoordinatesLists.at(i).begin(), textureCoordinatesLists.at(i).end());
            _texCoordsLists.push_back(texCoords);
        } else if (i >= textureCoordinatesLists.size()) {
            _texCoordsLists.at(i).resize(_texCoordsLists.at(i).size() + pts.size(), TVec2f(0.f, 0.f));
        } else {
            _texCoordsLists.at(i).insert(_texCoordsLists.at(i).end(), textureCoordinatesLists.at(i).begin(), textureCoordinatesLists.at(i).end());
        }

    }

    unsigned int pos = static_cast<unsigned int>(_vertices.size());

    for ( unsigned int i = 0; i < len; i++ )
    {
        _vertices.push_back( pts[i] );
        _indices.push_back(pos + i);
    }

#ifndef NDEBUG
    for (size_t i = 0; i < _texCoordsLists.size(); i++) {
        assert(_texCoordsLists.at(i).size() == _vertices.size());
    }
#endif
}
