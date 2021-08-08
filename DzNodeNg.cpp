#include "DzNodeNg.h"

DzNodeNg::DzNodeNg(DzValueNg *top, DzValueNg *bottom)
	: m_top(top)
	, m_bottom(bottom)
{
}

DzValueNg *DzNodeNg::top() const
{
	return m_top;
}

DzValueNg *DzNodeNg::bottom() const
{
	return m_bottom;
}
