#ifndef DZTERMINATOR_H
#define DZTERMINATOR_H

#include "DzValue.h"

class DzTerminator : public DzValue
{
	public:
		DzTerminator(const std::string &name);

		int compare(DzValue *other, const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

		static DzTerminator *instance();

	private:
		std::string m_name;
};

#endif // DZTERMINATOR_H
