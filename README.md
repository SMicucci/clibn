# C lib n
Yet another... you know the drill.

This is my personal C library to implement utilities avaible for creating complex project.
It is made to work with posix Os only by design, no desire for portability (just run in a docker I guess?)

# Compiling

`Makefile` is made to run this kind of task
**Actions**:
- build => generate dynamic library and create a copy of header and dynamic library
- debug => build everything with debug information, either the test file with main function.
- static => create a static library from the dynamic library, but don't export that
- clean => remove object, binaries and copy generated

**Subproject + Action**:
- default action is always build
- run one of the action **only** in the desired directory

`lib.mk` exist as a template to be copied in a new sub project and be run by the main Makefile.
it's build to follow default action and be customized on names of directories, name of the library and list of dependencies.
Just a blueprint I like to share and recycle around
