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

#ifndef __TESSELATOR_H__
#define __TESSELATOR_H__

#if defined(WIN32) || defined(_WIN32)
#	include <windows.h>
#else
#	define CALLBACK
#	define APIENTRY
#endif

#ifdef __APPLE__
  #include <OpenGL/glu.h>
#else
  #include <GL/glu.h>
#endif

#include <citygml/citygml_api.h>
#include <citygml/tesselatorbase.h>
#include <citygml/vecs.hpp>
#include <citygml/warnings.h>

namespace citygml {
    class CityGMLLogger;
}

// GLU based polygon tesselator
class LIBCITYGML_EXPORT Tesselator: public TesselatorBase
{
public:
    Tesselator( std::shared_ptr<citygml::CityGMLLogger> logger, GLenum winding_rule = GLU_TESS_WINDING_ODD);
    ~Tesselator();

    void init(const TVec3d& normal) override;

    /**
     * @brief Add a new contour - add the exterior ring first, then interiors
     * @param textureCoordinatesLists a list of texture coordinates lists for the countour. Each list contains one texture coordinate for each vertex.
     */
    void addContour(const std::vector<TVec3d>&, std::vector<std::vector<TVec2f> > textureCoordinatesLists) override;

    // Let's tesselate!
    void compute() override;

private:
    void processContours();

    typedef void (APIENTRY *GLU_TESS_CALLBACK)();
    static void CALLBACK beginCallback( GLenum, void* );
    static void CALLBACK vertexDataCallback( GLvoid*, void* );
    static void CALLBACK combineCallback( GLdouble[3], void* [4], GLfloat [4], void** , void* );
    static void CALLBACK endCallback( void* );
    static void CALLBACK errorCallback(GLenum, void*);

private:
    // 
    struct ContourRef {
        ContourRef(unsigned int index, unsigned int length) : index(index), length(length) {}
        unsigned int index;
        unsigned int length;
    };

    GLUtesselator *_tobj;
    GLenum _curMode;
    GLenum _windingRule;
    PRAGMA_WARN_DLL_BEGIN
    std::vector<TVec3d> _originalVertices;
    std::vector<ContourRef> _contourQueue;
    PRAGMA_WARN_DLL_END

};

#endif // __TESSELATOR_H__
