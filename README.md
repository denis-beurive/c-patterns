# C patterns

* [1](src/pattern1.c) Use a function to allocate an array of structures. The allocation is performed by a
  function to which we pass the address of a pointer used to store the address of the
  allocated memory location.
* [2](src/pattern2.c) Use a function to allocate an array of structures. The function returns a pointer to the 
  allocated array.
* [3](src/pattern3.c) Error reporting.
* [4](src/pattern4.c) How to extend a list of enums.

# Compile

```bash
cmake . && make clean && make
```

# Run tests

First compile the project, and then execute:

```bash
make test
```

# Run the executables

```bash
./bin/pattern1
./bin/pattern2
./bin/pattern3
./bin/pattern4
```
