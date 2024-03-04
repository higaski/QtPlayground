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

if(CMAKE_TOOLCHAIN_FILE)
  set(QT_TOOLCHAIN_FILE_OPTION -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
else()
  set(QT_TOOLCHAIN_FILE_OPTION)
endif()

get_qt(
  VERSION
  6.2.4
  MODULES
  qtbase
  CMAKE_OPTIONS
  ${QT_TOOLCHAIN_FILE_OPTION}
  -DCMAKE_BUILD_TYPE=Release # -release
  -DINPUT_optimize_size=ON # -optimize-size
  -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} #
  -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${DCMAKE_INTERPROCEDURAL_OPTIMIZATION} # -ltcg
  -DFEATURE_accessibility=OFF
  -DFEATURE_dbus=OFF
  -DFEATURE_cssparser=OFF
  -DFEATURE_glib=OFF
  -DFEATURE_fontconfig=OFF
  -DFEATURE_harfbuzz=OFF
  -DFEATURE_ico=OFF
  -DFEATURE_libjpeg=OFF
  -DFEATURE_libpng=ON
  -DFEATURE_sql=OFF
  -DFEATURE_system_doubleconversion=OFF
  -DFEATURE_system_harfbuzz=OFF
  -DFEATURE_system_pcre2=OFF
  -DFEATURE_system_zlib=OFF
  -DFEATURE_testlib=OFF
  -DFEATURE_texthtmlparser=OFF
  -DFEATURE_textmarkdownreader=OFF
  -DFEATURE_textmarkdownwriter=OFF
  -DFEATURE_textodfwriter=OFF
  -DFEATURE_vulkan=OFF
  # -DFEATURE_xml=OFF Not working on 6.2.4?
  #
  # Enable freetype to get some sort of font support
  -DFEATURE_freetype=ON
  -DFEATURE_system_freetype=OFF
  # Enable OpenGL
  -DFEATURE_opengl=ON
  #
  -DINPUT_libjpeg=no
  -DINPUT_libmd4c=no)

find_qt(REQUIRED COMPONENTS Core Widgets)
