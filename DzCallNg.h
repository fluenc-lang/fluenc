#ifndef DZCALLNG_H
#define DZCALLNG_H

#include "DzValueNg.h"

class DzCallNg : public DzValueNg
{
	public:
		DzCallNg(const std::string &name, std::vector<DzValueNg *> parameters);

		llvm::Value *build(const EntryPointInfo &entryPoint) const override;

	private:
		std::string m_name;
		std::vector<DzValueNg *> m_parameters;
};

#endif // DZCALLNG_H
