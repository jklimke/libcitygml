# libcitygml TODO List

* Rename Appearance in SurfaceData (an Appearance is actually the objects that defines a Theme)
* Allow two textures and materials per AppearanceTarget (one backfacing and one front facing)
* Allow polygon sharing (search for gml:CompositeSurface in Berlin_komplett.citygml)

# Optimization
* In the finish step check if the geometry hierachy can be folded. It should be possible to merge all geometries of the same lod level together... however thats not so simple with shared geometries (ImplicitGeomentry) 