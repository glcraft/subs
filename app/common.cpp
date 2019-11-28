#include "common.h"
#include <subs/maths/subs_fparser.h>
#include <subs/maths/subs_exprtk.h>

template <>
std::string convert_string<std::wstring, std::string>(const std::wstring & strFrom)
{
    std::string result;
#ifdef _WIN32
    BOOL usedDefChar=false;
    int nbch = WideCharToMultiByte(CP_ACP, 0, strFrom.c_str(), strFrom.length(), nullptr, 0, nullptr, &usedDefChar);
    result.resize(nbch, 0);
    WideCharToMultiByte(CP_ACP, 0, strFrom.c_str(), strFrom.length(), &result[0], nbch, nullptr, &usedDefChar);
#endif
    return result;
}

template <>
std::wstring convert_string<std::string, std::wstring>(const std::string & strFrom)
{
		std::wstring result;
#ifdef _WIN32
		int nbch = MultiByteToWideChar(CP_ACP, 0, strFrom.c_str(), strFrom.length(), nullptr, 0);
		result.resize(nbch, 0);
		MultiByteToWideChar(CP_ACP, 0, strFrom.c_str(), strFrom.length(), &result[0], nbch);
#endif
		return result;
}

void MyRegexLine::set(uint64_t nline)
{
    m_nline=std::to_string(nline);
}
std::string MyRegexLine::get() const
{
    return m_nline;
}
bool MyRegexLine::exists() const
{return true;}
void MyRegexFilename::set(std::string filename)
{
    m_filename=filename;
}
std::string MyRegexFilename::get() const
{
    return m_filename;
}
bool MyRegexFilename::exists() const
{return true;}
MyRegexContainer::MyRegexContainer()
{
    m_reglineItem = std::make_shared<MyRegexLine>();
    m_regfnItem = std::make_shared<MyRegexFilename>();
}
subs::Item::shared_ptr MyRegexContainer::getItem(biIterator name) const 
{
    if (name=="line")
        return m_reglineItem;
    else if (name=="filename")
        return m_regfnItem;
    else
        return subs::RegexContainer::getItem(name);
}
subs::Item::shared_ptr MyRegexContainer::getItem(biIterator name) 
{
    if (name=="line")
        return m_reglineItem;
    else if (name=="filename")
        return m_regfnItem;
    else
        return subs::RegexContainer::getItem(name);
}

void MyRegexContainer::setLine(uint64_t nline){m_reglineItem->set(nline);}
void MyRegexContainer::setFilename(std::string filename){m_regfnItem->set(filename);}