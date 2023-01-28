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
class ExpandedType;

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
			, immer::box<std::multimap<std::string, const CallableNode *>> functions
			, immer::box<std::map<std::string, const BaseValue *>> locals
			, immer::box<std::map<std::string, const Node *>> globals
			, immer::box<std::map<std::string, Prototype *>> types
			, immer::box<std::vector<const CallableNode *>> roots
			, immer::box<Stack> values
			, const ExpandedType *iteratorType
			);

		EntryPoint() = default;
		EntryPoint(const EntryPoint &) = default;

		void incorporate();

		int depth() const;
		int index() const;

		llvm::BasicBlock *block() const;

		llvm::Function *function() const;

		const ReferenceValue *alloc(const Type *type, const llvm::Twine &name = "") const;

		llvm::Module *module() const;
		llvm::LLVMContext *context() const;

		std::string name() const;
		std::string tag() const;

		const std::multimap<std::string, const CallableNode *> &functions() const;
		const std::map<std::string, const BaseValue *> &locals() const;
		const std::map<std::string, const Node *> &globals() const;
		const std::map<std::string, Prototype *> &types() const;
		const std::vector<const CallableNode *> &roots() const;

		const Stack &values() const;

		const EntryPoint *byName(const std::string &name) const;
		const EntryPoint *entry() const;
		const EntryPoint *parent() const;

		const ExpandedType *iteratorType() const;

		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withAlloc(llvm::BasicBlock *alloc) const;
		EntryPoint markEntry() const;
		EntryPoint withFunction(llvm::Function *function) const;
		EntryPoint withLocals(const std::map<std::string, const BaseValue *> &locals) const;
		EntryPoint withName(const std::string &name) const;
		EntryPoint withReturnValueAddress(llvm::Value *address) const;
		EntryPoint withValues(const Stack &values) const;
		EntryPoint withDepth(int depth) const;
		EntryPoint withIndex(int index) const;
		EntryPoint withIteratorType(const ExpandedType *iteratorType) const;
		EntryPoint detach() const;
		EntryPoint increaseDepth() const;

		void setParent(const EntryPoint &parent);

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

		immer::box<std::multimap<std::string, const CallableNode *>> m_functions;
		immer::box<std::map<std::string, const BaseValue *>> m_locals;
		immer::box<std::map<std::string, const Node *>> m_globals;
		immer::box<std::map<std::string, Prototype *>> m_types;
		immer::box<std::vector<const CallableNode *>> m_roots;
		immer::box<Stack> m_values;

		const ExpandedType *m_iteratorType;
};

#endif // ENTRYPOINT_H
