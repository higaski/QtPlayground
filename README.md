# QtPlayground

Overview over all GitHub runner-images  
https://github.com/actions/runner-images

Overview over glibc versions in different distros  
https://repology.org/project/glibc/versions

We need to build against an older version of glibc (e.g. 2.31 from Ubuntu 20.04 LTS).  
Ubuntu 18.04 ain't an option, it does only support GCC7.

Project maintaining a docker image for builds (maybe too old, it claims C++14, Qt6 needs C++17)  
https://github.com/phusion/holy-build-box

Ubuntu seems to have a mingw-w64 toolchain as well  
https://packages.ubuntu.com/search?keywords=mingw-w64

## Install guest additions on Ubuntu
```sh
sudo apt update
sudo apt upgrade
sudo apt install build-essential linux-headers-$(uname -r) -y
sudo ./VBoxLinuxAdditions.run
sudo usermod -aG vboxsf $USER
```

## Install build requirements
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

And then more... (XCB, OpenGL, ...)

https://doc.qt.io/qt-6/linux-requirements.html
https://forum.qt.io/topic/140822/qt-6-4-1-build-from-source-can-t-get-xcb-configured/10
```sh
sudo apt install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev
```

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