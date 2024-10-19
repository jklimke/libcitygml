#include <citygml/citygml.h>
#include <citygml/citygml_api.h>
#include <citygml/citymodel.h>
#include <citygml/geometry.h>
#include <citygml/implictgeometry.h>
#ifdef LIBCITYGML_USE_OPENGL
	#include <citygml/tesselator.h>
#endif

#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <time.h>


namespace {
	constexpr bool USE_STREAM = false;
	constexpr bool LOG = false;

void printIndent(unsigned int indent) {
	for ( unsigned int i = 0; i < indent; i++ ) std::cout << " ";
}

void printGeometry( const citygml::Geometry& geometry, unsigned int indent ) {
	printIndent(indent);
	std::cout << "Geometry for LOD" << geometry.getLOD() << ", type: " << geometry.getTypeAsString() << "\n";

	if(geometry.getLineStringCount() > 0) {
		printIndent(indent+1);
		std::cout << "Linestrings:" << geometry.getLineStringCount() << "\n";
	}

	if(geometry.getPolygonsCount() > 0) {
		printIndent(indent+1);
		std::cout << "Polygons:" << geometry.getPolygonsCount() << "\n";
	}

	if(geometry.getGeometriesCount() > 0) {
		printIndent(indent+1);
		std::cout << "SubGeometries:" << "\n";
		for( unsigned int i = 0; i < geometry.getGeometriesCount(); i++ ) {
			printGeometry(geometry.getGeometry(i), indent+1);
		}
	}
}


void printImplicitGeometry( const citygml::ImplicitGeometry& implicitGeometry, unsigned int indent ) {
	printIndent(indent);
	std::cout << "Reference point " << implicitGeometry.getReferencePoint() << "\n";
	for ( unsigned int i = 0; i < implicitGeometry.getGeometriesCount(); i++ ) {
		printGeometry( implicitGeometry.getGeometry(i), indent+1 );
	}
}

void analyzeObject( const citygml::CityObject& object, unsigned int indent ) {
	printIndent(indent);
	std::cout << "Object " << object.getTypeAsString() << ": " << object.getId() << "\n";

	for ( unsigned int i = 0; i < object.getGeometriesCount(); i++ ) {
		printGeometry( object.getGeometry(i), indent+1 );
	}

	for ( unsigned int i = 0; i < object.getImplicitGeometryCount(); i++ ) {
		printImplicitGeometry( object.getImplicitGeometry(i), indent+1 );
	}

	for ( unsigned int i = 0; i < object.getChildCityObjectsCount(); i++ ) {
		analyzeObject( object.getChildCityObject(i), indent+1 );
	}
}

void readFile(char const* fileName) {
	std::cout << "Parsing CityGML file " << fileName << " using libcitygml v." << LIBCITYGML_VERSIONSTR << "...\n";

	time_t start;
	time( &start );

	std::unique_ptr<TesselatorBase> tesselator;
#ifdef LIBCITYGML_USE_OPENGL
	tesselator.reset(new Tesselator(nullptr));
#endif

	citygml::ParserParams params;
	std::shared_ptr<const citygml::CityModel> city;
	try{
		if constexpr (USE_STREAM) {
			std::ifstream file;
			file.open( fileName, std::ifstream::in );
			std::shared_ptr<const citygml::CityModel> city = citygml::load( file, params, std::move(tesselator) );
		} else {
			city = citygml::load( fileName, params, std::move(tesselator) );
		}
	} catch(const std::runtime_error&){
		FAIL();
	}

	time_t end;
	time( &end );

	std::cout << "Done in " << difftime( end, start ) << " seconds.\n";

	if (!city) {
		FAIL();
	}

	if constexpr (LOG) {
		std::cout << "Analyzing the city objects...\n";

		for (auto const& cityObject : city->getRootCityObjects()) {
			std::cout << " Found " << cityObject->getTypeAsString() << "...\n";
			std::cout << "  + found object id " << cityObject->getId() << "\n";
			if (cityObject->getChildCityObjectsCount() > 0) {
				std::cout << "  + with " << cityObject->getChildCityObjectsCount() << " children\n";
			}
			std::cout << "  + with " << cityObject->getGeometriesCount() << " geometr" << ( ( cityObject->getGeometriesCount() > 1 ) ? "ies\n" : "y\n" );
			std::cout << "  + with " << cityObject->getImplicitGeometryCount() << " implicit geometr" << ( ( cityObject->getImplicitGeometryCount() > 1 ) ? "ies\n" : "y\n" );
		}
	}

	if constexpr (LOG) {
		std::cout << std::endl << "Objects hierarchy:\n";
		const citygml::ConstCityObjects& roots = city->getRootCityObjects();
		for ( unsigned int i = 0; i < roots.size(); i++ ) analyzeObject( *(roots[ i ]), 2 );
	}

	std::cout << "Done.\n";
}
} // anonymous namespace

TEST(FileReadTests, berlin_open_data_sample_data) {
	readFile("../../data/berlin_open_data_sample_data.citygml");
}

TEST(FileReadTests, b1_lod2_s) {
	readFile("../../data/b1_lod2_s.gml");
}

TEST(FileRedTests, FZK_Haus_LoD0_KIT_IAI_KHH_B36_V1) {
	readFile("../../data/FZK-Haus-LoD0-KIT-IAI-KHH-B36-V1.gml");
}
