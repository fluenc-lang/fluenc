#ifndef DZGLOBALTERMINATOR_H
#define DZGLOBALTERMINATOR_H

#include "DzValue.h"

class DzGlobalTerminator : public DzValue
{
	public:
		DzGlobalTerminator(const std::string &name);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::string m_name;
};

#endif // DZGLOBALTERMINATOR_H
