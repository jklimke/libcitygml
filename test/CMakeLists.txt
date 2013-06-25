IF(WIN32)
    SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
ENDIF()

IF( LIBCITYGML_USE_XERCESC )
	FIND_PACKAGE( Xerces REQUIRED )
	ADD_DEFINITIONS( -DUSE_XERCESC )
	SET( LIBXML2_INCLUDE_DIR "" )
	SET( LIBXML2_LIBRARIES "" )
ENDIF( LIBCITYGML_USE_XERCESC )

IF( LIBCITYGML_USE_LIBXML2 )
	FIND_PACKAGE( LibXml2 REQUIRED )
	ADD_DEFINITIONS( -DUSE_LIBXML2 )
	ADD_DEFINITIONS( ${LIBXML2_DEFINITIONS} )
	SET( XERCESC_INCLUDE "" )
	SET( XERCESC_LIBRARY "" )
ENDIF( LIBCITYGML_USE_LIBXML2 )

FIND_PACKAGE( OpenGL REQUIRED )
#FIND_PACKAGE( GLU REQUIRED ) # deprecated, GLU is now found with FindOpenGL

IF( COMMAND cmake_policy )
	cmake_policy( SET CMP0003 NEW )
ENDIF( COMMAND cmake_policy )

IF( LIBCITYGML_STATIC )
	ADD_DEFINITIONS( -DLIBCITYGML_STATIC )
ENDIF( LIBCITYGML_STATIC )

IF( LIBCITYGML_DYNAMIC )
	ADD_DEFINITIONS( -DLIBCITYGML_DYNAMIC )
ENDIF( LIBCITYGML_DYNAMIC )

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

# INCLUDE_DIRECTORIES( ${CITYGML_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ../include )

SET( PRG_SRCS citygmltest.cpp )

ADD_EXECUTABLE( citygmltest ${PRG_SRCS} )

# IF( MSVC_IDE )
	# SET_TARGET_PROPERTIES( citygmltest PROPERTIES PREFIX "../" )
# ENDIF( MSVC_IDE )

TARGET_LINK_LIBRARIES( citygmltest citygml ${XERCESC_LIBRARY} ${LIBXML2_LIBRARIES} ${OPENGL_LIBRARIES} )