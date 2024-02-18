#pragma once

#include "ast/unary_node.hpp"

#include "fragment.hpp"

#include "Emitter.h"

struct unary_fragment : public fragment
{
	const fluenc::unary_node* unary;
};

struct integer_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct boolean_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct float_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct string_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct array_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct without_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};

struct user_unary_fragment : public unary_fragment
{
	std::vector<DzResult> build(const Emitter &visitor, DefaultVisitorContext context) const override
	{
		return visitor.visit(this, context);
	}
};
