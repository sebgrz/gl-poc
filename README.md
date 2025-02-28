### Build
```
# ../glfw is a directory with source code of glfw library
# ../glfw/build is a place where a compiled library exists
clang-15 -I ../glfw/include  main.c -o main -lm -lGL -L../glfw/build/src -lglfw3
```
