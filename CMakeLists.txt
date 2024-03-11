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

add_executable(QtPlaygroundA src/a.cpp)
add_executable(QtPlaygroundB src/b.cpp)

# Qt6 requires C++17
target_compile_features(QtPlaygroundA PUBLIC cxx_std_17)
target_compile_features(QtPlaygroundB PUBLIC cxx_std_17)

find_qt(REQUIRED COMPONENTS Core Widgets)

target_link_libraries(QtPlaygroundA PRIVATE Qt::Core Qt::Widgets)
target_link_libraries(QtPlaygroundB PRIVATE Qt::Core Qt::Widgets)

# Get an old version of CQtDeployer compatible with GLIB 2.31
find_package(CQtDeployer 1.5.4.17 COMPONENTS Linux)
find_program(QMAKE_EXECUTABLE qmake6 REQUIRED)

add_custom_target(
  QtPlaygroundDeploy
  COMMAND
    ${CQTDEPLOYER_EXECUTABLE} #
    clear #
    noTranslations #
    -bin $<TARGET_FILE:QtPlaygroundA>,$<TARGET_FILE:QtPlaygroundB> #
    -qmake ${QMAKE_EXECUTABLE} #
    -recursiveDepth 2 #
    -targetDir ${CMAKE_BINARY_DIR}/deploy #
    -verbose 3 #
    -platform linux_x86_64 #
  DEPENDS QtPlaygroundA QtPlaygroundB #
)
