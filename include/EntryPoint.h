#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>

#include <map>

#include "Stack.h"

class CallableNode;
class DzStruct;
class Prototype;
class IteratorStorage;
class Node;
class ReferenceValue;
class Type;

class EntryPoint
{
	public:
		EntryPoint(int depth
			, int index
			, const EntryPoint *parent
			, const EntryPoint *entry
			, llvm::BasicBlock *block
			, llvm::BasicBlock *alloc
			, llvm::Function *function
			, llvm::Module *module
			, llvm::LLVMContext *context
			, const std::string &name
			, const std::multimap<std::string, CallableNode *> &functions
			, const std::map<std::string, const BaseValue *> &locals
			, const std::map<std::string, const Node *> &globals
			, const std::map<std::string, Prototype *> &types
			, const Stack &values
			, IteratorStorage *iteratorStorage
			);

		EntryPoint() = default;
		EntryPoint(const EntryPoint &) = default;

		int depth() const;
		int index() const;

		llvm::BasicBlock *block() const;

		llvm::Function *function() const;

		const ReferenceValue *alloc(const Type *type, const llvm::Twine &name = "") const;

		llvm::Module *module() const;
		llvm::LLVMContext *context() const;

		std::string name() const;
		std::string tag() const;

		std::multimap<std::string, CallableNode *> functions() const;
		std::map<std::string, const BaseValue *> locals() const;
		std::map<std::string, const Node *> globals() const;
		std::map<std::string, Prototype *> types() const;

		Stack values() const;

		IteratorStorage *iteratorStorage() const;

		const EntryPoint *byName(const std::string &name) const;
		const EntryPoint *entry() const;

		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withAlloc(llvm::BasicBlock *alloc) const;
		EntryPoint withIteratorStorage(IteratorStorage *iteratorStorage) const;
		EntryPoint markEntry() const;
		EntryPoint withFunction(llvm::Function *function) const;
		EntryPoint withLocals(const std::map<std::string, const BaseValue *> &locals) const;
		EntryPoint withName(const std::string &name) const;
		EntryPoint withReturnValueAddress(llvm::Value *address) const;
		EntryPoint withValues(const Stack &values) const;
		EntryPoint withDepth(int depth) const;
		EntryPoint withIndex(int index) const;

	private:
		int m_depth;
		int m_index;

		const EntryPoint *m_parent;
		const EntryPoint *m_entry;

		llvm::BasicBlock *m_block;
		llvm::BasicBlock *m_alloc;

		llvm::Function *m_function;

		llvm::Module *m_module;
		llvm::LLVMContext *m_context;

		std::string m_name;

		std::multimap<std::string, CallableNode *> m_functions;
		std::map<std::string, const BaseValue *> m_locals;
		std::map<std::string, const Node *> m_globals;
		std::map<std::string, Prototype *> m_types;

		Stack m_values;

		IteratorStorage *m_iteratorStorage;
};

#endif // ENTRYPOINT_H
