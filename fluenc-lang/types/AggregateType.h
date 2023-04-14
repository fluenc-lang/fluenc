#ifndef AGGREGATETYPE_H
#define AGGREGATETYPE_H

#include <sstream>
#include <numeric>

#include "Type.h"

class AggregateType : public Type
{
	public:
		AggregateType(const std::vector<const Type *> &subjects);

		TypeId id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> subjects() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static AggregateType *get(const std::vector<const Type *> &types);

	private:
		const std::vector<const Type *> m_subjects;
};

#endif // AGGREGATETYPE_H
