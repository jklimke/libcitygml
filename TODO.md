# libcitygml TODO List

# Correctness
* Fix texture coordinates when tesselation removes or adds vertices
* Check if Appearance assignment is correct for shared polygons
* Check if the normals calculated for tesselation is always correct even if the vertices are in a spherical coordinate system
  ... if not one should think about removing the tesselation from libcitygml 
* Check if OrientableSurfaces are supported properly... may be the vertices must be reverted if the orientation is '-'
* Check if non implicit geoemtries can be shared
* Ensure that polygons that are children of shared geometries are not also children of non shared geometries (otherwise a coordinate transformation might be applied on the vertices which is not allowed for shared geometries)

# Features
* Enable CityObject filering by type mask (ParserParams)
* Implement georefereced texture parsing

# Optimization
* In the finish step check if the geometry hierachy can be folded. It should be possible to merge all geometries of the same lod level together... however thats not so simple with shared geometries (ImplicitGeomentry) 
* Remove empty objects

# Refactoring
* Change the NodeTypes so that typeID is a constant expression -> Use switch-case structures instead of if-then-else in the element parsers
* Rename Appearance in SurfaceData (an Appearance is actually the objects that defines a Theme)