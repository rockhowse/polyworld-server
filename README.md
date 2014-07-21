polyworld
=========

This is a git repo taken from the original polyworld CVS branch. 

    http://shinyverse.org/larryy/Polyworld.html

It mainly follows the installation instructions found here:

    http://shinyverse.org/larryy/BuildingPolyworld.html#Linux

Copy of the instructions here, with modifications to make it work on Linux Mint 14 x64:

Linux
=============
First install Polyworld's dependencies:

   sudo apt-get install build-essential xorg-dev libgl1-mesa-dev scons cvs libgsl0-dev libqt4-opengl-dev python2.7-dev

   *NOTE* On unbutu, we had to include the python2.7-dev to fix the #include <Python.h> and other header file issues.
   
Solution taken from here: 

    http://stackoverflow.com/questions/8282231/ubuntu-i-have-python-but-gcc-cant-find-python-h

Then get your code from this github repo or fork it to your own and clone from there:

   https://www.github.com/rockhowse/polyworld

We then need to add a couple of symbolic links that are 

If you attempt make at this point you will get two errors:

1. /usr/bin/ld: .bld/Polyworld/utils/Mutex.o: undefined reference to symbol 'pthread_spin_init@@GLIBC_2.2.5'
2. /usr/bin/ld: .bld/Polyworld/proplib/cppprops.o: undefined reference to symbol 'dlsym@@GLIBC_2.2.5'
  
To fix this, add the directives "-pthread -ldl" to the very last g++ linker command example: 

    g++ -o Polyworld ... -pthread -ldl

Here is what each one fixes:

    -pthread ~ resolves the pthread_spin_init error.
    -ldl     ~ resolves the dslym error.

I will try to add these to the SCons (?) configuration.

Build Polyworld using make, and try the only worldfile that currently exists to test with:

    make
    ./Polyworld ./sandbox/social/feed_young.wf

For the analysis tools you will also need to install the following dependencies:

    sudo apt-get install python-numpy python-scipy gnuplot

All these installed fine for me, so nothing extra for Unbutu 14.

QTCreator
=====================
The original code I found didn't include a .pro file for building inside QTCreator. I have included a new polyworld.pro file that has the configuration I used. Basically, it does the following:

1. Includes all the base directories/.cp/.h file from the main folders in the polyworld project
2. I had to specify a specific C++ standard becasue by default it tried to use C++98
 
    QMAKE_CC = g++ -std=c++11

3. I had to add a bunch of libraries to the linking path in the .pro file as I saw no way to do it easily inside QTCreator GUI itself

    LIBS += -pthread -ldl -lz -lgsl -lgslcblas -lGL -lGLU -lpython2.7

I have yet to test and see if the application runs as intended. At this point it fully compiles inside QTCreator5.x on Mint 14 so I am making progress.
 
