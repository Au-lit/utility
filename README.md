# Au-lit/utility-functions
A collection of fast C++ utility functions / small libs all in single header files with no code implications.
Files | Inside namespace | C++ standard required | Description
------------ | ------------- | ------------- | -------------
random_value.hpp | Aulit | C++11 | A simple function for random values.
is_char.hpp | Aulit | C++11 | A type trait to find if a type is a character type.
scope.hpp | Aulit (inline namespace standard::experimental) | C++11 | `scope_exit` class, etc.
endian.hpp | Aulit::endian | C++20 | A small library to handle endianness.

# FAQ
#### Where is the documetation? I don't see it in the code...
See this repo wiki. (I dislike putting all the ducumentation in one single file).
#### Why not C?
Because [stb](https://github.com/nothings/stb) already exists for C. (and because it's not my language. And C++ is clearer.)
#### Why is this not public domain?
Well it almost is; when you included a file form here there is already the copyright notice in it so for open-source projects you don't need to do anything special.
