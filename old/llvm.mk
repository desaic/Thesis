INCLUDE_FLAGS = -I/home/desaic/workspace/extern/llvm/llvm-3.2.src/include \
	-I/home/desaic/workspace/extern/llvm/llvm-3.2.src/build/include
CXX_FLAGS = -fPIC -fvisibility-inlines-hidden \
	 -D_GNU_SOURCE -Wall -W -Wno-unused-parameter \
         -Wwrite-strings -pedantic -Wno-long-long \
         -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS \
         -D__STDC_LIMIT_MACROS
LIBS = -L/home/desaic/workspace/extern/llvm/llvm-3.2.src/build/lib \
       -lLLVMX86AsmParser -lLLVMX86Disassembler \
       -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMMCParser \
       -lLLVMX86Desc -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils \
       -lLLVMJIT -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMCodeGen \
       -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils \
       -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMObject \
       -lLLVMCore -lLLVMSupport -ldl -lpthread
all: parser

clean:
	rm parser.cpp parser.hpp parser tokens.cpp

parser.cpp: parser.y
	bison -d -o $@ $^
	
parser.hpp: parser.cpp

tokens.cpp: tokens.l parser.hpp
	flex -o $@ $^

parser: parser.cpp codegen.cpp main.cpp tokens.cpp
	g++ -g -o $@ *.cpp ${INCLUDE_FLAGS} ${CXX_FLAGS} ${LIBS}
