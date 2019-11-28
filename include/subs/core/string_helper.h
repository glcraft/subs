#pragma once
#include <string>

#define TOKENS "{}$;(),?:\\^&|"
#define STR(x) x
#define WSTR(x) L#x
#define U16STR(x) u#x
#define U32STR(x) U#x

#define MAKE_STRCOMP(x) { ##x, L##x }

//string helper
namespace strh
{
	constexpr int _getId(char i, int pos = 0)
	{
		return TOKENS[pos] == i ? pos : _getId(i, pos + 1);
	}

	template <typename Char> constexpr Char getChar(char c) 
	{ 
		return "{}$;(),?:\\^&|"[_getId(c)];
	}
	template <typename char_t>
	std::basic_string<char_t> trim(const std::basic_string<char_t>& in, const char_t* suppressor)
	{
		size_t i1 = in.find_first_not_of(suppressor);
		size_t i2 = in.find_last_not_of(suppressor);
		return in.substr(i1, i2-i1);
	}

	class StringComp
	{
	public:
		StringComp();
		StringComp(const std::string& str, const std::wstring& wstr);
		StringComp(const std::string& str);
		StringComp(const std::wstring& wstr);
		void set(const std::string& str, const std::wstring& wstr);
		operator std::string() const 
		{
			return m_str;
		}
		operator std::wstring() const
		{
			return m_wstr;
		}
		bool operator<(const std::string& str);
		bool operator<(const std::wstring& wstr);
	private:
		std::string m_str;
		std::wstring m_wstr;
	};

	//const static wchar_t i = getChar<wchar_t>('}');
}