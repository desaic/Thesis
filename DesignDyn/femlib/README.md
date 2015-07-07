PaintMat
========

External Dependencies:

In ubuntu the dependencies can be installed by installing the ipopt package.
>sudo apt-get install coinor-libipopt-dev

If you are not using openblas , simply remove dependency on openblas in CMakeLists.txt

Otherwise you need to compile it and its dependency from source.
blas
  - openblas: https://github.com/xianyi/OpenBLAS
    - plus a fortran compiler runtime
       in ubuntu, gfortran can be installed by
        > sudo apt-get install gfortran
    To compile
    cd into openblas directory and
    > make
    copy libopenblas* to femLib/extern/lib
Ipopt 3.10 or higher: http://www.coin-or.org/download/source/Ipopt/Ipopt-3.11.7.zip
  To compile:
    cd into the ipopt directory
    cd ThirdParty
    cd Mumps
    ./get.Mumps
    cd ../../
    mkdir build
    ../configure --with-blas="-LAbsolutePathToDirectoryOfBlasLib -lopenblasOrYourBlasLibrary"
    make
    make install
  Copy include and lib directories to femLib/extern/lib/Ipopt
  You also need a linear solver such as pardiso, hsl or mumps.
  https://projects.coin-or.org/Ipopt/wiki/CompilationHints

In femLib/extern/lib
expects a static library file:
  libopenblas.a
  
In extern/ipopt
expects two directories.
  lib and include
  They are copied from the corresponding directories in ipopt package after compiling it.
