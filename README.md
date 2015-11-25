# CppRobots
The goal of CppRobots is the creation of a C++ framework for simulating AI-driven robots in a virtual environment. CppRobots was inspired by [CROBOTS](http://crobots.deepthought.it/home.php), a programming game first published in	december 1985. CppRobots does not intend to be a replacement for CROBOTS.

# Building #
To build the project use `cmake` and `make`
~~~~~~~~~~~~~
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
Copyright 2015 Jan-Niklas Braak.

This project is released under the MIT License, see the file LICENSE.md for rights and limitations.
