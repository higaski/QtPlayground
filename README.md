# QtPlayground

Overview over all GitHub runner-images  
https://github.com/actions/runner-images

Overview over glibc versions in different distros  
https://repology.org/project/glibc/versions

## Common prerequisites
We need guest additions on Ubuntu to mount shared folders
```sh
sudo apt update
sudo apt upgrade
sudo apt install build-essential linux-headers-$(uname -r) -y
sudo ./VBoxLinuxAdditions.run
sudo usermod -aG vboxsf $USER
```

Some build tools
```sh
sudo apt install git
sudo apt install ninja-build
```

And a newer version of CMake...  
https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line  
```sh
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4
sudo apt update
sudo apt install cmake
```

# Linux
For Linux we need to build against an older version of glibc (e.g. 2.31 from Ubuntu 20.04 LTS).  
Ubuntu 18.04 ain't an option, it does only support GCC7.

Install a bunch of dependencies for XCB and OpenGL  
https://doc.qt.io/qt-6/linux-requirements.html
https://forum.qt.io/topic/140822/qt-6-4-1-build-from-source-can-t-get-xcb-configured/10
```sh
sudo apt install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev
```

https://bugreports.qt.io/browse/QTBUG-117950  
The current version on Ubuntu 22.04 (6.2.4) fails to build with libxkbcommon-1.6.0 but works with 1.5.0

## CQtDeployer
First version which requires GLIBC_2.34 is v1.6.2181  
Versions before that follow a different naming scheme though...
The previous version (v1.5.4.17) has a .zip filed called CQtDeployer_1.5.4.17_Linux_x86_64.zip
Also... it doesn't have a windows zip version

## Platforms
https://stackoverflow.com/questions/21488072/what-is-the-use-of-various-qt-platform-plugins
So... on Linux Qt can use various platforms. The default seems to be xcb
So... xcb is trouble. When building with xcb on a system where it's installed, all is good. But Ubuntu 20.04 does not come with libxcb installed.
The good news is though, simply copying libxcb-cursor seems to fix that issue...

# Windows
For Windows we will use Ubuntu 22.04 because 20.04 does not have a qt6 package for our host.

Get host installation of Qt6 and MinGW
```sh
sudo apt install qt6-base-dev mingw-w64
```

And as I just found out the cross-compiled version can't be any newer than the HOST one!

Not sure if I need CMAKE_PREFIX_PATH or CMAKE_SYSROOT?
Currently CMake complains that it finds , but still sets the _FOUND variables to false
```sh
CMake Warning at build/_deps/qt6-build/lib/cmake/Qt6/Qt6Config.cmake:209 (find_package):
  Found package configuration file:

    /home/vinci/Downloads/QtPlayground/build/_deps/qt6-build/lib/cmake/Qt6Core/Qt6CoreConfig.cmake

  but it set Qt6Core_FOUND to FALSE so package "Qt6Core" is considered to be
  NOT FOUND.  Reason given by package:

  Target "Qt6::Core" was not found.

Call Stack (most recent call first):
  build/CMakeModules/cmake/find_qt.cmake:31 (find_package)
  CMakeLists.txt:109 (find_qt)

```

Ubuntu 22.04 mingw64 does not contain zlib and friends? The "3rdparty" folder inside qtbase contains it's source though? Why not build it from there?

Cmake finds cross compiled Qt6 config files but Qt6_FOUND is set to FALSE
https://forum.qt.io/topic/137551/cmake-finds-cross-compiled-qt6-config-files-but-qt6_found-is-set-to-false/6

Installed Qt6 to Linux ... cmake ... Target "Qt6::Core" was not found
https://forum.qt.io/topic/134586/installed-qt6-to-linux-cmake-target-qt6-core-was-not-found/6

CMake cannot find packages when only Qt6_DIR and not CMAKE_PREFIX_PATH is set
https://bugreports.qt.io/browse/QTBUG-97615

Improve find_package(Qt6) diagnostics
https://bugreports.qt.io/browse/QTBUG-104998?focusedCommentId=672482&page=com.atlassian.jira.plugin.system.issuetabpanels%3Acomment-tabpanel#comment-672482

Looks like CMAKE_PREFIX_PATH must point to the Qt6 installation as well? (not the host)