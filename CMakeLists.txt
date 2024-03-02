cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
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

if(CMAKE_BUILD_TYPE STREQUAL Release)
  if(CMAKE_TOOLCHAIN_FILE)
    set(QT_TOOLCHAIN_FILE_OPTION -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE})
  else()
    set(QT_TOOLCHAIN_FILE_OPTION)
  endif()

  get_qt(
    VERSION
    6.6.1
    MODULES
    qtbase
    CMAKE_OPTIONS
    ${QT_TOOLCHAIN_FILE_OPTION}
    -DCMAKE_BUILD_TYPE=Release
    -DINPUT_optimize_size=ON
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${DCMAKE_INTERPROCEDURAL_OPTIMIZATION}
    # Disable as much as possible
    -DFEATURE_accessibility=OFF
    -DFEATURE_dbus=OFF
    -DFEATURE_cssparser=OFF
    -DFEATURE_glib=OFF
    -DFEATURE_fontconfig=OFF
    -DFEATURE_harfbuzz=OFF
    -DFEATURE_ico=OFF
    -DFEATURE_libjpeg=OFF
    -DFEATURE_libpng=ON
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
    # Disable all platform plugins but XCB
    -DFEATURE_linuxfb=OFF
    -DFEATURE_vkkhrdisplay=OFF
    -DFEATURE_vnc=OFF
    -DFEATURE_zstd=OFF
    # No FEATURE flags yet
    -DINPUT_libjpeg=no
    -DINPUT_libmd4c=no)
endif()

find_qt(REQUIRED COMPONENTS Core Widgets)

target_link_libraries(QtPlayground PRIVATE Qt::Core Qt::Widgets)

if(CMAKE_BUILD_TYPE STREQUAL Release)
  #
  set(ORGANISATION_NAME "ZIMO")
  set(APPLICATION_NAME "QtPlayground")
  set(INSTALLER_APP_DIR "@ApplicationsDirX64@")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.xml
                 ${CMAKE_BINARY_DIR}/config.xml)

  if(CMAKE_SYSTEM_NAME STREQUAL Linux)
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

    # Get an old version of CQtDeployer compatible with GLIB 2.31
    get_cqtdeployer(VERSION 1.5.4.17 SYSTEMS Linux)
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
        ${CMAKE_COMMAND} -E copy
        ${dejavu-fonts_SOURCE_DIR}/ttf/DejaVuSansMono.ttf
        ${CMAKE_BINARY_DIR}/deploy/lib/fonts/DejaVuSansMono.ttf
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        clear #
        noTranslations #
        # qif #
        -bin $<TARGET_FILE:QtPlayground> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -extraLibs xcb-cursor #
        -extraPlugin ${EXTRA_PLUGIN} #
        -recursiveDepth 2 #
        -targetDir ${CMAKE_BINARY_DIR}/deploy #
        -verbose 3 #
        -platform linux_x86_64 #
      # -qifOut QtPlayground-${PROJECT_VERSION}-x86_64 # -qifConfig
      # ${CMAKE_BINARY_DIR}/config.xml
    )

  elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    #
    get_cqtdeployer(VERSION 1.6.2345 SYSTEMS Linux Windows)
    file(GLOB_RECURSE QMAKE_EXECUTABLE ${QT_BINARY_DIR}/*/qmake6)

    # GUI executable instead of a console application
    set_target_properties(QtPlayground PROPERTIES WIN32_EXECUTABLE TRUE)

    # For Windows we need Qt and system libs, so use CMAKE_FIND_ROOT_PATH
    list(JOIN CMAKE_FIND_ROOT_PATH "," LIB_DIR)
    find_program(WINE_EXECUTABLE wine REQUIRED)

    add_custom_command(
      TARGET QtPlayground
      POST_BUILD
      COMMAND
        ${CQTDEPLOYER_EXECUTABLE} #
        force-clear #
        noTranslations #
        qif #
        -bin $<TARGET_FILE:QtPlayground> #
        -qmake ${QMAKE_EXECUTABLE} #
        -libDir ${LIB_DIR} #
        -recursiveDepth 2 #
        -targetDir ${CMAKE_BINARY_DIR}/deploy #
        -verbose 3 #
        -platform win_x86_64 #
        -qifOut QtPlayground-${PROJECT_VERSION}-x86_64 #
        -qifConfig ${CMAKE_BINARY_DIR}/config.xml #
        -binarycreator "${WINE_EXECUTABLE} ${BINARYCREATOR_WINDOWS_EXECUTABLE}")
  endif()
endif()

file(
  DOWNLOAD
  https://raw.githubusercontent.com/ZIMO-Elektronik/.clang-format/master/.clang-format
  ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
add_clang_format_target(QtPlaygroundFormat OPTIONS -i FILES ${SRC})
