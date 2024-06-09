# libg203lightsync

A library for programmatically interacting with the Logitech G203 Lightsync mouse.

## Building

1. `git clone --recurse-submodules https://github.com/carlos-menezes/libg203lightsync.git`
2. `cd libg203lightsync`
3. `mkdir build && cdbuild`
4. `cmake ..`
5. `cmake --build .`


## Examples
Check the [examples](/examples) folder for example usage of the library.  The examples are excluded from the `ALL` target, thus, in order to build them, you must target them explicitly, e.g.:

```shell
cmake --build . --target set_solid_color google
```
