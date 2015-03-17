SET( target citygml )

ADD_DEFINITIONS( -DCITYGML_LIBRARY )

ADD_DEFINITIONS( -DLIBCITYGML_BUILD )

IF( LIBCITYGML_STATIC )
  ADD_DEFINITIONS( -DLIBCITYGML_STATIC )
ENDIF( LIBCITYGML_STATIC )

IF( LIBCITYGML_DYNAMIC )
  ADD_DEFINITIONS( -DLIBCITYGML_DYNAMIC )
ENDIF( LIBCITYGML_DYNAMIC )

configure_file(include/citygml/citygml_api.h.in ${CMAKE_BINARY_DIR}/include/citygml/citygml_api.h)

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/sources/include ${XERCESC_INCLUDE}  ${ICONV_INCLUDE_DIR} ${GLU_INCLUDE_PATH} ${GDAL_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/include)

SET(SOURCES
  src/citygml/citymodel.cpp
  src/citygml/tesselator.cpp
  src/citygml/object.cpp
  src/citygml/featureobject.cpp
  src/citygml/appearance.cpp
  src/citygml/texture.cpp
  src/citygml/material.cpp
  src/citygml/georeferencedtexture.cpp
  src/citygml/envelope.cpp
  src/citygml/appearancemanager.cpp
  src/citygml/cityobject.cpp
  src/citygml/geometry.cpp
  src/citygml/implictgeometry.cpp
  src/citygml/linearring.cpp
  src/citygml/polygon.cpp
  src/citygml/transformmatrix.cpp
  src/citygml/texturetargetdefinition.cpp
  src/citygml/materialtargetdefinition.cpp
  src/citygml/texturecoordinates.cpp
  src/citygml/appearancetarget.cpp
  src/citygml/citygmlfactory.cpp
  src/citygml/polygonmanager.cpp
  src/citygml/geometrymanager.cpp

  src/parser/nodetypes.cpp
  src/parser/citygmldocumentparser.cpp
  src/parser/citygmlelementparser.cpp
  src/parser/elementparser.cpp
  src/parser/delayedchoiceelementparser.cpp
  src/parser/attributes.cpp
  src/parser/parserxercesc.cpp
  src/parser/geocoordinatetransformer.cpp

  src/parser/gmlobjectparser.cpp
  src/parser/gmlfeaturecollectionparser.cpp
  src/parser/citymodelelementparser.cpp
  src/parser/cityobjectelementparser.cpp
  src/parser/appearanceelementparser.cpp
  src/parser/materialelementparser.cpp
  src/parser/textureelementparser.cpp
  src/parser/georeferencedtextureelementparser.cpp
  src/parser/geometryelementparser.cpp
  src/parser/polygonelementparser.cpp
  src/parser/linearringelementparser.cpp
  src/parser/implicitgeometryelementparser.cpp
)

SET(PUBLIC_HEADER
  ${CMAKE_BINARY_DIR}/include/citygml/citygml_api.h
  include/citygml/citygmllogger.h
  include/citygml/polygon.h
  include/citygml/material.h
  include/citygml/geometry.h
  include/citygml/object.h
  include/citygml/featureobject.h
  include/citygml/georeferencedtexture.h
  include/citygml/cityobject.h
  include/citygml/envelope.h
  include/citygml/appearance.h
  include/citygml/vecs.hpp
  include/citygml/citymodel.h
  include/citygml/linearring.h
  include/citygml/citygml.h
  include/citygml/transformmatrix.h
  include/citygml/implictgeometry.h
  include/citygml/tesselator.h
  include/citygml/texture.h
  include/citygml/appearancetargetdefinition.h
  include/citygml/texturetargetdefinition.h
  include/citygml/materialtargetdefinition.h
  include/citygml/texturecoordinates.h
  include/citygml/appearancetarget.h
  include/citygml/citygmlfactory.h
)

SET(HEADERS
  ${PUBLIC_HEADER}

  include/citygml/citygml_api.h.in

  include/parser/nodetypes.h
  include/parser/citygmldocumentparser.h
  include/parser/citygmlelementparser.h
  include/parser/elementparser.h
  include/parser/delayedchoiceelementparser.h
  include/parser/attributes.h
  include/parser/documentlocation.h
  include/parser/geocoordinatetransformer.h
  include/parser/parserutils.hpp
  include/citygml/tesselator.h
  include/citygml/utils.h
  include/citygml/appearancemanager.h
  include/citygml/polygonmanager.h
  include/citygml/geometrymanager.h

  include/parser/gmlobjectparser.h
  include/parser/gmlfeaturecollectionparser.h
  include/parser/citymodelelementparser.h
  include/parser/cityobjectelementparser.h
  include/parser/appearanceelementparser.h
  include/parser/materialelementparser.h
  include/parser/textureelementparser.h
  include/parser/georeferencedtextureelementparser.h
  include/parser/geometryelementparser.h
  include/parser/polygonelementparser.h
  include/parser/linearringelementparser.h
  include/parser/implicitgeometryelementparser.h
)

ADD_LIBRARY( ${target} ${LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC} ${SOURCES} ${HEADERS} )

TARGET_LINK_LIBRARIES( ${target} ${XERCESC_LIBRARIES} ${OPENGL_LIBRARIES} ${GDAL_LIBRARY} )

INSTALL(
    TARGETS ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
# FIXME: Do not run for OS X framework
INSTALL(
    FILES        ${PUBLIC_HEADER}
    DESTINATION  ${CMAKE_INSTALL_PREFIX}/include/${target}
)

