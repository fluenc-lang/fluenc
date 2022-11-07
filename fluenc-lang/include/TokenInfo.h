#ifndef TOKENINFO_H
#define TOKENINFO_H

namespace peg
{
	template <typename Annotation>
	struct AstBase;

	struct EmptyType;

	using Ast = AstBase<EmptyType>;
}

#endif // TOKENINFO_H
