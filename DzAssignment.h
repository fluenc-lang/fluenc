#ifndef DZASSIGNMENT_H
#define DZASSIGNMENT_H

#include <string>

class DzValue;

class DzAssignment
{
	public:
		DzAssignment(const std::string &name, DzValue *value);

		std::string name() const;

		DzValue *value() const;

	private:
		std::string m_name;


};

#endif // DZASSIGNMENT_H
