# Qtz Framework

Qtz is a Qt-based Rapid Application Development Framework. Currently Qtz
consists of following libraries:

* QtzCore
  - Authentication 
  - Access Control Lists
  - User Management System
  - Settings Backend
* QtzData
  - Database Integration Backend (multiple vendors)
  - Data Management Utilities (cross-vendor backup, restore, encrypt tools)
  - Data Analysis Tools
* QtzWidgets
  - Advanced Editor Widgets
  - Data Management GUI
  - Security GUI Backend
  - Miscellaneous 

## Dependencies

This library depends on Qt4 or Qt5 and Crypto++. You also need a modern C++
compiler that implements C++11 specification. Currently GCC >= 4.6 and Clang are
supported. There is no guarantee to successful builds of Microsoft Visual C++
compilers, and there is no plan to support them in the future.

## Compile
To compile Qtz on *nix like environments type:

    qmake CONFIG+=release CONFIG+=local
    make

In Windows environment:
    mingw32-make

To speed up build process you may want to use `make -j<N>` where `<N>` is number
of your processors.

## Installation
If build has been finished successfully, you may want to install it:

    sudo make install

On Linux platforms all files will go through correct path. On Windows you may
need to define explicit paths.

    sudo mingw32-make install INSTALL_ROOT=c:\mingw64