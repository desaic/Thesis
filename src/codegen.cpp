#include "Ast.hpp"
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
CodeGenContext::CodeGenContext():mainFunction(0)
{
  module = new Module("main", getGlobalContext());
  linker=new llvm::Linker("phc", module);

  char * externLibDir=getenv(PHC_ROOT_ENV);
  if(externLibDir==0){
    std::cout<<"Need Enviroment Variable "<<PHC_ROOT_ENV<<"\n";
  }
  else{
    std::cout<<"Parse print function\n";
    SMDiagnostic Err;
    std::string dir(externLibDir, strlen(externLibDir));
    std::string filename("/extern/print.s");
    filename = dir+filename;
    libs = llvm::ParseIRFile(filename.c_str(),Err,getGlobalContext());
    std::cout<<"Status: "<<Err.getMessage()<<"\n";
    if(libs == 0){
      std::cout<<"Error: cannot parse module "<<filename<<"\n";
      return;
    }

    std::cout<<"Link print function\n";
    std::string errorMsg;
    linker->LinkModules(module,libs,llvm::Linker::DestroySource,&errorMsg);
    std::cout<<"Status: "<<errorMsg<<"\n";
  }
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
	ReturnInst::Create(getGlobalContext(), bblock);
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

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const AstType& type)
{
  switch(type.typeId){
  case AstType::AST_VOID:
    return Type::getVoidTy(getGlobalContext());
    break;
  case AstType::AST_FLOAT:
      return Type::getFloatTy(getGlobalContext());
      break;
  case AstType::AST_DOUBLE:
      return Type::getDoubleTy(getGlobalContext());
      break;
  case AstType::AST_INT:
      return Type::getInt32Ty(getGlobalContext());
      break;
  //case struct todo
  }
  return Type::getVoidTy(getGlobalContext());
}

/* -- Code Generation -- */

Value* NInteger::codeGen(CodeGenContext& context)
{
	std::cout << "Creating integer: " << value << std::endl;
	return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value, true);
}

Value* NDouble::codeGen(CodeGenContext& context)
{
	std::cout << "Creating double: " << value << std::endl;
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), value);
}

Value* NFloat::codeGen(CodeGenContext& context)
{
  std::cout << "Creating float: " << value << std::endl;
  return ConstantFP::get(Type::getFloatTy(getGlobalContext()), value);
}

Value* NIdentifier::codeGen(CodeGenContext& context)
{
	std::cout << "Creating identifier reference: " << name << std::endl;
	if (context.locals().find(name) == context.locals().end()) {
		std::cerr << "undeclared variable " << name << std::endl;
		return NULL;
	}
	return new LoadInst(context.locals()[name], "", false, context.currentBlock());
}

Value* NMethodCall::codeGen(CodeGenContext& context)
{
	Function *function = context.module->getFunction(id.name.c_str());
	if (function == NULL) {
		std::cerr << "no such function " << id.name << std::endl;
		return NULL;
	}
	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		args.push_back((**it).codeGen(context));
	}
	CallInst *call = CallInst::Create(function, args, "", context.currentBlock());
	std::cout << "Creating method call: " << id.name << std::endl;
	return call;
}

Value* NBinaryOperator::codeGen(CodeGenContext& context)
{
	std::cout << "Creating binary operation " << op << std::endl;
	Instruction::BinaryOps instr;
	switch (op) {
		case ADD:
		  instr = Instruction::Add;
		  break;
		case SUB:
		  instr = Instruction::Sub;
		  break;
		case MUL:
		  instr = Instruction::Mul;
		  break;
		case DIV:
		  instr = Instruction::SDiv;
		  break;
		/* TODO comparison */
		default:
		  return NULL;
	}
	return BinaryOperator::Create(instr, lhs.codeGen(context), 
		rhs.codeGen(context), "", context.currentBlock());
}

Value* NAssignment::codeGen(CodeGenContext& context)
{
	std::cout << "Creating assignment for " << lhs.name << std::endl;
	if (context.locals().find(lhs.name) == context.locals().end()) {
		std::cerr << "undeclared variable " << lhs.name << std::endl;
		return NULL;
	}
	return new StoreInst(rhs.codeGen(context), context.locals()[lhs.name], false, context.currentBlock());
}

Value* NBlock::codeGen(CodeGenContext& context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << std::endl;
	return last;
}

Value* NExpressionStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating code for " << typeid(expression).name() << std::endl;
	return expression.codeGen(context);
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context)
{
	std::cout << "Creating variable declaration " << type->typeId << std::endl;
	AllocaInst *alloc = new AllocaInst(typeOf(*type), id.name.c_str(), context.currentBlock());
	context.locals()[id.name] = alloc;
	if (assignmentExpr != NULL) {
		NAssignment assn(id, *assignmentExpr);
		assn.codeGen(context);
	}
	return alloc;
}

Value* NFunctionDeclaration::codeGen(CodeGenContext& context)
{
	vector< Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf (*(**it).type));
	}
	FunctionType *ftype = FunctionType::get(typeOf(*type), argTypes, false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

	context.pushBlock(bblock);

	for (it = arguments.begin(); it != arguments.end(); it++) {
		(**it).codeGen(context);
	}
	
	block.codeGen(context);
	ReturnInst::Create(getGlobalContext(), bblock);

	context.popBlock();
	std::cout << "Creating function: " << id.name << std::endl;
	return function;
}
