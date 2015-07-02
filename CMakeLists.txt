CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2 FATAL_ERROR)

PROJECT ( libcitygml )

set(META_VERSION_MAJOR       "1")
set(META_VERSION_MINOR       "4")
set(META_VERSION_PATCH       "2")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")

SET( CMAKE_MODULE_PATH "${libcitygml_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}" )

IF(WIN32)

    IF(MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

        # More MSVC specific compilation flags
        ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ENDIF(MSVC)

    # Both Cygwin and Msys need -DNOMINMAX ???
    IF(UNIX)
        ADD_DEFINITIONS(-DNOMINMAX)
    ENDIF(UNIX)

########################################################################################################
# the foolowing options are MSVC specific,
# the first OSG_MSVC_VERSIONED_DLL activate a custom build-time layout that should allow to run examples and application
# fron bin folder without requiring installation step.
# it also prepend "osg${OPENSCENEGRAPH_SOVERSION}-" to only .dll files, leaving .lib files untouched in lib
# it also use a hack to get rid of Debug and Release folder in MSVC projects
# all the .dll and .pdb are in bin and all the .lib and .exp are in lib
#
# the second option disable incremental linking in debug build , that is enabled by default by CMake
##########################################################################################################

    IF(MSVC)
        # IF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4 AND ${CMAKE_PATCH_VERSION} LESS 7)
            # MESSAGE("Warning:  disabling versioned options 2.4.6 exibits inconsintencies in .pdb naming, at least under MSVC, suggested upgrading at least to 2.4.7")
            # SET(OSG_MSVC_VERSIONED_DLL OFF)
            # SET(OSG_MSVC_DEBUG_INCREMENTAL_LINK ON)
        # ELSE(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4 AND ${CMAKE_PATCH_VERSION} LESS 7)
            OPTION(LIBCITYGML_MSVC_VERSIONED_DLL "Set to ON to build libcitygml with versioned dll names" ON)
            MARK_AS_ADVANCED(LIBCITYGML_MSVC_VERSIONED_DLL)
            OPTION(LIBCITYGML_MSVC_DEBUG_INCREMENTAL_LINK "Set to OFF to build libcitygml without incremental linking in debug (release is off by default)" ON)
            MARK_AS_ADVANCED(LIBCITYGML_MSVC_DEBUG_INCREMENTAL_LINK)
            IF(NOT LIBCITYGML_MSVC_DEBUG_INCREMENTAL_LINK)
                SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG "/debug /INCREMENTAL:NO")
                SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/debug /INCREMENTAL:NO")
                SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "/debug /INCREMENTAL:NO")
            ENDIF(NOT LIBCITYGML_MSVC_DEBUG_INCREMENTAL_LINK)
        # ENDIF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4 AND ${CMAKE_PATCH_VERSION} LESS 7)
    ENDIF(MSVC)
ENDIF(WIN32)

if(UNIX)
    if(APPLE)
        MESSAGE(STATUS "APPLE")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
    else(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")

        MESSAGE(STATUS "Setting UNIX compile flags: ${CMAKE_CXX_FLAGS}")
    endif(APPLE)
endif(UNIX)

IF(MSVC_IDE)
  # From CMake 2.8.2, new way to manage output generation dirs !
    # SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES PREFIX "../")
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin)
    SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin)

    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/lib)
    SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/lib)

    SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin)
    SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin)
ENDIF(MSVC_IDE)

SET(CMAKE_DEBUG_POSTFIX  "d")

# Dynamic vs Static Linking
OPTION(LIBCITYGML_DYNAMIC "Set to ON to build libcitygml for dynamic linking.  Use OFF for static." ON)
IF   (LIBCITYGML_DYNAMIC)
    SET(LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
ELSE (LIBCITYGML_DYNAMIC)
    SET(LIBCITYGML_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
ENDIF(LIBCITYGML_DYNAMIC)

SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})

SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})

# IF( DEBUG_BUILD )
  # SET( CMAKE_BUILD_TYPE "Debug" )
# ELSE( DEBUG_BUILD )
  # SET( CMAKE_BUILD_TYPE "Release" )
# ENDIF( DEBUG_BUILD )

OPTION(LIBCITYGML_STATIC_CRT "Set to ON to link libcitygml with the multithread CRT static library (instead of DLL)." ON)

# gdal library
OPTION(LIBCITYGML_USE_GDAL "Set to ON to build libcitygml with GDAL library so that it supports coordinates transformations." ON)


FIND_PACKAGE( Xerces REQUIRED )

IF( LIBCITYGML_USE_GDAL )
  FIND_PACKAGE( GDAL REQUIRED )
  ADD_DEFINITIONS( -DUSE_GDAL )
