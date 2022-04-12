#ifndef USE_H
#define USE_H

#include <string>

class Use
{
	public:
		Use(const std::string &fileName);

		std::string fileName() const;

	private:
		std::string m_fileName;
};

#endif // USE_H
