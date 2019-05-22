INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BAR bar)

FIND_PATH(
    BAR_INCLUDE_DIRS
    NAMES bar/api.h
    HINTS $ENV{BAR_DIR}/include
        ${PC_BAR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BAR_LIBRARIES
    NAMES gnuradio-bar
    HINTS $ENV{BAR_DIR}/lib
        ${PC_BAR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BAR DEFAULT_MSG BAR_LIBRARIES BAR_INCLUDE_DIRS)
MARK_AS_ADVANCED(BAR_LIBRARIES BAR_INCLUDE_DIRS)

