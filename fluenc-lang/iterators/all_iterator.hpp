#pragma once

class all_true
{
public:
	all_true(bool& value);

	bool value() const;

	all_true& operator++();
	all_true& operator*();
	all_true& operator=(bool value);

private:
	bool& m_value;
};
