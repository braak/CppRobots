# CppRobots
The goal of CppRobots is the creation of a C++ framework for simulating AI-driven robots in a virtual environment. CppRobots was inspired by [CROBOTS](http://crobots.deepthought.it/home.php), a programming game first published in	december 1985. CppRobots does not intend to be a replacement for CROBOTS.

# Building #

The following sections will explain how to get and compile the project. Currently only Ubuntu systems are fully explained. If you are an expert feel free to deviate from the steps below.

## Requirements ##

This project needs the following things:
* A C++11-standard-compliant compiler
* make
* cmake
* The [SFML 2](http://www.sfml-dev.org/) Library.


### On Ubuntu ###
On Ubuntu install the following Packages:
* `cmake`
* `build-essential`
* `libsfml-dev`
* `git` (optional)
* `doxygen` (optional)

Just run the following command:
~~~~
sudo apt-get install git cmake build-essential libsfml-dev doxygen
~~~~


To install googletest go to https://github.com/google/googletest and follow their advice. Otherwise here is a quick way to do it (No guaranties):

~~~~~~~~~~~~~
git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake ..
make
sudo make install
~~~~~~~~~~~~~
## Getting the Source ##

To download the project run
~~~~
git clone https://github.com/braak/CppRobots
~~~~

Or download and extract the Zip file: https://github.com/braak/CppRobots/archive/master.zip

## Compiling ##
To build the project use `cmake` and `make`
~~~~~~~~~~~~~
cd CppRobots
mkdir build
cd build
cmake ..
make
~~~~~~~~~~~~~

## What it builds ##
The makefile will always build an executable called `robots`, this is a program that shows a small demo. If the [SFML 2](http://www.sfml-dev.org/) library is found on the system executable is build with a UI otherwise it prints its output to the console.

The executable depends on libraries `CppRobots` and `CppRobotsAgents`, which will be created automatically and placed in the `lib` folder. In addition the `CppRobotsSFML` will be build if SFML is used.

If Cmake finds [gtest](https://github.com/google/googletest) on the system it will generate the following tests:
* `testPose`
* `testVector`
* `testRectangle`
* `testCollision`
* `testSignal`


## Documentation ##
Creating the documentation requires [Doxygen](http://www.stack.nl/~dimitri/doxygen/). And optionally [Graphviz](http://www.graphviz.org/) for graphs.

To generate the documentation run:
~~~~~~~~~~~~~
make doc
~~~~~~~~~~~~~

# Copyright and license
Copyright 2016 Hochschule Emden/Leer.

This project is released under the MIT License, see the file [LICENSE.md](LICENSE.md) for rights and limitations.

This project uses the [Noto](https://github.com/googlei18n/noto-fonts) font. It is Licensed under the SIL Open Font License,
Version 1.1.
