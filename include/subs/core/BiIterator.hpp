#pragma once
#include <string>

struct biIterator
{
	typedef char Char;
	typedef std::basic_string<Char> _string;

	biIterator() {};
	biIterator(const biIterator& cpy) : begin(cpy.begin), end(cpy.end) {};
	biIterator(biIterator&& mov)
	{
		swap(mov);
	};
	biIterator(typename _string::const_iterator b, typename _string::const_iterator e) : begin(b), end(e) {};
	biIterator(const _string& s) : begin(s.begin()), end(s.end()) {};
	biIterator& operator=(biIterator biit)
	{
		swap(biit);
		return *this;
	}
	void swap(biIterator& biit)
	{
		std::swap(biit.begin, begin);
		std::swap(biit.end, end);
	}
	bool operator==(const _string& comp) const
	{
		return compare(comp) == 0;
	}
	bool operator!=(const _string& comp) const
	{
		return operator!=(biIterator(comp));
	}
	bool operator==(const biIterator& comp) const
	{
		return compare(comp)==0;
	}
	bool operator!=(const biIterator& comp) const
	{
		return !operator==(comp);
	}
	operator _string() const
	{
		return str();
	}
	void set(typename _string::const_iterator b, typename _string::const_iterator e)
	{
		std::swap(b, begin);
		std::swap(e, end);
	}
	size_t size() const
	{
		return std::distance(begin, end);
	}
	size_t length() const
	{
		return size();
	}
	int compare(const _string& comp) const
	{
		return compare(biIterator(comp.begin(), comp.end()));
	}
	/*
	return same as std::strcomp... maybe... kind of
	*/
	int compare(const biIterator& comp) const
	{
		if (empty() && comp.empty())
			return false;
		else if (empty())
			return -*comp.begin;
		else if (comp.empty())
			return *begin;

		bool equal = true;
		/*return (std::_Traits_compare<char>(begin, size(),
			comp.begin, comp.size()));*/
		_string::const_iterator it1, it2;
		for (it1 = begin, it2 = comp.begin; it1 != end && it2 != comp.end && equal; it1++, it2++)
		{
			if (*it1 != *it2) equal = false;
		}
		it1--, it2--;
		if (equal)
			return size() - comp.size();
		else
			return *it1 - *it2;
	}
	void rtrim()
	{
		bool stop = false;
		auto tmp = begin;
		while (!stop && begin != end)
		{
			switch (*tmp)
			{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				tmp++;
				break;
			default:
				stop = true;
			}
		}
		begin = tmp;
	}
	void ltrim()
	{
		bool stop = false;
		auto tmp = end - 1;
		while (!stop && begin != end)
		{
			switch (*tmp)
			{
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				tmp--;
				break;
			default:
				stop = true;
			}
		}
		end = tmp + 1;
	}
	void trim()
	{
		rtrim();
		ltrim();
	}
	bool empty() const
	{
		return begin == end;
	}
	_string str() const
	{
		return _string(begin, end);
	}
	typename _string::const_iterator begin, end;
};