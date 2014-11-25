SET( target citygml )

ADD_DEFINITIONS( -DCITYGML_LIBRARY )

ADD_DEFINITIONS( -DLIBCITYGML_BUILD )

IF( LIBCITYGML_STATIC )
  ADD_DEFINITIONS( -DLIBCITYGML_STATIC )
ENDIF( LIBCITYGML_STATIC )

IF( LIBCITYGML_DYNAMIC )
  ADD_DEFINITIONS( -DLIBCITYGML_DYNAMIC )
ENDIF( LIBCITYGML_DYNAMIC )

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/sources/include ${XERCESC_INCLUDE} ${LIBXML2_INCLUDE_DIR} ${ICONV_INCLUDE_DIR} ${GLU_INCLUDE_PATH} ${GDAL_INCLUDE_DIR} )

SET(SOURCES
  src/citygml/citymodel.cpp
  src/citygml/tesselator.cpp
  src/citygml/object.cpp
  src/citygml/appearance.cpp
  src/citygml/texture.cpp
  src/citygml/material.cpp
  src/citygml/georeferencedtexture.cpp
  src/citygml/envelope.cpp
  src/citygml/appearancemanager.cpp
  src/citygml/cityobject.cpp
  src/citygml/composite.cpp
  src/citygml/geometry.cpp
  src/citygml/implictgeometry.cpp
  src/citygml/landuse.cpp
  src/citygml/linearring.cpp
  src/citygml/polygon.cpp
  src/citygml/transformmatrix.cpp
  src/citygml/texturetarget.cpp
  src/citygml/texturecoordinates.cpp
  src/citygml/appearancetarget.cpp
  src/citygml/citygmlfactory.cpp

  src/parser/parser.cpp
  src/parser/parserxercesc.cpp
  src/parser/parserlibxml2.cpp
)

SET(PUBLIC_HEADER
  include/citygml/citygml_api.h
  include/citygml/citygmllogger.h
  include/citygml/polygon.h
  include/citygml/material.h
  include/citygml/geometry.h
  include/citygml/appearancemanager.h
  include/citygml/object.h
  include/citygml/georeferencedtexture.h
  include/citygml/cityobject.h
  include/citygml/envelope.h
  include/citygml/landuse.h
  include/citygml/appearance.h
  include/citygml/vecs.hpp
  include/citygml/citymodel.h
  include/citygml/composite.h
  include/citygml/linearring.h
  include/citygml/citygml.h
  include/citygml/transformmatrix.h
  include/citygml/implictgeometry.h
  include/citygml/tesselator.h
  include/citygml/texture.h
  include/citygml/texturetarget.h
  include/citygml/texturecoordinates.h
  include/citygml/cityobjectdeclerations.hpp
  include/citygml/appearancetarget.h
  include/citygml/citygmlfactory.h
)

SET(HEADERS
  ${PUBLIC_HEADER}

  include/parser/parser.h
  include/parser/transform.h
  include/citygml/tesselator.h
  include/citygml/utils.h
)

ADD_LIBRARY( ${target} ${LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC} ${SOURCES} ${HEADERS} )

TARGET_LINK_LIBRARIES( ${target} ${XERCESC_LIBRARIES} ${LIBXML2_LIBRARIES} ${OPENGL_LIBRARIES} ${GDAL_LIBRARY} )

INSTALL(
    TARGETS ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
# FIXME: Do not run for OS X framework
INSTALL(
    FILES        ${PUBLIC_HEADER}
    DESTINATION  ${INSTALL_INCDIR}/${target}
)

