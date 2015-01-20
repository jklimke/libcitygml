set(target citygmlosgplugin)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
find_package( OpenSceneGraph REQUIRED osgDB osgText)

include_directories(
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/sources/include
)

set( sources ReaderWriterCityGML.cpp )

add_library(${target} SHARED ${sources})

target_link_libraries(${target} ${OPENSCENEGRAPH_LIBRARIES})

INSTALL(
    TARGETS ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
