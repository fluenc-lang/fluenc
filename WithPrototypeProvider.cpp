#include "WithPrototypeProvider.h"
#include "DzFieldAccess.h"

#include "types/UserType.h"
#include "types/Prototype.h"

WithPrototypeProvider *WithPrototypeProvider::instance()
{
	static WithPrototypeProvider instance;

	return &instance;
}

Prototype *WithPrototypeProvider::provide(const EntryPoint &entryPoint, Stack values)
{
	UNUSED(entryPoint);

	auto addressOfValue = values.pop();

	auto valueOfType = (UserType *)addressOfValue.type();
	auto fields = valueOfType->fields();

	std::vector<PrototypeField> prototypeFields;

	std::transform(begin(fields), end(fields), std::back_insert_iterator(prototypeFields), [=](auto field) -> PrototypeField
	{
		return { field.name(), new DzFieldAccess(addressOfValue, field) };
	});

	return new Prototype(valueOfType->tag()
		, prototypeFields
		);
}
