# Locate iconv
# This module defines
# ICONV_LIBRARY
# ICONV_FOUND, if false, do not try to link to ICONV 
# ICONV_INCLUDE_DIR, where to find the headers
#
# $ICONV_DIR is an environment variable that would
# correspond to the ./configure --prefix=$ICONV_DIR


FIND_PATH(ICONV_INCLUDE_DIR iconv.h
        ${ICONV_DIR}/include
    $ENV{ICONV_DIR}/include
    $ENV{ICONV_DIR}
    $ENV{OSGDIR}/include
    $ENV{OSGDIR}
    $ENV{OSG_ROOT}/include
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include
    /usr/include
    /sw/include # Fink
    /opt/local/include # DarwinPorts
    /opt/csw/include # Blastwave
    /opt/include
    /usr/freeware/include
)

FIND_LIBRARY(ICONV_LIBRARY 
    NAMES ICONV
    PATHS
    ${ICONV_DIR}/lib
    $ENV{ICONV_DIR}/lib
    $ENV{ICONV_DIR}
    $ENV{OSGDIR}/lib
    $ENV{OSGDIR}
    $ENV{OSG_ROOT}/lib
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/lib
    /usr/lib
    /sw/lib
    /opt/local/lib
    /opt/csw/lib
    /opt/lib
    /usr/freeware/lib64
)

SET(ICONV_FOUND "NO")
IF(ICONV_LIBRARY AND ICONV_INCLUDE_DIR)
    SET(ICONV_FOUND "YES")
ENDIF(ICONV_LIBRARY AND ICONV_INCLUDE_DIR)


