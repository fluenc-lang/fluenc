#pragma once

#include "ast/binary_node.hpp"

#include "fragment.hpp"

#include "Emitter.h"

struct binary_fragment_base : public fragment
{
	const fluenc::binary_node* binary;
};

struct integer_binary_fragment : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct boolean_binary_fragment : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct float_binary_fragment : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct string_binary_fragment : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct array_binary_fragment : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct without_binary_fragment : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct user_binary_fragment  : public binary_fragment_base
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};
