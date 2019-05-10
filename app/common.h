#include <subs/maths/subs_fparser.h>
#include <subs/maths/subs_exprtk.h>
#include <subs/container/vector_compil_subs.hpp>
#ifdef WIN32
#include <Windows.h>
#endif
template <typename T>
class FactoryType : public subs::Factory
{
public:
    virtual std::unique_ptr<subs::Function> make_function()
    {
        return std::make_unique<T>();
    }
    virtual std::unique_ptr<subs::condition::Question> make_question()
    {
        return std::make_unique<T>();
    }
};

class MyRegexLine : public subs::Item
{
public:
    void set(uint64_t nline);
    virtual std::string get() const;
    virtual bool exists() const;
protected:
    std::string m_nline;
};
class MyRegexFilename : public subs::Item
{
public:
    void set(std::string filename);
    virtual std::string get() const;
    virtual bool exists() const;
protected:
    std::string m_filename;
};

class MyRegexContainer : public subs::RegexContainer
{
public:
    MyRegexContainer();
    virtual subs::Item::shared_ptr getItem(biIterator name) const ;
    virtual subs::Item::shared_ptr getItem(biIterator name);

    void setLine(uint64_t nline);
    void setFilename(std::string filename);
protected:
    std::shared_ptr<MyRegexLine> m_reglineItem;
    std::shared_ptr<MyRegexFilename> m_regfnItem;
};
template <class string_from_t, class string_to_t>
string_to_t convert_string(const string_from_t&);
