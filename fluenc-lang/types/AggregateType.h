#ifndef AGGREGATETYPE_H
#define AGGREGATETYPE_H

#include <sstream>
#include <numeric>

#include "Type.h"

class AggregateType : public Type
{
	public:
		AggregateType(const std::vector<const Type *> &subjects);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;

		static AggregateType *get(const std::vector<const Type *> &types);

	private:
		const std::vector<const Type *> m_subjects;
};

#endif // AGGREGATETYPE_H
