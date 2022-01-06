#ifndef ITERATORTYPEHANDLE_H
#define ITERATORTYPEHANDLE_H

class IteratorType;
class Type;

class IteratorTypeHandle
{
	public:
		IteratorTypeHandle();

		const Type *type() const;

	private:
		IteratorType *m_type;
};

#endif // ITERATORTYPEHANDLE_H
