#ifndef DZINSTANTIATION_H
#define DZINSTANTIATION_H

#include <unordered_set>

#include <llvm/IR/Instructions.h>

#include "DzValue.h"
#include "DzTypeName.h"

class DzTypeName;
class DzAssignment;
class IPrototypeProvider;

class DzInstantiation : public DzValue
{
	public:
		DzInstantiation(DzValue *consumer
			, IPrototypeProvider *prototypeProvider
			, const std::vector<std::string> &fields
			);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
		IPrototypeProvider *m_prototypeProvider;

		std::vector<std::string> m_fields;
};

#endif // DZINSTANTIATION_H
