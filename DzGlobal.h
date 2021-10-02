#ifndef DZGLOBAL_H
#define DZGLOBAL_H

#include "DzValue.h"
#include "TypedValue.h"

class DzGlobal : public DzValue
{
	public:
		DzGlobal(DzValue *value, const std::string &name);

		std::string name() const;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		DzValue *m_value;

		std::string m_name;
};

#endif // DZGLOBAL_H
