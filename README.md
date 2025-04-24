# C lib n
Yet another... you know the drill.

This is my personal C library to implement utilities avaible for creating complex project.  
It is made to work with posix Os only by design, no desire for portability (just run in a docker I guess?)  

## deafult API

The API from this library are build like this: `<data-structure>_<action>`.  

Here are some possible action with theyr meanings:  
- **size**: retrieval of the memory size of the kind of element stored  
- **length**: retrieval of the number of elements stored  
- **type**: retrieval of the kind of element stored
- **insert_first**: insertion of an element in the first position  
- **insert_last**: insertion of an element in the last position  
- **insert_at**: insertion of an element in any valid position  
- **remove_first**: removal of an element in the first position  
- **remove_last**: removal of an element in the last position  
- **remove_at**: removal of an element in any valid position  
- **peek_first**: peek of an element in the first position  
- **peek_last**: peek of an element in the last position  
- **peek_at**: peek of an element in any valid position  
- **set_first**: modify of an element in the first position  
- **set_last**: modify of an element in the last position  
- **set_at**: modify of an element in any valid position  

# Compiling

`Makefile` is made to run this kind of task  
**Actions**:
- build => generate dynamic library and create a copy of header and dynamic library
- debug => build everything with debug information, either the test file with main function.
- test => run test under valgrind to check for memory leakage
- clean => remove object, binaries and copy generated

**Subproject + Action**:
- default action is always build
- run one of the action **only** in the desired directory

`lib.mk` exist as a template to be copied in a new sub project and be run by the main Makefile.  
it's build to follow default action and be customized on names of directories, name of the library and list of dependencies.  
Just a blueprint I like to share and recycle around  
