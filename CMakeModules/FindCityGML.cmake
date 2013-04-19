# Locate libcitygml
# This module defines
# CITYGML_LIBRARY
# CITYGML_LIBRARY_DEBUG 
# CITYGML_LIBRARIES, choses the correct debug or optimized library for linking, add this as Target Link library in your project
# CITYGML_FOUND, if false, do not try to link to CITYGML 
# CITYGML_INCLUDE_DIR, where to find the headers
#
# $CITYGML_DIR is an environment variable that would
# correspond to the ./configure --prefix=$CITYGML_DIR

OPTION(CITYGML_DYNAMIC "Set to ON if libcitygml was built using dynamic linking.  Use OFF for static." OFF)

OPTION(CITYGML_USE_XERCESC "Set to ON to build libcitygml with Xerces-c library." ON)
OPTION(CITYGML_USE_LIBXML2 "Set to ON to build libcitygml with LibXml2 library." OFF)

IF( CITYGML_DYNAMIC )
	ADD_DEFINITIONS( -DLIBCITYGML_DYNAMIC )
ENDIF( CITYGML_DYNAMIC )

IF ( CITYGML_USE_XERCESC AND CITYGML_USE_LIBXML2 )
	MESSAGE("Error: You cannot build the library with Xerces-c AND LibXml2! Xerces library will be used by default.")
	SET( CITYGML_USE_LIBXML2 OFF CACHE BOOL "Set to ON to build libcitygml with LibXml2 library." FORCE)
ENDIF( CITYGML_USE_XERCESC AND CITYGML_USE_LIBXML2 )

IF( CITYGML_USE_XERCESC )
	FIND_PACKAGE( Xerces REQUIRED )
	ADD_DEFINITIONS( -DUSE_XERCESC )
	SET( LIBXML2_INCLUDE_DIR "" )
	# SET( LIBXML2_LIBRARIES "" )
	SET( LIBXML2_LIBRARY "" )
	SET( LIBXML2_LIBRARY_DEBUG "" )
ENDIF( CITYGML_USE_XERCESC )

IF( CITYGML_USE_LIBXML2 )
	FIND_PACKAGE( LibXml2 REQUIRED )
	ADD_DEFINITIONS( -DUSE_LIBXML2 )
	ADD_DEFINITIONS( ${LIBXML2_DEFINITIONS} )
	SET( XERCESC_INCLUDE "" )
	SET( XERCESC_LIBRARY "" )
	SET( XERCESC_LIBRARY_DEBUG "" )
ENDIF( CITYGML_USE_LIBXML2 )

FIND_PATH( CITYGML_INCLUDE_DIR citygml.h
	./include
	../include
	$ENV{CITYGML_DIR}/include
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/include
	/usr/include
	/sw/include # Fink
	/opt/local/include # DarwinPorts
	/opt/csw/include # Blastwave
	/opt/include
	/usr/freeware/include
)

FIND_LIBRARY( CITYGML_LIBRARY 
	NAMES citygml
	PATHS
	./lib
	../lib
	$ENV{CITYGML_DIR}/lib
	$ENV{CITYGML_DIR}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	/usr/freeware/lib64
)

FIND_LIBRARY( CITYGML_LIBRARY_DEBUG
	NAMES citygmld
	PATHS
	./lib
	../lib
	$ENV{CITYGML_DIR}/lib
	$ENV{CITYGML_DIR}
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	/usr/freeware/lib64
)

SET( CITYGML_FOUND FALSE )

IF(CITYGML_LIBRARY AND CITYGML_INCLUDE_DIR)
	SET(CITYGML_FOUND TRUE)
	IF(NOT CITYGML_LIBRARY_DEBUG)
		MESSAGE("-- Warning Debug LibCityGML not found, using: ${CITYGML_LIBRARY}")
		SET(CITYGML_LIBRARY_DEBUG "${CITYGML_LIBRARY}" CACHE FILEPATH "Path to a library." FORCE)
	ENDIF(NOT CITYGML_LIBRARY_DEBUG)
ENDIF(CITYGML_LIBRARY AND CITYGML_INCLUDE_DIR)

SET(CITYGML_LIBRARIES optimized ${CITYGML_LIBRARY} debug ${CITYGML_LIBRARY_DEBUG})

