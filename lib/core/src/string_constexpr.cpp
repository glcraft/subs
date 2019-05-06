
#include <subs/core/string_helper.h>
namespace strh
{
	StringComp::StringComp()
	{
	}

	StringComp::StringComp(const std::string & str, const std::wstring & wstr)
	{
		set(str, wstr);
	}

	StringComp::StringComp(const std::string & str)
	{
		m_str = str;
	}

	StringComp::StringComp(const std::wstring & wstr)
	{
		m_wstr = wstr;
	}

	void StringComp::set(const std::string & str, const std::wstring & wstr)
	{
		m_str = str;
		m_wstr = wstr;
	}

	bool StringComp::operator<(const std::string & str)
	{
		return m_str < str;
	}
	bool StringComp::operator<(const std::wstring & wstr)
	{
		return m_wstr < wstr;
	}
}