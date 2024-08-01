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

#ifndef __TESSELATORBASE_H__
#define __TESSELATORBASE_H__

#include <citygml/citygml_api.h>
#include <citygml/vecs.hpp>
#include <vector>
#include <memory>

namespace citygml {
    class CityGMLLogger;
}

// base class for polygon tesselator
class LIBCITYGML_EXPORT TesselatorBase
{
public:
    TesselatorBase( std::shared_ptr<citygml::CityGMLLogger> logger );
    virtual ~TesselatorBase();

    void setLogger(std::shared_ptr<citygml::CityGMLLogger> logger);

    virtual void init(const TVec3d& normal) = 0;

    /**
     * @brief Add a new contour - add the exterior ring first, then interiors
     * @param textureCoordinatesLists a list of texture coordinates lists for the countour. Each list contains one texture coordinate for each vertex.
     */
    virtual void addContour(const std::vector<TVec3d>&, std::vector<std::vector<TVec2f> > textureCoordinatesLists);

    // Let's tesselate!
    virtual void compute() = 0;

    // Tesselation result access
    const std::vector<TVec3d>& getVertices() const;
    const std::vector<std::vector<TVec2f> >& getTexCoords() const { return _texCoordsLists; }
    const std::vector<unsigned int>& getIndices() const;

    void setKeepVertices(bool val);
    bool keepVertices() const;

protected:
#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4251 4275)
#endif
    std::vector<TVec3d> _vertices;
    std::vector<std::vector<TVec2f> > _texCoordsLists;
    std::vector<unsigned int> _indices;
    std::vector<unsigned int> _outIndices;

    std::vector<unsigned int> _curIndices;
    std::shared_ptr<citygml::CityGMLLogger> _logger;
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

    bool _keepVertices;
};

#endif // __TESSELATORBASE_H__
