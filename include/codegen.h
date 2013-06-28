#ifndef CODEGEN_HPP
#define CODEGEN_HPP
#include <vector>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Type.h>
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/PassManager.h>
#include <llvm/Instructions.h>
#include <llvm/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IRBuilder.h>
#include <llvm/Module.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/raw_ostream.h>

class NBlock;
class AstNode;
namespace llvm
{
class Linker;
}

struct Symbol{
  Symbol():node(0),value(0){}
  Symbol(const Symbol& ss):node(ss.node),value(ss.value){}
  Symbol(AstNode * n, llvm::Value * v):node(n),value(v){}
  const Symbol& operator=(const Symbol & ss){
    node =ss.node;value = ss.value;
    return *this;
  }

  AstNode * node;
  llvm::Value * value;
};

class CodeGenBlock
{
public:
  llvm::BasicBlock *block;
  std::map<std::string, Symbol> locals;

};

class CodeGenContext
{
  std::vector<CodeGenBlock *> blocks;
  llvm::Function *mainFunction;

public:
  llvm::Module *module;
  llvm::Module * libs;
  llvm::Linker * linker;
  llvm::IRBuilder<> builder;
  CodeGenContext();

  void generateCode(NBlock& root);
  llvm::GenericValue runCode();
  std::map<std::string, Symbol>& locals();
  llvm::BasicBlock *currentBlock();
  void pushBlock(llvm::BasicBlock *block);
  void popBlock();
  void saveLLVMIR(const char * filename);
};
#endif
