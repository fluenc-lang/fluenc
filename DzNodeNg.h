#ifndef DZNODENG_H
#define DZNODENG_H

class DzValueNg;

class DzNodeNg
{
	public:
		DzNodeNg(DzValueNg *top, DzValueNg *bottom);

		DzValueNg *top() const;
		DzValueNg *bottom() const;

	private:
		DzValueNg *m_top;
		DzValueNg *m_bottom;
};

#endif // DZNODENG_H
