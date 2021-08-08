#ifndef DZMEMBERNG_H
#define DZMEMBERNG_H

#include <string>

class DzTypeNameNg;

class DzMemberNg
{
	public:
		DzMemberNg(const std::string &name, DzTypeNameNg *type);

		std::string name() const;

		DzTypeNameNg *type() const;

	private:
		std::string m_name;

		DzTypeNameNg *m_type;
};
#endif // DZMEMBERNG_H
