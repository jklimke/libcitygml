set(target osgdb_citygml)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
find_package( OpenSceneGraph REQUIRED osgDB osgViewer osgGA osgUtil osgText)

include_directories(
  ${OPENSCENEGRAPH_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/sources/include
)

set( sources ReaderWriterCityGML.cpp )

add_library(${target} SHARED ${sources})
set_target_properties(${target} PROPERTIES PREFIX "")

target_link_libraries(${target} ${OPENSCENEGRAPH_LIBRARIES} citygml)

set(OSG_PLUGINS "osgPlugins-${OPENSCENEGRAPH_VERSION}")
set(LIBCITYGML_OSG_PLUGIN_INSTALL_DIR "" CACHE PATH "The directory in which the plugin will be installed. (osgPlugins-<version> is appended to this path)")

set(PLUGIN_INSTALL_PATH lib/${OSG_PLUGINS})
if (LIBCITYGML_OSG_PLUGIN_INSTALL_DIR)
  set(PLUGIN_INSTALL_PATH ${LIBCITYGML_OSG_PLUGIN_INSTALL_DIR}/${OSG_PLUGINS})
endif()

add_definitions(-DPLUGIN_BIN_DIR=\"${CMAKE_BINARY_DIR}/lib\")
add_executable(citygmlOsgViewer CitygmlOsgViewer.cpp)
target_link_libraries(citygmlOsgViewer ${OPENSCENEGRAPH_LIBRARIES} citygml)

INSTALL(
    TARGETS ${target} citygmlOsgViewer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${PLUGIN_INSTALL_PATH}
    ARCHIVE DESTINATION ${PLUGIN_INSTALL_PATH}
)
