cmake_minimum_required(VERSION 2.8)
PROJECT( DPL )

# version (e.g. for packaging)
set(DPL_MAJOR_VERSION 1)
set(DPL_MINOR_VERSION 2)
set(DPL_PATCH_VERSION 0)
set(DPL_VERSION ${DPL_MAJOR_VERSION}.${DPL_MINOR_VERSION}.${DPL_PATCH_VERSION})

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")

# set(CMAKE_CXX_FLAGS "--pedantic-errors -Wall -Wextra -Werror -ftabstop=4 -march=native -std=gnu++14 -fshow-column -ftabstop=4 -ftrapv -pipe ${FABIO_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "--pedantic-errors -Wall -Wextra -ftabstop=4 -march=native -std=gnu++14 -fshow-column -ftabstop=4 -ftrapv -pipe ${FABIO_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")


include_directories(src/include)

#add_library(DPL SHARED
  # src/utils/heap.cpp
  #)

set(DPL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(DPL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# support for pkg-config
set(PKG_NAME "DPL")
set(PKG_DESC "DPL Planning Library")
set(PKG_DPL_LIBS "-lDPL")
set(pkg_conf_file_in "${CMAKE_CURRENT_SOURCE_DIR}/DPL.pc.in")
set(pkg_conf_file "${PROJECT_BINARY_DIR}/InstallFiles/DPL.pc")
configure_file("${pkg_conf_file_in}" "${pkg_conf_file}" @ONLY)
install(FILES ${pkg_conf_file} DESTINATION lib/pkgconfig/ COMPONENT pkgconfig)

# support for cmake-config:
configure_file(DPL-config.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/DPL-config.cmake" @ONLY)
configure_file(DPL-config-version.cmake.in
  "${PROJECT_BINARY_DIR}/InstallFiles/DPL-config-version.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}/InstallFiles/DPL-config.cmake"
  "${PROJECT_BINARY_DIR}/InstallFiles/DPL-config-version.cmake"
DESTINATION share/DPL/)

install(DIRECTORY src/include/DPL DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
# install(TARGETS DPL
#         LIBRARY DESTINATION lib)

add_executable(benchmark src/benchmark.cpp)
# target_link_libraries(benchmark DPL)

# add_subdirectory(src/planners/AStar)
