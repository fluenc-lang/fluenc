#ifndef DZTYPENAME_H
#define DZTYPENAME_H

#include "ITypeName.h"

class EntryPoint;
class Type;

class DzTypeName : public ITypeName
{
	public:
		DzTypeName(const std::string &name);

		Type *resolve(const EntryPoint &entryPoint) const override;

		static DzTypeName *int32();
		static DzTypeName *int64();
		static DzTypeName *uint32();
		static DzTypeName *byte();
		static DzTypeName *boolean();
		static DzTypeName *string();
		static DzTypeName *without();

	private:
		std::string m_name;
};

#endif // DZTYPENAME_H
