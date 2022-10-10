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
};

#endif // MODULEINFO_H
