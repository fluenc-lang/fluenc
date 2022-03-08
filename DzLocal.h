#ifndef DZLOCAL_H
#define DZLOCAL_H

#include "DzValue.h"

class DzLocal : public DzValue
{
	public:
		DzLocal(const DzValue *consumer, const std::string &name);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const DzValue *m_consumer;

		std::string m_name;
};


#endif // DZLOCAL_H
