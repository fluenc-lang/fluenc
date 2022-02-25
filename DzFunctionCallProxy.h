#ifndef DZFUNCTIONCALLPROXY_H
#define DZFUNCTIONCALLPROXY_H

#include "DzValue.h"

class DzFunctionCallProxy : public DzValue
{
	public:
		DzFunctionCallProxy(const std::string name, const DzValue *consumer, const DzValue *candidate);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> regularCall(const EntryPoint &entryPoint, Stack values) const;

		std::string m_name;

		const DzValue *m_consumer;
		const DzValue *m_subject;
};

#endif // DZFUNCTIONCALLPROXY_H