ELSE( LIBCITYGML_USE_GDAL )
  SET( GDAL_INCLUDE_DIR "" )
  SET( GDAL_LIBRARY "" )
ENDIF( LIBCITYGML_USE_GDAL )

FIND_PACKAGE( OpenGL REQUIRED )
#FIND_PACKAGE( GLU REQUIRED ) # deprecated, GLU is now found with FindOpenGL

IF( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )
ENDIF( COMMAND cmake_policy )

IF( MSVC AND LIBCITYGML_STATIC_CRT )
  #We statically link to reduce dependancies
  FOREACH( flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO )
    IF( ${flag_var} MATCHES "/MD" )
      string( REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}" )
    ENDIF( ${flag_var} MATCHES "/MD" )
    IF( ${flag_var} MATCHES "/MDd" )
      string( REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}" )
    ENDIF( ${flag_var} MATCHES "/MDd" )
  ENDFOREACH( flag_var )
ENDIF( MSVC AND LIBCITYGML_STATIC_CRT )

# core
ADD_SUBDIRECTORY( sources )

# osg plugin
OPTION(LIBCITYGML_OSGPLUGIN "Set to ON to build libcitygml osgplugin programs." OFF)
IF   (LIBCITYGML_OSGPLUGIN)
  ADD_SUBDIRECTORY( osgplugin )
ENDIF(LIBCITYGML_OSGPLUGIN)

# test
OPTION(LIBCITYGML_TESTS "Set to ON to build libcitygml tests programs." ON)
IF   (LIBCITYGML_TESTS)
  ADD_SUBDIRECTORY( test )
ENDIF(LIBCITYGML_TESTS)


#-----------------------------------------------------------------------------
### uninstall target
#-----------------------------------------------------------------------------
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Create debian package
option(BUILD_DEBIAN_PACKAGE  "Creates a debian package" ON)
if(BUILD_DEBIAN_PACKAGE)
    # set project description for packaging
    set(META_PROJECT_NAME        "libcitygml")
    set(META_PROJECT_SUMMARY     "Open source C++ library for parsing CityGML files")
    set(META_PROJECT_DESCRIPTION "libcitygml is a small and easy to use open source C++ library for parsing
                                  CityGML files in such a way that data can be easily exploited by 3D
                                  rendering applications (geometry data are tesselated and optimized for
                                  rendering during parsing). For instance, it can be used to develop readers
                                  of CityGML files in many 3D based applications (OpenGL, OpenSceneGraph, ...)
                                  Most metadata are not lost, they are available through a per-node hashmap.

                                  CityGML (City Geography Markup Language) is an XML-based schema for the
                                  modelling and exchange of georeferenced 3D city and landscape models that
                                  is quickly being adopted on an international level.")
    set(META_AUTHOR_ORGANIZATION "3D Content Logistics")
    set(META_AUTHOR_DOMAIN       "https://github.com/jklimke/libcitygml.git")
    set(META_AUTHOR_MAINTAINER   "Marcel Pursche <marcel.pursche@3dcontentlogistics.com>")

    # Package information
    string(TOLOWER ${META_PROJECT_NAME} package_name)          # Package name
    set(CPACK_PACKAGE_NAME                  "${META_PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR                "${META_AUTHOR_ORGANIZATION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "${META_PROJECT_SUMMARY}")
    set(CPACK_PACKAGE_DESCRIPTION           "${META_PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VERSION               "${META_VERSION}")
    set(CPACK_PACKAGE_VERSION_MAJOR         "${META_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR         "${META_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH         "${META_VERSION_PATCH}")
    set(CPACK_RESOURCE_FILE_LICENSE         "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_PACKAGE_RELOCATABLE           OFF)

    # Debian package information
    set(CPACK_DEBIAN_PACKAGE_NAME           "${package_name}")
    set(CPACK_DEBIAN_PACKAGE_VERSION        "${CPACK_PACKAGE_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER     "${META_AUTHOR_MAINTAINER}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION    "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    set(CPACK_DEBIAN_PACKAGE_SECTION        "Development")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY       "optional")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS        "libgdal1h (>= 1.10.1), libxerces-c3.1 (>= 3.1.1-5)")
    set(CPACK_DEBIAN_PACKAGE_CONFLICTS      "libcitygml0, libcitygml0-dev")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE       "${META_AUTHOR_DOMAIN}")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA  "${CMAKE_CURRENT_SOURCE_DIR}/debian_scripts/postinst;")
    
    set(CPACK_GENERATOR "DEB")

    include(CPack)
endif()
