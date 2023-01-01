#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <vector>
#include <map>
#include <unordered_map>
#include <string>

class CallableNode;
class BaseValue;
class Node;
class Prototype;
class Use;

struct ModuleInfo
{
	std::vector<const CallableNode *> roots;
	std::multimap<std::string, const CallableNode *> functions;
	std::map<std::string, const BaseValue *> locals;
	std::map<std::string, const Node *> globals;
	std::map<std::string, Prototype *> types;
	std::unordered_map<std::string, Use *> uses;

	static ModuleInfo merge(const ModuleInfo &left, const ModuleInfo &right, bool skipRoots)
	{
		ModuleInfo target;

		if (!skipRoots)
		{
			std::copy(begin(left.roots), end(left.roots), back_inserter(target.roots));
			std::copy(begin(right.roots), end(right.roots), back_inserter(target.roots));
		}

		std::copy(begin(left.functions), end(left.functions), inserter(target.functions, begin(target.functions)));
		std::copy(begin(right.functions), end(right.functions), inserter(target.functions, begin(target.functions)));

		std::copy(begin(left.locals), end(left.locals), inserter(target.locals, begin(target.locals)));
		std::copy(begin(right.locals), end(right.locals), inserter(target.locals, begin(target.locals)));

		std::copy(begin(left.globals), end(left.globals), inserter(target.globals, begin(target.globals)));
		std::copy(begin(right.globals), end(right.globals), inserter(target.globals, begin(target.globals)));

		std::copy(begin(left.types), end(left.types), inserter(target.types, begin(target.types)));
		std::copy(begin(right.types), end(right.types), inserter(target.types, begin(target.types)));

		std::copy(begin(left.uses), end(left.uses), inserter(target.uses, begin(target.uses)));
		std::copy(begin(right.uses), end(right.uses), inserter(target.uses, begin(target.uses)));

		return target;
	}
};

#endif // MODULEINFO_H
