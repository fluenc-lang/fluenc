#ifndef ENTRYPOINTINFO_H
#define ENTRYPOINTINFO_H

#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

class DzTypeNameNg;
class DzCallable;

class EntryPointInfo
{
	public:
		EntryPointInfo(DzTypeNameNg *returnType
			, std::unique_ptr<llvm::LLVMContext> &context
			, std::unique_ptr<llvm::Module> &module
			, llvm::BasicBlock *block
			, llvm::Function *function
			, const std::map<std::string, DzCallable *> &functions
			, const std::map<std::string, llvm::Value *> &locals
			, const std::string &name
			);

		DzTypeNameNg *returnType() const;

		std::unique_ptr<llvm::LLVMContext> &context() const;
		std::unique_ptr<llvm::Module> &module() const;

		llvm::BasicBlock *block() const;
		llvm::Function *function() const;

		std::map<std::string, DzCallable *> functions() const;
		std::map<std::string, llvm::Value *> locals() const;

		std::string name() const;

		EntryPointInfo withBlock(llvm::BasicBlock *block) const;
		EntryPointInfo withFunction(llvm::Function *function) const;
		EntryPointInfo withLocals(const std::map<std::string, llvm::Value *> &locals) const;
		EntryPointInfo withName(const std::string &name) const;

	private:
		DzTypeNameNg *m_returnType;

		std::unique_ptr<llvm::LLVMContext> &m_context;
		std::unique_ptr<llvm::Module> &m_module;

		llvm::BasicBlock *m_block;
		llvm::Function *m_function;

		std::map<std::string, DzCallable *> m_functions;
		std::map<std::string, llvm::Value *> m_locals;

		std::string m_name;
};

#endif // ENTRYPOINTINFO_H
