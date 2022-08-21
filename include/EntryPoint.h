#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>

#include <map>

#include "immer/box.hpp"

#include "Stack.h"

class CallableNode;
class DzStruct;
class Prototype;
class IIteratorStorage;
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
			, immer::box<std::multimap<std::string, CallableNode *>> functions
			, immer::box<std::map<std::string, const BaseValue *>> locals
			, immer::box<std::map<std::string, const Node *>> globals
			, immer::box<std::map<std::string, Prototype *>> types
			, immer::box<Stack> values
			, IIteratorStorage *iteratorStorage
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

		const std::multimap<std::string, CallableNode *> &functions() const;
		const std::map<std::string, const BaseValue *> &locals() const;
		const std::map<std::string, const Node *> &globals() const;
		const std::map<std::string, Prototype *> &types() const;

		const Stack &values() const;

		IIteratorStorage *iteratorStorage() const;

		const EntryPoint *byName(const std::string &name) const;
		const EntryPoint *entry() const;

		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withAlloc(llvm::BasicBlock *alloc) const;
		EntryPoint withIteratorStorage(IIteratorStorage *iteratorStorage) const;
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

		immer::box<std::multimap<std::string, CallableNode *>> m_functions;
		immer::box<std::map<std::string, const BaseValue *>> m_locals;
		immer::box<std::map<std::string, const Node *>> m_globals;
		immer::box<std::map<std::string, Prototype *>> m_types;
		immer::box<Stack> m_values;

		IIteratorStorage *m_iteratorStorage;
};

#endif // ENTRYPOINT_H
