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
			, const std::unordered_map<std::string, DzValue *> &fields
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_consumer;
		IPrototypeProvider *m_prototypeProvider;

		std::unordered_map<std::string, DzValue *> m_fields;
};

#endif // DZINSTANTIATION_H
