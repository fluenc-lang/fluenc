#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>

#include "Stack.h"

class DzCallable;
class DzStruct;
class Prototype;

class EntryPoint
{
	const char *empty = "_empty";

	public:
		EntryPoint(int depth
			, const EntryPoint *parent
			, const EntryPoint *entry
			, llvm::BasicBlock *block
			, llvm::BasicBlock *alloc
			, llvm::Function *function
			, llvm::Value *returnValueAddress
			, std::unique_ptr<llvm::Module> *module
			, std::unique_ptr<llvm::LLVMContext> *context
			, const std::string &name
			, const std::stack<std::string> &tag
			, const std::multimap<std::string, DzCallable *> &functions
			, const std::map<std::string, const BaseValue *> &locals
			, const std::map<std::string, Prototype *> &types
			, const Stack &values
			);

		EntryPoint() = default;
		EntryPoint(const EntryPoint &) = default;

		int depth() const;

		llvm::BasicBlock *block() const;

		llvm::Function *function() const;
		llvm::Value *returnValueAddress() const;

		llvm::Value *alloc(llvm::Type *type) const;

		std::unique_ptr<llvm::Module> &module() const;
		std::unique_ptr<llvm::LLVMContext> &context() const;

		std::string name() const;
		std::string tag() const;

		std::multimap<std::string, DzCallable *> functions() const;
		std::map<std::string, const BaseValue *> locals() const;
		std::map<std::string, Prototype *> types() const;

		Stack values() const;

		const EntryPoint *byName(const std::string &name) const;
		const EntryPoint *entry() const;

		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withAlloc(llvm::BasicBlock *alloc) const;
		EntryPoint markEntry() const;
		EntryPoint withFunction(llvm::Function *function) const;
		EntryPoint withLocals(const std::map<std::string, const BaseValue *> &locals) const;
		EntryPoint withName(const std::string &name) const;
		EntryPoint pushTag(const std::string &tag) const;
		EntryPoint popTag() const;
		EntryPoint withReturnValueAddress(llvm::Value *address) const;
		EntryPoint withValues(const Stack &values) const;
		EntryPoint withDepth(int depth) const;

	private:
		int m_depth;

		const EntryPoint *m_parent;
		const EntryPoint *m_entry;

		llvm::BasicBlock *m_block;
		llvm::BasicBlock *m_alloc;

		llvm::Function *m_function;
		llvm::Value *m_returnValueAddress;

		std::unique_ptr<llvm::Module> *m_module;
		std::unique_ptr<llvm::LLVMContext> *m_context;

		std::string m_name;

		std::stack<std::string> m_tags;
		std::multimap<std::string, DzCallable *> m_functions;
		std::map<std::string, const BaseValue *> m_locals;
		std::map<std::string, Prototype *> m_types;

		Stack m_values;
};

#endif // ENTRYPOINT_H
