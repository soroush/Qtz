# Qtz Framework [![Build Status](https://travis-ci.org/soroush/Qtz.png?branch=master)](https://travis-ci.org/soroush/Qtz)

Qtz is a Qt-based Application Development Framework. Currently Qtz consists of the following libraries:

* QtzCore
  - Solar Hijri (Jalali) Date and Time
  - Persian Number Formatter (Numeral to String converter)
  - Single Application Mechanism
  - Settings Backend
* QtzSecurity
  - Authentication
  - Access Control Lists
  - Simple Encription
* QtzData
  - Asynchronous Data Model (Thanks to [obeezzy](https://github.com/obeezzy/AsyncSql))
  - Database Integration Backend (multiple vendors)
  - Data Management Utilities (vendor-agnostic backup, restore, encryption tools)
  - Data Analysis Tools
* QtzWidgets
  - Advanced Editor Widgets
  - Data Management GUI
  - Security GUI Backend
  - OpenCV Image Viewer
  - Miscellaneous 

## Dependencies

This library depends on Qt5, OpenSSL and OpenCV. You also need a modern C++
compiler that implements C++11 specification. The library is built and tested
on following envoirnments:

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

## Compile
To compile Qtz on *nix like environments type:

    qmake CONFIG+=release CONFIG+=local CONFIG+=default_key
    make
    
The `default_key` passes the key
`f9bf8a579d1fa38c4e20ee884e4096c054b57771153799f49fb674d1006caf52` to internal

AES encryption algorithm. It's strongly recommended to
use your own key instead. You can pass your private key to qmake by adding:

    qmake CONFIG+=release CONFIG+=local DEFINES+=PRIVATE_KEY=\\\"YOUR_PRIVATE_HEX_KEY\\\"

Number of escapes depend on platform and shell. Typically in most Linux systems
(for example my current machine: Ubuntu 12.04 /bin/bash) there 7 escapes are
required. So you go like:

    qmake CONFIG+=release CONFIG+=local DEFINES+=PRIVATE_KEY=\\\\\\\"YOUR_PRIVATE_HEX_KEY\\\\\\\"

In Windows environment:

    mingw32-make

To speed up build process you may want to use `make -j<N>` where `<N>` is number
of your processors.

##Test
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


## Installation
If build has been finished successfully, you may want to install it:

    sudo make install

On Linux platforms all files will go through correct path. On Windows you may
need to define explicit paths.

    mingw32-make install INSTALL_ROOT=c:\mingw64
