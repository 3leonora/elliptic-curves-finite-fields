# Elliptic Curves over Finite Fields

A code that checks the solutions for elliptic curves over large prime fields (p<=2^40)

Informative papers on the topic:
- [Sato-Tate distributions by Andrew V. Sutherland](https://math.mit.edu/~drew/conm14904.pdf)
- 


## Building

This project aims for building and running on Linux and MacOS systems. The bazel build tool need to be downloaded and installed. Instructions can be found on [this page](https://bazel.build/install).

For now this project uses C++20 and one might have to download and install the clang compiler toolchain version 16 or higher. See the .bazelrc in the project root for how one can change the default C++ toochain to use.

### Example build invocations

Build and run all tests:

    bazel run //test/...

Compile the primefield cc_library on Linux using clang
    
    bazel build --config=k8_clang //src/libs:primefield

Clean the build (prepare for the next build with -s). Normally not needed. This will trigger a rebuild of the google test framework as well (it will not remove the download of it however):

    bazel clean

Cleaning might complain about the unorthodox way of configuring the toolchain.

Build all tests and show the actual tool invocations on stdout:

    bazel build -s //test/...

## TODO

primefield lib:
- Finnish inverse routines
- Montgomery helper functions
- Simple prime sieve
- Parallelization
