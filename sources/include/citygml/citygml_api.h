#define LIBCITYGML_VERSION_MAJOR 2
#define LIBCITYGML_VERSION_MINOR 4
#define LIBCITYGML_VERSION_REVISION 1

#define LIBCITYGML_CREATE_VERSION_STRING_(x, y, z) #x"."#y"."#z
#define LIBCITYGML_CREATE_VERSION_STRING(major,minor,revision) LIBCITYGML_CREATE_VERSION_STRING_(major, minor, revision)
#define LIBCITYGML_VERSIONSTR LIBCITYGML_CREATE_VERSION_STRING(LIBCITYGML_VERSION_MAJOR, LIBCITYGML_VERSION_MINOR, LIBCITYGML_VERSION_REVISION)

#define LIBCITYGML_VERSION_LESS(major, minor, revision) major > LIBCITYGML_VERSION_MAJOR || \
                                                        (major == LIBCITYGML_VERSION_MAJOR && (minor > LIBCITYGML_VERSION_MINOR || \
                                                        (minor == LIBCITYGML_VERSION_MINOR && revision > LIBCITYGML_VERSION_REVISION)))

#include <citygml/citygml_export.h>
