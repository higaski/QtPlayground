cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY https://github.com/ZIMO-Elektronik/CMakeModules
  GIT_TAG qt
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  QtPlayground
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES C CXX)

set(QT_VERSION 6.4.3)
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

if(CMAKE_SYSTEM_NAME STREQUAL Linux AND CMAKE_BUILD_TYPE STREQUAL Release)
  build_qt(
    VERSION
    ${QT_VERSION}
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
    -DFEATURE_gif=OFF
    -DFEATURE_ico=OFF
    -DFEATURE_libjpeg=OFF
    -DFEATURE_libpng=OFF
    -DFEATURE_opengl=OFF
    -DFEATURE_png=OFF
    -DFEATURE_printsupport=OFF
    -DFEATURE_sql=OFF
    -DFEATURE_system_doubleconversion=OFF
    -DFEATURE_system_freetype=OFF
    -DFEATURE_system_harfbuzz=OFF
    -DFEATURE_system_libb2=OFF
    -DFEATURE_system_pcre2=OFF
    -DFEATURE_system_zlib=OFF
    -DFEATURE_testlib=OFF
    -DFEATURE_texthtmlparser=OFF
    -DFEATURE_textmarkdownreader=OFF
    -DFEATURE_textmarkdownwriter=OFF
    -DFEATURE_textodfwriter=OFF
    -DFEATURE_vulkan=OFF
    -DFEATURE_xml=OFF
    # Enable freetype and harfbuzz to get some sort of font support
    -DFEATURE_freetype=ON
    -DFEATURE_harfbuzz=ON
    # Default to xcb and disable all other platform plugins
    -DQT_QPA_DEFAULT_PLATFORM=xcb
    -DFEATURE_linuxfb=OFF
    -DFEATURE_system_xcb_xinput=OFF
    -DFEATURE_vkkhrdisplay=OFF
    -DFEATURE_vnc=OFF
    -DFEATURE_zstd=OFF
    # No FEATURE flags yet
    -DINPUT_libjpeg=no
    -DINPUT_libmd4c=no)
elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
  execute_process(
    COMMAND
      ${Python3_EXECUTABLE} -m aqt install-qt #
      --outputdir ${CMAKE_BINARY_DIR}/qt6-host-build #
      linux desktop ${QT_VERSION} gcc_64 #
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR} #
                      COMMAND_ERROR_IS_FATAL ANY)

  build_qt(
    VERSION
    ${QT_VERSION}
    MODULES
    qtbase
    CMAKE_OPTIONS
    -DQT_HOST_PATH=${CMAKE_BINARY_DIR}/qt6-host-build/${QT_VERSION}/gcc_64
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
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
    -DFEATURE_gif=OFF
    -DFEATURE_ico=OFF
    -DFEATURE_libjpeg=OFF
    -DFEATURE_libpng=OFF
    -DFEATURE_png=OFF
    -DFEATURE_printsupport=OFF
    -DFEATURE_sql=OFF
    -DFEATURE_system_doubleconversion=OFF
    -DFEATURE_system_freetype=OFF
    -DFEATURE_system_harfbuzz=OFF
    -DFEATURE_system_libb2=OFF
    -DFEATURE_system_pcre2=OFF
    -DFEATURE_system_zlib=OFF
    -DFEATURE_testlib=OFF
    -DFEATURE_texthtmlparser=OFF
    -DFEATURE_textmarkdownreader=OFF
    -DFEATURE_textmarkdownwriter=OFF
    -DFEATURE_textodfwriter=OFF
    -DFEATURE_vulkan=OFF
    -DFEATURE_xml=OFF
    # Enable freetype and harfbuzz to get some sort of font support
    -DFEATURE_freetype=ON
    -DFEATURE_harfbuzz=ON
    # No FEATURE flags yet
    -DINPUT_libjpeg=no
    -DINPUT_libmd4c=no)
endif()

find_qt(REQUIRED COMPONENTS Core Widgets)

target_link_libraries(QtPlayground PRIVATE Qt::Core Qt::Widgets)

# Deploy
if(CMAKE_BUILD_TYPE STREQUAL Release)
  #
  set(ORGANISATION_NAME "ZIMO")
  set(APPLICATION_NAME "QtPlayground")
  set(INSTALLER_APP_DIR "@ApplicationsDirX64@")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.xml
                 ${CMAKE_BINARY_DIR}/config.xml)

  # Qt6 does not ship with fonts and we have built without fontconfig
  cpmaddpackage(
    NAME
    dejavu-fonts
    URL
    https://github.com/dejavu-fonts/dejavu-fonts/releases/download/version_2_37/dejavu-fonts-ttf-2.37.zip
    VERSION
    ${ARG_VERSION}
    DOWNLOAD_ONLY
    TRUE)
  add_custom_command(
    TARGET QtPlayground
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy
      ${dejavu-fonts_SOURCE_DIR}/ttf/DejaVuSansMono.ttf
      ${CMAKE_BINARY_DIR}/deploy/lib/fonts/DejaVuSansMono.ttf)

  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    # Get an old version of CQtDeployer compatible with GLIB 2.31
    find_package(CQtDeployer 1.5.4.17 COMPONENTS Linux)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake6)

    # Qt libs path
    execute_process(
      COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_LIBS
      OUTPUT_VARIABLE LIB_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Qt plugins path
    execute_process(
      COMMAND ${QMAKE_EXECUTABLE} -query QT_INSTALL_PLUGINS
      OUTPUT_VARIABLE QT_INSTALL_PLUGINS
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    file(GLOB EXTRA_PLUGIN ${QT_INSTALL_PLUGINS}/*)
    list(JOIN EXTRA_PLUGIN "," EXTRA_PLUGIN)

    add_custom_command(
      TARGET QtPlayground
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        clear #
        noTranslations #
        -bin $<TARGET_FILE:QtPlayground> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -extraLibs xcb-cursor #
        -extraPlugin ${EXTRA_PLUGIN} #
        -recursiveDepth 2 #
        -targetDir ${CMAKE_BINARY_DIR}/deploy #
        -verbose 3 #
        -platform linux_x86_64 #
    )

  elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    #
    find_package(CQtDeployer 1.6.2323 COMPONENTS Linux)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake)

    # GUI executable instead of a console application
    set_target_properties(QtPlayground PROPERTIES WIN32_EXECUTABLE TRUE)

    # For Windows we need Qt and system libs, so use CMAKE_FIND_ROOT_PATH
    list(JOIN CMAKE_FIND_ROOT_PATH "," LIB_DIR)

    add_custom_command(
      TARGET QtPlayground
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        clear #
        noTranslations #
        deploySystem #
        -bin $<TARGET_FILE:QtPlayground> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -recursiveDepth 2 #
        -targetDir ${CMAKE_BINARY_DIR}/deploy #
        -verbose 3 #
        -platform win_x86_64 #
    )
  endif()
endif()
