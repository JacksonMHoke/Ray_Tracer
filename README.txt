COMPILATION

To compile the code, run "scons" from the commandline. If you do not already
have this program, you will need to install it. The tool is comparable in
function to the "make" utility you may be familiar with.

The SConstruct file is similar to a makefile in that it contains instructions
for scons on how to build the code. SConstruct files written in python. You may
need to modify this file to get things to compile on your computer. (For
example, if libraries are located in different places.) You may also need to
install other libraries (such as the dev files for libpng)