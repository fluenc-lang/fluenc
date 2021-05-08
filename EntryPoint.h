#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/BasicBlock.h>

#include <string>
#include <map>

class DzFunction;
class DzValue;
class DzTypeName;

class EntryPoint
{
	public:
		EntryPoint(llvm::BasicBlock *block
			, llvm::LLVMContext *context
			, llvm::Module *module
			, DzTypeName *returnType
			, const std::map<std::string, DzFunction *> &functions
			, const std::map<std::string, llvm::Value *> &locals
			);

		llvm::BasicBlock *block() const;
		llvm::LLVMContext *context() const;
		llvm::Module *module() const;
		llvm::Type *returnType() const;
		llvm::Function *function() const;

		std::map<std::string, DzFunction *> functions() const;
		std::map<std::string, llvm::Value *> locals() const;

		EntryPoint withLocals(const std::map<std::string, llvm::Value *> &locals) const;
		EntryPoint withBlock(llvm::BasicBlock *block) const;

	private:
		llvm::BasicBlock *m_block;
		llvm::LLVMContext *m_context;
		llvm::Module *m_module;

		DzTypeName *m_returnType;

		std::map<std::string, DzFunction *> m_functions;
		std::map<std::string, llvm::Value *> m_locals;
};


#endif // ENTRYPOINT_H
