#include "Ast.hpp"
#include "NVariableDeclaration.hpp"
#include "codegen.h"
#include "parser.hpp"
#include <llvm/Support/IRReader.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Linker.h>
using namespace std;
#include <typeinfo>
#include <fstream>
#include <string.h>
#include <stdlib.h>
const char * PHC_ROOT_ENV="PHC_ROOT";
using namespace llvm;

Symbol
CodeGenContext::findSymbol(const std::string & name)
{
  for(int ii = (int)blocks.size()-1; ii>=0; ii--){
    std::map<std::string, Symbol> & vars = blocks[ii]->locals;
    if(vars.find(name) != vars.end()){
      return vars[name];
    }
  }
  return Symbol();
}

CodeGenContext::CodeGenContext():
  mainFunction(0),
  builder(getGlobalContext())
{
  module = new Module("main", getGlobalContext());
  linker=new llvm::Linker("phc", module);

  char * externLibDir=getenv(PHC_ROOT_ENV);
  if(externLibDir==0){
    std::cout<<"Need Enviroment Variable "<<PHC_ROOT_ENV<<"\n";
    return;
  }
  std::cout << "Parse print function\n";
  SMDiagnostic Err;
  std::string dir(externLibDir, strlen(externLibDir));
  std::string filename("/extern/print.s");
  filename = dir + filename;
  libs = llvm::ParseIRFile(filename.c_str(), Err, getGlobalContext());
  std::cout << "Status: " << Err.getMessage() << "\n";
  if (libs == 0) {
    std::cout << "Error: cannot parse module " << filename << "\n";
    return;
  }

  std::cout << "Link print function\n";
  std::string errorMsg;
  linker->LinkModules(module, libs, llvm::Linker::DestroySource, &errorMsg);
  std::cout << "Status: " << errorMsg << "\n";

}

void CodeGenContext::saveLLVMIR(const char * filename)
{
  std::ofstream out(filename);
  if(!out.good()){
    std::cout<<"Error: Cannot open output file "<<filename<<"\n";
    return;
  }
  llvm::raw_os_ostream llvmout (out);
  llvmout<<*module;
  out.close();
}

/* Compile the AST into a module */
void CodeGenContext::generateCode(NBlock& root)
{
	std::cout << "Generating code...\n";
	
	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
	mainFunction = Function::Create(ftype, GlobalValue::ExternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
	/* Push a new variable/block context */
	pushBlock(bblock);
	root.codeGen(*this); /* emit bytecode for the toplevel block */
	builder.CreateRetVoid();
	popBlock();
	
	/* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
	//std::cout << "Code is generated.\n";
	//PassManager pm;
	//pm.add(createPrintModulePass(&outs()));
	//pm.run(*module);
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
	std::cout << "Running code...\n";
	InitializeNativeTarget();
	ExecutionEngine *ee = ExecutionEngine::create(module, false);
	vector<GenericValue> noargs;
	GenericValue v = ee->runFunction(mainFunction, noargs);
	std::cout << "Code was run.\n";
	return v;
}

std::map<std::string, Symbol>&
CodeGenContext::locals()
{
  return blocks.back()->locals;
}

llvm::BasicBlock *
CodeGenContext::currentBlock()
{
  return blocks.back()->block;
}

void
CodeGenContext::pushBlock(llvm::BasicBlock *block)
{
  blocks.push_back(new CodeGenBlock());
  blocks.back()->block = block;
  builder.SetInsertPoint(block);
}

void
CodeGenContext::popBlock()
{
  CodeGenBlock *top = blocks.back();
  blocks.pop_back();
  if (blocks.size() > 0) {
    builder.SetInsertPoint(blocks.back()->block);
  }
  delete top;
}

/* -- Code Generation -- */

Value* NAssignment::codeGen(CodeGenContext& context)
{
	std::cout << "Creating assignment for " << lhs.name << std::endl;
	Symbol leftSymbol = context.findSymbol(lhs.name);
	if (leftSymbol.node==0) {
		std::cerr << "undeclared variable " << lhs.name << std::endl;
		return NULL;
	}


	Value * castInst = NULL;
	castInst = rhs.codeGen(context);
	if( leftSymbol.node->type.getId() != rhs.type.getId() ){
	  castInst = cast(&(rhs.type), &(leftSymbol.node->type), castInst,context);
	  if(castInst == NULL){
	    std::cout<<"Error: no known conversion from type "<<rhs.type.toString()
	        <<" to "<<leftSymbol.node->type.toString()<<"\n";
	    delete castInst;
	    return NULL;
	  }
	}
  return context.builder.CreateStore(castInst, leftSymbol.value, false);
}

Value* NExpressionStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating code for " << typeid(expression).name() << std::endl;
	return expression.codeGen(context);
}
