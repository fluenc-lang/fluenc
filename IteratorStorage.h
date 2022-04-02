#ifndef ITERATORSTORAGE_H
#define ITERATORSTORAGE_H

#include <llvm/IR/Value.h>

class EntryPoint;

class IteratorStorage
{
	public:
		llvm::Value *getOrCreate(size_t id, const EntryPoint &entryPoint);

	private:
		std::unordered_map<size_t, llvm::Value *> m_storage;
};

#endif // ITERATORSTORAGE_H
