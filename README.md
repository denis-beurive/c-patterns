# C patterns

* [1](src/pattern1.c) Use a function to allocate an array of structures. The allocation is performed by a
  function to which we pass the address of a pointer used to store the address of the
  allocated memory location.
* [2](src/pattern2.c) Use a function to allocate an array of structures. The function returns a pointer to the 
  allocated array.

Compile:

```bash
cmake . && make clean && make
```




