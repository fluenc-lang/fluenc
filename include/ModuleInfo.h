#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <vector>
#include <map>
#include <unordered_set>
#include <string>

class CallableNode;
class BaseValue;
class Node;
class Prototype;

struct ModuleInfo
{
	std::vector<CallableNode *> roots;
	std::multimap<std::string, CallableNode *> functions;
	std::map<std::string, const BaseValue *> locals;
	std::map<std::string, const Node *> globals;
	std::map<std::string, Prototype *> types;
	std::unordered_set<std::string> uses;
};

#endif // MODULEINFO_H
