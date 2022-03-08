prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: citygml
Description: Read and Write CityGML files
Requires: libxml-2.0 glu
Version: ${META_VERSION}
Libs: -L${LIB_INSTALL_DIR} -lcitygml${LIBCITYGML_POSTFIX}
Cflags: -I${INCLUDE_INSTALL_DIR}
