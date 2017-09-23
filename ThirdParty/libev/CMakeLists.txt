project ( libevd )
cmake_minimum_required ( VERSION 2.8 )

include(ExternalProject)
ExternalProject_Add(
  libevd
  URL http://dist.schmorp.de/libev/libev-4.24.tar.gz
  CONFIGURE_COMMAND ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}
  BUILD_IN_SOURCE 1
)