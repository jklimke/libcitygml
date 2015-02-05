#define LIBCITYGML_VERSION_MAJOR 1
#define LIBCITYGML_VERSION_MINOR 1
#define LIBCITYGML_VERSION_REVISION 0

#define LIBCITYGML_CREATE_VERSION_STRING(major,minor,revision) "#major.#minor.#revision"
#define LIBCITYGML_VERSIONSTR LIBCITYGML_CREATE_VERSION_STRING(LIBCITYGML_VERSION_MAJOR, LIBCITYGML_VERSION_MINOR, LIBCITYGML_VERSION_REVISION)

#define LIBCITYGML_VERSION_LESS(major, minor, revision) major > LIBCITYGML_VERSION_MAJOR || \
                                                        (major == LIBCITYGML_VERSION_MAJOR && (minor > LIBCITYGML_VERSION_MINOR || \
                                                        (minor == LIBCITYGML_VERSION_MINOR && revision > LIBCITYGML_VERSION_REVISION)))

#if defined( _MSC_VER ) && defined( LIBCITYGML_DYNAMIC )
#	ifdef LIBCITYGML_BUILD
#		define LIBCITYGML_EXPORT __declspec( dllexport )
#	else
#		define LIBCITYGML_EXPORT __declspec( dllimport )
#	endif
#else
#	define LIBCITYGML_EXPORT
#endif
