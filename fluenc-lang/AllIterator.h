#ifndef ALLITERATOR_H
#define ALLITERATOR_H

class all_true
{
	public:
		all_true(bool &value);

		bool value() const;

		all_true &operator ++();
		all_true &operator *();
		all_true &operator =(bool value);

	private:
		bool &m_value;
};

#endif // ALLITERATOR_H
