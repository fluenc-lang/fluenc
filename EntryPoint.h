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
		EntryPoint(const EntryPoint *parent
			, llvm::BasicBlock *block
			, llvm::LLVMContext *context
			, llvm::Module *module
			, llvm::Value *closure
			, DzTypeName *returnType
			, const std::map<std::string, DzFunction *> &functions
			, const std::map<std::string, DzValue *> &locals
			);

		const EntryPoint *parent() const;

		llvm::BasicBlock *block() const;
		llvm::LLVMContext *context() const;
		llvm::Module *module() const;
		llvm::Type *returnType() const;
		llvm::Value *closure() const;

		std::map<std::string, DzFunction *> functions() const;
		std::map<std::string, DzValue *> locals() const;

		EntryPoint withLocals(const std::map<std::string, DzValue *> &locals) const;
		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withClosure(llvm::Value *closure) const;
		EntryPoint withParent(const EntryPoint *parent) const;

	private:
		const EntryPoint *m_parent;

		llvm::BasicBlock *m_block;
		llvm::LLVMContext *m_context;
		llvm::Module *m_module;
		llvm::Value *m_closure;

		DzTypeName *m_returnType;

		std::map<std::string, DzFunction *> m_functions;
		std::map<std::string, DzValue *> m_locals;
};


#endif // ENTRYPOINT_H
