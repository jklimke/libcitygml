prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${CMAKE_INSTALL_BINDIR}
libdir=${CMAKE_INSTALL_LIBDIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: citygml
Description: Read and Write CityGML files
Requires: ${PKG_CONFIG_REQUIRES}
Version: ${META_VERSION}
Libs: -L${CMAKE_INSTALL_LIBDIR} -lcitygml${LIBCITYGML_POSTFIX}
Cflags: -I${INCLUDE_INSTALL_DIR}
