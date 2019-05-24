Memory Allocation Tracer
=======================

Proof of concept for simple memory allocation tracer and memory leak detection.

Description
-----------

With this shared library we are able to hook functions that operate on memory such as `malloc`, `realloc` or `free` with the use of `dlsym` and track allocations and frees used in a program loaded with `LD_PRELOAD=hooks.so` enviromental variable.

