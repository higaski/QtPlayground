cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY https://github.com/ZIMO-Elektronik/CMakeModules
  GIT_TAG get_qt
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  QtPlayground
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(DCMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt comes with zlib, use it instead of zstd
set(CMAKE_AUTORCC_OPTIONS "-compress-algo;zlib" "--compress;9")

file(GLOB_RECURSE SRC src/*.c src/*.cpp)
add_executable(QtPlayground ${SRC})

# Qt6 requires C++17
target_compile_features(QtPlayground PUBLIC cxx_std_17)

get_qt(
  VERSION
  6.2.4
  MODULES
  qtbase
  CMAKE_OPTIONS
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  -DCMAKE_BUILD_TYPE=Release # -release
  -DINPUT_optimize_size=ON # -optimize-size
  -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} #
  -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${DCMAKE_INTERPROCEDURAL_OPTIMIZATION} # -ltcg
  -DFEATURE_dbus=OFF # -no-dbus
  -DFEATURE_system_doubleconversion=OFF # -doubleconversion
  -DFEATURE_glib=OFF # -glib
  -DFEATURE_system_pcre2=OFF # -pcre
  -DFEATURE_system_zlib=OFF # -zlib
  -DFEATURE_system_freetype=OFF # -freetype
  -DFEATURE_system_harfbuzz=OFF # -harfbuzz
  -DFEATURE_libpng=ON # -libpng
  -DFEATURE_libjpeg=OFF # -libjpeg
  -DFEATURE_sql=OFF #
  #
  -DFEATURE_zstd=OFF #
  -DINPUT_libjpeg=no # Workaround, DFEATURE_libjpeg not working
)
