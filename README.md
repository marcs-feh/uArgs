# μArgs : Tiny lib for CLI flags.

μArgs (stylized in ASCII as uArgs) is a small library that can handle short and
long flags, it supports the creation of flag objects and registries which can
be run against strings to run the flags and handle events such as an unknown
flag.

## Features

* Basic handling with callbacks
* Flag validation
* No heap allocations

## Building

Run `make` to create the static library, you can also embed the code directly
into your project pretty easily, as it consists of just 2 files.

## Examples

Check out the `examples.c` file, you can compile it in this dir with `clang -o
example examples.c -L. -luargs`, you can also delete this file as it isn't
required for the library to work.

