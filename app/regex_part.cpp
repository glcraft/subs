#include "common.h"
#include <regex>
#include <iostream>
#include <string_view>
#include <fstream>
#include <subs/container/vector_compil_subs.hpp>

class MyRegexLine : public subs::Item
{
public:
    void set(uint64_t nline)
    {
        m_nline=std::to_string(nline);
    }
    virtual std::string get() const
    {
        return m_nline;
    }
    virtual bool exists() const
    {return true;}
protected:
    std::string m_nline;
};

class MyRegexFilename : public subs::Item
{
public:
    void set(std::string filename)
    {
        m_filename=filename;
    }
    virtual std::string get() const
    {
        return m_filename;
    }
    virtual bool exists() const
    {return true;}
protected:
    std::string m_filename;
};

class MyRegexContainer : public subs::RegexContainer
{
public:
    MyRegexContainer()
    {
        m_reglineItem = std::make_shared<MyRegexLine>();
        m_regfnItem = std::make_shared<MyRegexFilename>();
    }
    virtual subs::Item::shared_ptr getItem(biIterator name) const 
    {
        if (name=="line")
            return m_reglineItem;
        else if (name=="filename")
            return m_regfnItem;
        else
            return subs::RegexContainer::getItem(name);
    }

    void setLine(uint64_t nline){m_reglineItem->set(nline);}
    void setFilename(std::string filename){m_regfnItem->set(filename);}
protected:
    std::shared_ptr<MyRegexLine> m_reglineItem;
    std::shared_ptr<MyRegexFilename> m_regfnItem;
};
void regex_mode(const std::string_view& input, const std::string_view& subs_out, const std::list<std::string_view>& files)
{
    std::regex rgxp(input.data(), input.length());
    subs::Compile subs_compiler;
    std::shared_ptr<MyRegexContainer> regCont = std::make_shared<MyRegexContainer>();
    subs_compiler.setContainer(regCont);
    subs_compiler.addModuleFactory("fparser", std::make_shared<FactoryType<subs::FParser>>());
    auto subs_object = subs_compiler.parse(std::string(subs_out));
    
    for (auto& filename : files)
    {
        std::ifstream file(filename.data());
        
        if (!file)
        {
            std::cerr << filename << " isn't accessible. continue..." << std::endl;
            continue;
        }
        std::string line;
        const std::string& cline = line;
        uint64_t nline=0;
        regCont->setFilename(filename.data());
        while (std::getline(file, line))
        {
            regCont->setLine(++nline);
            std::smatch sm;
            std::string::const_iterator begin = line.begin(), end = line.end();
            #ifdef _DEBUG
                std::cout << "subsout: ";
            #endif
            while(std::regex_search(begin, end, sm, rgxp))
            {
                regCont->set(sm);
                if (sm.prefix().matched)
                    std::cout << sm.prefix();
                std::cout << subs_object->get();
                begin=sm.suffix().first;
            } 
            std::cout << std::string(begin, end) << std::endl;
        }
    }
}