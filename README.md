simple-rnn
=====
recurrent neuronal network in c++ with boost

This is a partially simplified version of Alex Graves incredible multidimensional rnnlib.
It uses a smaller feature set and is reduced in complexity to work with one dimensional data again.
More details on his implementation can be found in http://www.cs.toronto.edu/~graves/phd.pdf, all credits for this work go to Alex Graves. The author of this library does not own anything but tries to maintain this library to the best of his knowledge.

The original project was released under a GPLv3 license, and the same applies here.

SIMPLE RNN readme
---------------

RNNLIB is written in C++ and should compile on any platform. However it was reworked and tested only with VS2012.

Building it requires the following:

* A modern C++ compiler (e.g. gcc 3.0 or higher)
* Boost C++ Libraries version 1.36 or higher (headers only, no compilation needed)
