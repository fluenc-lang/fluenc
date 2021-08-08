#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>

class DzCallable;

class EntryPoint
{
	public:
		EntryPoint(llvm::BasicBlock *block
			, llvm::Function *function
			, std::unique_ptr<llvm::Module> &module
			, std::unique_ptr<llvm::LLVMContext> &context
			, const std::map<std::string, DzCallable *> &functions
			, const std::map<std::string, llvm::Value *> &locals
			);

		llvm::BasicBlock *block() const;
		llvm::Function *function() const;

		std::unique_ptr<llvm::Module> &module() const;
		std::unique_ptr<llvm::LLVMContext> &context() const;

		std::map<std::string, DzCallable *> functions() const;
		std::map<std::string, llvm::Value *> locals() const;

		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withFunction(llvm::Function *function) const;
		EntryPoint withLocals(const std::map<std::string, llvm::Value *> &locals) const;

	private:
		llvm::BasicBlock *m_block;
		llvm::Function *m_function;

		std::unique_ptr<llvm::Module> &m_module;
		std::unique_ptr<llvm::LLVMContext> &m_context;

		std::map<std::string, DzCallable *> m_functions;
		std::map<std::string, llvm::Value *> m_locals;
};

#endif // ENTRYPOINT_H
