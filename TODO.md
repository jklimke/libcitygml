# libcitygml TODO List

# Correctness
* Check if the normals calculated for tesselation is always correct even if the vertices are in a spherical coordinate system
  ... if not one should think about removing the tesselation from libcitygml 
* Check if OrientableSurfaces are supported properly... may be the vertices must be reverted if the orientation is '-'
* Check if Appearance assignment is correct for shared polygons
* Check if non implicit geoemtries can be shared
* Ensure that polygons that are children of shared geometries are not also children of non shared geometries (otherwise a coordinate transformation might be applied on the vertices which is not allowed for shared geometries)
* THe namespace of the different modules may differ from the recommended one... make a namespace mapping based on the uri

# Completness
* Implement parsing of CityObject <generalizesTo> member (contains a cityobject or references one that is the generalization of the current one) => requires cityobject sharing
* Implement parsing of CityObject <externalReference> member
* Implement complete address parsing: Currently the address attributes (street, postalcode, etc.) are stored as indivdual address attributes. However a address can be assigned zero or more 2D or 3D point geometries (one gml:MultiPoint geometry) locating the entrance. This requires parsing of the <Address> as an individual element
* Implement parsing of ImplicitGeometry <libraryObject> member
* Implement GeoreferencedTexture parsing
* Implement appearence <TexCoordGen> support
* Implement <grp:geometry> support (Darunter kann eine beliebige GML Geometrie hängen)
* Implement full support for <grp:groupMember> and <grp:parent> (requires city object sharing, currently only inline definitions are supported)
* Implement lod0 parsing
* Implement Relief/Terrain Model (Namespace DEM) parsing. More precisely implement ReliefComponentPropertyType parsing.

# Refactoring
* Change the NodeTypes so that typeID is a constant expression -> Use switch-case structures instead of if-then-else in the element parsers
* Rename Appearance in SurfaceData (an Appearance is actually the objects that defines a Theme)

# Features
* Enable CityObject filering by type mask (ParserParams)


# Optimization
* Remove empty objects
* In the finish step check if the geometry hierachy can be folded. It should be possible to merge all geometries of the same lod level together... however thats not so simple with shared geometries (ImplicitGeomentry) 
