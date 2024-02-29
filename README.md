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

Trust all folders no matter the ownership (DON'T do this on a real machine!)
```sh
git config --global --add safe.directory "*"
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