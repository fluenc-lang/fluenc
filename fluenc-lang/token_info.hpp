#pragma once

namespace peg
{
	template <typename Annotation>
	struct AstBase;

	struct EmptyType;

	using Ast = AstBase<EmptyType>;
}
