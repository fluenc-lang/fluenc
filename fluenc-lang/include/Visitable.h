#ifndef VISITABLE_H
#define VISITABLE_H

template<typename... Args>
struct Types { };

template <typename... TTypes>
struct VisitableDeclaration;

template<typename TVisitor, typename ...TVisitors>
struct VisitableDeclaration<TVisitor, TVisitors...> : VisitableDeclaration<TVisitors...>
{
	virtual typename TVisitor::ReturnType accept(const TVisitor &visitor, typename TVisitor::ContextType context) const = 0;

	using VisitableDeclaration<TVisitors...>::accept;
};

template<typename TVisitor>
struct VisitableDeclaration<TVisitor>
{
	virtual typename TVisitor::ReturnType accept(const TVisitor &visitor, typename TVisitor::ContextType context) const = 0;
};

template<typename ...TVisitor>
struct Visitable : public VisitableDeclaration<TVisitor...>
{
	using VisitorType = Types<TVisitor...>;

	using VisitableDeclaration<TVisitor...>::accept;
};

template<typename TBase, typename TSelf, typename ...TVisitors>
struct VisitableImplementation;

template<typename TBase, typename TSelf, typename TVisitor, typename ...TVisitors>
struct VisitableImplementation<TBase, TSelf, TVisitor, TVisitors...> : VisitableImplementation<TBase, TSelf, TVisitors...>
{
	typename TVisitor::ReturnType accept(const TVisitor &visitor, typename TVisitor::ContextType context) const override
	{
		return visitor.visit(static_cast<const TSelf *>(this), context);
	}

	using VisitableImplementation<TBase, TSelf, TVisitors...>::accept;
};

template<typename TBase, typename TSelf, typename TVisitor>
struct VisitableImplementation<TBase, TSelf, TVisitor> : public TBase
{
	typename TVisitor::ReturnType accept(const TVisitor &visitor, typename TVisitor::ContextType context) const override
	{
		return visitor.visit(static_cast<const TSelf *>(this), context);
	}
};


template<typename TBase, typename TSelf, typename TVisitor>
struct VisitableExpansion;

template<typename TBase, typename TSelf, typename ...TVisitor>
struct VisitableExpansion<TBase, TSelf, Types<TVisitor...>> : public VisitableImplementation<TBase, TSelf, TVisitor...>
{
};

#endif // VISITABLE_H
