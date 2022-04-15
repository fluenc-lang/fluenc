#ifndef STRINGITERATABLE_H
#define STRINGITERATABLE_H

#include "values/IIteratable.h"

class StringIteratable : public IIteratable
{
	public:
		StringIteratable(const ReferenceValue *index, llvm::Value *address, size_t length);

		std::vector<DzResult> build(const EntryPoint &entryPoint) const override;

	private:
		const ReferenceValue *m_index;

		llvm::Value *m_address;

		size_t m_length;
};


#endif // STRINGITERATABLE_H
