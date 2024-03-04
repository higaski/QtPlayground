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

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  get_qt(
    VERSION
    6.6.1
    MODULES
    qtbase
    CMAKE_OPTIONS
    -DCMAKE_BUILD_TYPE=Release
    -DINPUT_optimize_size=ON
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${DCMAKE_INTERPROCEDURAL_OPTIMIZATION}
    # Disable features that are not needed
    -DFEATURE_accessibility=OFF
    -DFEATURE_dbus=OFF
    -DFEATURE_cssparser=OFF
    -DFEATURE_glib=OFF
    -DFEATURE_fontconfig=OFF
    -DFEATURE_harfbuzz=OFF
    -DFEATURE_ico=OFF
    -DFEATURE_libjpeg=OFF
    -DFEATURE_libpng=OFF
    -DFEATURE_opengl=OFF
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
    -DFEATURE_xml=OFF
    # Enable freetype to get some sort of font support
    -DFEATURE_freetype=ON
    -DFEATURE_system_freetype=OFF
    # Default to xcb and disable all other platform plugins
    -DQT_QPA_DEFAULT_PLATFORM=xcb
    -DFEATURE_linuxfb=OFF
    -DFEATURE_vkkhrdisplay=OFF
    -DFEATURE_vnc=OFF
    -DFEATURE_zstd=OFF
    # No FEATURE flags yet
    -DINPUT_libjpeg=no
    -DINPUT_libmd4c=no)
elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
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
    -DFEATURE_accessibility=OFF
    -DFEATURE_dbus=OFF
    -DFEATURE_cssparser=OFF
    -DFEATURE_glib=OFF
    -DFEATURE_fontconfig=OFF
    -DFEATURE_harfbuzz=OFF
    -DFEATURE_ico=OFF
    -DFEATURE_libjpeg=OFF
    -DFEATURE_libpng=OFF
    -DFEATURE_opengl=OFF
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
    # No FEATURE flags yet
    -DINPUT_libjpeg=no
    -DINPUT_libmd4c=no)
endif()

find_qt(REQUIRED COMPONENTS Core Widgets)

target_link_libraries(QtPlayground PRIVATE Qt::Core Qt::Widgets)
