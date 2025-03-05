include(GenerateExportHeader)
include(GNUInstallDirs)

IF (NOT DEFINED CMAKE_MODULE_PATH)
  GET_FILENAME_COMPONENT(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeModules" ABSOLUTE)
ENDIF (NOT DEFINED CMAKE_MODULE_PATH)

SET( target citygml )

IF( LIBCITYGML_USE_OPENGL)
  FIND_PACKAGE( OpenGL REQUIRED )
ELSE( LIBCITYGML_USE_OPENGL )
  SET( GLU_INCLUDE_PATH "" )
ENDIF( LIBCITYGML_USE_OPENGL )
FIND_PACKAGE( XercesC )
IF( NOT XercesC_FOUND )
  FIND_PACKAGE( Xerces REQUIRED )
ENDIF( NOT XercesC_FOUND)

# gdal library
OPTION(LIBCITYGML_USE_GDAL "Set to ON to build libcitygml with GDAL library so that it supports coordinates transformations." ON)

IF( LIBCITYGML_USE_GDAL )
  FIND_PACKAGE( GDAL REQUIRED )
  ADD_DEFINITIONS( -DUSE_GDAL )
ELSE( LIBCITYGML_USE_GDAL )
  SET( GDAL_INCLUDE_DIR "" )
  SET( GDAL_LIBRARY "" )
ENDIF( LIBCITYGML_USE_GDAL )

OPTION(LIBCITYGML_DEBUG_POSTFIX "Set to on to add \"d\" postfix to targets" ON)
IF( LIBCITYGML_DEBUG_POSTFIX )
  SET(CMAKE_DEBUG_POSTFIX  "d")
ELSE( LIBCITYGML_DEBUG_POSTFIX )
  UNSET(CMAKE_DEBUG_POSTFIX)
ENDIF( LIBCITYGML_DEBUG_POSTFIX )

# Dynamic vs Static Linking
OPTION(LIBCITYGML_DYNAMIC "Set to ON to build libcitygml for dynamic linking.  Use OFF for static." ON)
IF   (LIBCITYGML_DYNAMIC)
  SET(LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
  ADD_DEFINITIONS( -DLIBCITYGML_DYNAMIC )
ELSE (LIBCITYGML_DYNAMIC)
  SET(LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
  ADD_DEFINITIONS( -DLIBCITYGML_STATIC )
ENDIF(LIBCITYGML_DYNAMIC)

ADD_DEFINITIONS( -DCITYGML_LIBRARY )

ADD_DEFINITIONS( -DLIBCITYGML_BUILD )

configure_file("${CMAKE_MODULE_PATH}/citygml_api.h.in" ${CMAKE_CURRENT_SOURCE_DIR}/include/citygml/citygml_api.h)

SET(EXPORT_HEADER_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/citygml/citygml_export.h)

INCLUDE_DIRECTORIES(    ${CMAKE_CURRENT_SOURCE_DIR}/include
                        ${GLU_INCLUDE_PATH}
                        ${GDAL_INCLUDE_DIR})
IF ( NOT XercesC_FOUND )
  INCLUDE_DIRECTORIES(  ${XERCESC_INCLUDE})
ENDIF ( NOT XercesC_FOUND )

SET(SOURCES
  src/citygml/attributesmap.cpp
  src/citygml/citymodel.cpp
  src/citygml/tesselatorbase.cpp
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
  src/citygml/linestring.cpp
  src/citygml/address.cpp
  src/citygml/rectifiedgridcoverage.cpp
  src/citygml/externalreference.cpp

  src/parser/nodetypes.cpp
  src/parser/attributes.cpp

  src/parser/geocoordinatetransformer.cpp

  src/parser/citygmldocumentparser.cpp
  src/parser/parserxercesc.cpp
  src/parser/citygmlelementparser.cpp
  src/parser/elementparser.cpp

  src/parser/delayedchoiceelementparser.cpp
  src/parser/skipelementparser.cpp
  src/parser/sequenceparser.cpp

  src/parser/gmlobjectparser.cpp
  src/parser/gmlfeaturecollectionparser.cpp

  src/parser/citymodelelementparser.cpp
  src/parser/cityobjectelementparser.cpp
  src/parser/appearanceelementparser.cpp
  src/parser/externalreferenceparser.cpp
  src/parser/materialelementparser.cpp
  src/parser/textureelementparser.cpp
  src/parser/textureassociationelementparser.cpp
  src/parser/georeferencedtextureelementparser.cpp
  src/parser/geometryelementparser.cpp
  src/parser/polygonelementparser.cpp
  src/parser/rectifiedgridcoverageparser.cpp
  src/parser/linestringelementparser.cpp
  src/parser/linearringelementparser.cpp
  src/parser/implicitgeometryelementparser.cpp
  src/parser/addressparser.cpp
)

if(UNIX)
  if (APPLE)
    if (LIBCITYGML_USE_OPENGL)
      set_source_files_properties(src/citygml/tesselator.cpp PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations") # for warnings: 'glu*' is deprecated...
    endif (LIBCITYGML_USE_OPENGL)
  endif (APPLE)
endif(UNIX)

SET(PUBLIC_HEADER
  include/citygml/citygml_api.h
  include/citygml/citygml_export.h
  include/citygml/attributesmap.h
  include/citygml/enum_type_bitmask.h
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
  include/citygml/tesselatorbase.h
  include/citygml/texture.h
  include/citygml/appearancetargetdefinition.h
  include/citygml/texturetargetdefinition.h
  include/citygml/materialtargetdefinition.h
  include/citygml/texturecoordinates.h
  include/citygml/appearancetarget.h
  include/citygml/citygmlfactory.h
  include/citygml/linestring.h
  include/citygml/address.h
  include/citygml/rectifiedgridcoverage.h
  include/citygml/externalreference.h
  include/citygml/warnings.h
)


if(LIBCITYGML_USE_OPENGL) 
  SET(SOURCES 
    ${SOURCES} 
    src/citygml/tesselator.cpp
  )

  SET(PUBLIC_HEADER 
    ${PUBLIC_HEADER} 
    include/citygml/tesselator.h
  )
endif(LIBCITYGML_USE_OPENGL)

SET(HEADERS
  ${PUBLIC_HEADER}

  ${CMAKE_MODULE_PATH}/citygml_api.h.in

  include/citygml/utils.h
  include/citygml/appearancemanager.h
  include/citygml/polygonmanager.h
  include/citygml/geometrymanager.h

  include/parser/nodetypes.h
  include/parser/attributes.h
  include/parser/documentlocation.h

  include/parser/parserutils.hpp
  include/parser/geocoordinatetransformer.h

  include/parser/citygmldocumentparser.h
  include/parser/citygmlelementparser.h
  include/parser/elementparser.h

  include/parser/delayedchoiceelementparser.h
  include/parser/skipelementparser.h
  include/parser/sequenceparser.h

  include/parser/gmlobjectparser.h
  include/parser/gmlfeaturecollectionparser.h

  include/parser/citymodelelementparser.h
  include/parser/cityobjectelementparser.h
  include/parser/appearanceelementparser.h
  include/parser/externalreferenceparser.h
  include/parser/materialelementparser.h
  include/parser/textureelementparser.h
  include/parser/georeferencedtextureelementparser.h
  include/parser/geometryelementparser.h
  include/parser/polygonelementparser.h
  include/parser/rectifiedgridcoverageparser.h
  include/parser/linestringelementparser.h
  include/parser/linearringelementparser.h
  include/parser/implicitgeometryelementparser.h
  include/parser/addressparser.h
)

ADD_LIBRARY( ${target} ${LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC} ${SOURCES} ${HEADERS} )


generate_export_header(citygml
                       EXPORT_MACRO_NAME LIBCITYGML_EXPORT
                       EXPORT_FILE_NAME ${EXPORT_HEADER_FILE_NAME})

if(XercesC_FOUND)
  TARGET_LINK_LIBRARIES( ${target} PRIVATE XercesC::XercesC)
else(XercesC_FOUND)
  TARGET_LINK_LIBRARIES( ${target} PRIVATE ${XERCESC_LIBRARIES} )
endif(XercesC_FOUND)

if(LIBCITYGML_USE_OPENGL)
  TARGET_LINK_LIBRARIES( ${target} PRIVATE ${OPENGL_LIBRARIES} )
endif(LIBCITYGML_USE_OPENGL)
if(LIBCITYGML_USE_GDAL)
    TARGET_LINK_LIBRARIES( ${target} PRIVATE ${GDAL_LIBRARY} )
endif(LIBCITYGML_USE_GDAL)

set_target_properties( ${target} PROPERTIES
    VERSION ${META_VERSION}
    SOVERSION ${META_VERSION_MAJOR}
)

if(NOT DEFINED INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR "include")
endif(NOT DEFINED INCLUDE_INSTALL_DIR)
set(INCLUDE_INSTALL_CITYGML_DIR "${CMAKE_INSTALL_PREFIX}/${INCLUDE_INSTALL_DIR}/citygml")

if(WIN32)
  set(CONFIG_INSTALL_DIR "cmake")
else()
  set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/citygml")
endif()

INSTALL(
    TARGETS ${target}
    COMPONENT "runtime"
    EXPORT citygmlConfigInternal
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
)
INSTALL(
    EXPORT citygmlConfigInternal
    DESTINATION "${CONFIG_INSTALL_DIR}"
    NAMESPACE "citygml::"
    COMPONENT "development"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/citygmlConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/citygmlConfig.cmake"
  INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}")
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/citygmlConfigVersion.cmake
  VERSION "${META_VERSION}"
  COMPATIBILITY SameMajorVersion)
INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/citygmlConfig.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/citygmlConfigVersion.cmake
    DESTINATION "${CONFIG_INSTALL_DIR}"
    COMPONENT "development"
)

# FIXME: Do not run for OS X framework
INSTALL(
    FILES
      ${PUBLIC_HEADER}
    DESTINATION  ${INCLUDE_INSTALL_CITYGML_DIR}
    COMPONENT "development"
)

INSTALL(
    FILES
      ${EXPORT_HEADER_FILE_NAME}
    DESTINATION
      ${INCLUDE_INSTALL_CITYGML_DIR}
    COMPONENT "development"
)

if (LIBCITYGML_DEBUG_POSTFIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBCITYGML_POSTFIX ${CMAKE_DEBUG_POSTFIX})
else()
    set(LIBCITYGML_POSTFIX "")
endif()

set(PKG_CONFIG_REQUIRES "xerces-c")

if(LIBCITYGML_USE_OPENGL)
  set(PKG_CONFIG_REQUIRES "${PKG_CONFIG_REQUIRES} glu")
  endif(LIBCITYGML_USE_OPENGL)

if (LIBCITYGML_USE_GDAL)
    set(PKG_CONFIG_REQUIRES "${PKG_CONFIG_REQUIRES} gdal")
endif()

configure_file(citygml.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/citygml.pc)

INSTALL(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/citygml.pc 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig
    COMPONENT "development"
)

