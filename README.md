PhysLang
========

DSL for solid and deformable physics.

Needs to point enviroment variable PHC_ROOT to the root directory of the source.

Dependency:
Flex and Bison executable.
Clang 3.2.

LLVM 3.2 compiled from source using cmake.

Usage:
>cd extern
>./genIr.sh
>cd ..
>mkdir build
>cd build
> cmake ../ -DCMAKE_BUILD_TYPE=Debug
> make
> ./phc ../test/test.txt
> clang++ a.ll
>./a.out

