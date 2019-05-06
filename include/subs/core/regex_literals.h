#pragma once
#include <regex>
#include <string>
// Match result with string inside
template <typename CharT>
struct match_results_ws: public std::match_results<typename std::basic_string<CharT>::const_iterator>
{
public:
	using _string = std::basic_string<CharT>;
	match_results_ws() : std::match_results<typename _string::const_iterator>() {}
	match_results_ws(_string _s) : std::match_results<typename _string::const_iterator>(), s(_s) {}
	_string& str()
	{return s;}
	const _string& str() const
	{return s;}
protected:
	_string s;
};
template <typename CharT>
struct struct_rm : public std::basic_regex<CharT>
{
    using _string = std::basic_string<CharT>;
    typedef typename _string::const_iterator const_iterator; 
    typedef std::match_results<const CharT *> _cmatch;
	typedef std::match_results<const_iterator > _smatch;
	typedef match_results_ws<CharT> _smatch_ws;
	
    struct_rm(_string str)
    {
        this->assign(str);
    }
    struct_rm(const CharT *_Str, size_t _Len)
    {
        this->assign(_Str, _Len, std::regex_constants::ECMAScript);
    }

	// Deleted method
	// To get the match results, we have to get the string reference !
	// Use match_results_ws instead to get the string into it
	inline bool match(_string&& str, _smatch& sm) = delete;
	// Deleted method
	// To get the match results, we have to get the string reference !
	// Use match_results_ws instead to get the string into it.
	inline bool search(_string&& str, _smatch& sm) = delete;

	
	inline bool match(_string&& str, _smatch_ws& sm)
	{
		sm.str().swap(str);
		return std::regex_match(sm.str(), sm, *this);
	}
	inline bool search(_string&& str, _smatch_ws& sm)
	{
		sm.str().swap(str);
		return std::regex_search(sm.str(), sm, *this);
	}
	inline bool match(const CharT* str, _cmatch& sm)
	{
		return std::regex_match(str, sm, *this);
	}
    inline bool match(const _string& str, _smatch& sm)
    {
        return std::regex_match(str, sm, *this);
    }
	inline bool match(const CharT* str)
	{
		return std::regex_match(str, *this);
	}
	inline bool match(const _string& str)
	{
		return std::regex_match(str, *this);
	}
    inline bool match(_string&& str)
    {
        return std::regex_match(str, *this);
    }
	inline bool search(const CharT* str, _cmatch& sm)
	{
		return std::regex_search(str, sm, *this);
	}
    inline bool search(const _string& str, _smatch& sm)
    {
        return std::regex_search(str, sm, *this);
    }
	inline bool search(const CharT* str)
	{
		return std::regex_search(str, *this);
	}
	inline bool search(const _string& str)
	{
		return std::regex_search(str, *this);
	}
    inline bool search(_string&& str)
    {
        return std::regex_search(str, *this);
    }
};
inline namespace literals
{
    inline namespace regex_literals
    {
        inline struct_rm<char> operator "" _rg(const char *_Str, size_t _Len)
        {	// construct literal from [_Str, _Str + _Len)
            return (struct_rm<char>(_Str, _Len));
        }
		inline struct_rm<wchar_t> operator "" _rg(const wchar_t *_Str, size_t _Len)
        {	// construct literal from [_Str, _Str + _Len)
            return (struct_rm<wchar_t>(_Str, _Len));
        }
    }
}