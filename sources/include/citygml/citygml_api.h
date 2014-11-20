#define LIBCITYGML_VERSION_MAJOR 0
#define LIBCITYGML_VERSION_MINOR 1
#define LIBCITYGML_VERSION_REVISION 5

#define LIBCITYGML_VERSIONSTR "0.1.5"

#if defined( _MSC_VER ) && defined( LIBCITYGML_DYNAMIC )
#	ifdef LIBCITYGML_BUILD
#		define LIBCITYGML_EXPORT __declspec( dllexport )
#	else
#		define LIBCITYGML_EXPORT __declspec( dllimport )
#	endif
#else
#	define LIBCITYGML_EXPORT
#endif
