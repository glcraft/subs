#include <iostream>
#include <exception>
#include <regex>
#include <string>
#include <string_view>
#include <fstream>

#include <subs/core/vector_compil_subs.hpp>
#include <subs/maths/subs_maths.h>

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
    subs_compiler.addModuleFactory("maths", std::make_shared<FactoryType<subs::Maths>>());
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


void print_help()
{
    std::cout << R"(Subs application 1.1.0
subs <input_method> <input> <subs> [options...] [files...]
Input methods :
    help        display this.
    regex       takes regex in input. subs variables refer 
                to the regex position (ex: $1;)
    json        takes json syntaxe. subs variables is a 
                json path (ex: $ob1.ob2[3].string;)
    json_file   takes json file. Same syntax as `json`

Regex available special variables
    size        number of regex captures
    line        line number
    filename    filename (same as files arguments)
)";
}
class AppException : public std::exception
{
public:
    AppException() : m_what(){}
    AppException(std::string str) : m_what(std::move(str)){}
    const char* what() const{return m_what.c_str();};
private:
    std::string m_what;
};

int real_main (int argc, char** argv)
{
    if (argc<5)
        throw AppException("Arguments is missing.");
    std::list<std::string_view> listFiles;
    std::string_view inputMethod(argv[1]), input(argv[2]), subs_output(argv[3]);
    
    for (int i=4;i<argc;++i)
        listFiles.emplace_back(argv[i]);

    subs::Container::ptr m_cont;
    if (inputMethod=="help")
        print_help();
    else if (inputMethod=="regex")
        regex_mode(input, subs_output, listFiles);
    else if (inputMethod=="json")
        throw AppException("Json en developpement.");
    else if (inputMethod=="json_file")
        throw AppException("Json en developpement.");
    return EXIT_SUCCESS;
}

int main (int argc, char** argv)
{
    try
    {
        return real_main(argc, argv);
    }
    catch(const AppException& e)
    {
        std::cerr << e.what() << '\n';
        print_help();
        return 1;
    }
    catch(const std::regex_error& e)
    {
        std::cerr << e.what() << " (code "<<e.code()<<")\n";
        print_help();
        return 1;
    }
    catch(const std::exception& e)
    {
        std::cerr << "std::exception thrown\n";
        std::cerr << e.what() << '\n';
        return 2;
    }
    return 0;
}

