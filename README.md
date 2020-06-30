# Qtz Framework [![Build Status](https://travis-ci.org/soroush/Qtz.png?branch=master)](https://travis-ci.org/soroush/Qtz)

Qtz is a Qt-based Application Development Framework. Currently Qtz consists of 
the following libraries:

* QtzCore
  - Solar Hijri (Jalali) Date and Time
  - Persian Number Formatter (Numeral to String converter)
  - Single Application Instance Mechanism
  - Settings Backend
* QtzSecurity
  - Authentication
  - Access Control Lists
  - Simple Encription / Decryption and Hash algorithms
  - Software License Management
  - Virtual Machine Detection
  - Unique Hardwer Identifier
* QtzData
  - Asynchronous Data Model (Thanks to [obeezzy](https://github.com/obeezzy/AsyncSql))
  - Database Integration Backend (multiple RDMs)
  - Data Management Utilities (vendor-agnostic backup, restore, encryption tools)
  - Data Analysis Tools
* QtzWidgets
  - Advanced Editor Widgets (Editable Label, Various Tables, etc.)
  - Hexadecimal Editor (Based on [Dax89](https://github.com/Dax89/QHexEdit)'s work)
  - Data Management GUI
  - Security GUI Backend
  - OpenCV Image Viewer
  - Miscellaneous 

## Dependencies

This library depends on Qt5, QCA and OpenCV. Following versions of these
libraries were used to build Qtz. Any compatible version should also do the
work.

| Dependency      | Version              |
| ----------------|----------------------|
| QCA             | 2.1.3                |
| OpenCV          | 3.4.2                |


You will also need a modern C++ compiler that implements C++11 specification.
The library is built and tested on following envoirnments:

| Operating System      | Version / Distro     | Compiler                     |
| ----------------------|----------------------|------------------------------|
| Debian                | 8.0 jessie           | gcc 4.9.2                    |
|                       | 9.0 stretch          | gcc 6.3.0                    |
| Ubuntu                | 16.04 LTS xenial     | gcc 5.3.1                    |
|                       | 16.10 yakkety        | gcc 6.1.1                    |
|                       | 17.04 zesty          | gcc 6.3.0                    |
|                       | 17.10 artful         | gcc 7.2.0                    |
| Windows               | 7                    | MSVC 12.0 (Visual Studio 2013)  |
|                       | 7                    | MSVC 14.0 (Visual Studio 2015)  |
|                       | 7                    | MSVC 14.1 (Visual Studio 2017)  |

## Install Pre-Built Binaries

Debian/Ubuntu packages are available at following repository. You will need 
repository key to validate you got the package from the person you think you're 
getting it from. This prevent anyone injecting packages into your updates:

In order to install public key:

    wget -qO - https://deb.ametisco.ir/key.gpg | sudo apt-key add -
    
Fingerprint of the above key must be:

    8995 8D90 8EB5 CD04 46D7 D45D 457C EB8B 9CA8 25CC
    
Add repository:

    sudo add-apt-repository "deb https://deb.ametisco.ir/ bionic ametis"
    
Install software:

    sudo apt install \
        libqtzcore-dev libqtzdata-dev libqtzsecurity-dev libqtzwidgets-dev

## Compile
To compile Qtz on *nix like environments type:

    mkdir build && cd build
    cmake ..
    cmake --build .

To speed up build process you may want to use `make -j<N>` where `<N>` is number
of your processors.

## Test
Qtz is shipped with a comprehensive automated test suite. In order to run tests,
you need to specify input data to test program. After compiling the library, go
to build directory and run `tst_qtz` and pass input parameters in command line
as specified below:

`--enable-mysql` Enables MySQL test suite. Default: `set`.  
`--disable-mysql` Disables MySQL test suite. Default: `not-set`.  
`--mysql-database` Specifies MySQL database name. If `--disable-mysql` is set, this parameter is ignored.  
`--mysql-host` Specifies MySQL host. If `--disable-mysql` is set, this parameter is ignored.  
`--mysql-port` Specifies MySQL port number. If `--disable-mysql` is set, this parameter is ignored. Default: `3306`  
`--mysql-username` Specifies MySQL username. If `--disable-mysql` is set, this parameter is ignored. Default: `root`  
`--mysql-password` Specifies MySQL user's password. If `--disable-mysql` is set, this parameter is ignored.  
`--mysql-root-password` Specifies MySQL root's password. If `--disable-mysql` is set, this parameter is ignored.


## Install
If build has been finished successfully, you may want to install it:

    sudo cmake --install .

