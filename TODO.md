# libcitygml TODO List

# Correctness
* Check if Appearance assigment is correct for shared polygons

# Features
* Enable CityObject filering by type mask (ParserParams)
* Implement georefereced texture parsing
* Implement transformation to other spatial reference system after parsing is finished

# Optimization
* In the finish step check if the geometry hierachy can be folded. It should be possible to merge all geometries of the same lod level together... however thats not so simple with shared geometries (ImplicitGeomentry) 

# Refactoring
* Change the NodeTypes so that typeID is a constant expression -> Use switch-case structures instead of if-then-else in the element parsers
* Rename Appearance in SurfaceData (an Appearance is actually the objects that defines a Theme)