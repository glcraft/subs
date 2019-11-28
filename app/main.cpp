#include <iostream>
#include <exception>
#include <regex>
#include <string>
#include <string_view>
#include <fstream>

#include "common.h"
#include "subs/core/subs_conditional.h"
#include "nlohmann/json.hpp"


void rename_mode(const std::string_view& input, const std::string_view& subs_out);

void regex_mode(const std::string_view& input, const std::string_view& subs_out, const std::list<std::string_view>& files);

void json_mode(const std::string_view& input, const std::string_view& subs_out, const std::list<std::string_view>& files);

void print_help()
{
    std::cout << "Subs application " SUBS_VERSION R"(
subs <input_method> <input> <subs> [options...] [files...]
Input methods :
    help        display this.
    regex       takes regex in input. subs variables refer 
                to the regex position (ex: $1;)
    rename      rename files using regex research.
    json        takes json syntaxe. subs variables is a 
                json path (ex: $ob1.ob2[3].string;)
                Note : if file(s) is specified, 'input' 
                doesn't count, and vice versa...

Rename options
    -R, --recursive     go through subdirectories

Regex subs available special variables
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
    const char* what() const noexcept override {return m_what.c_str();};
private:
    std::string m_what;
};

int real_main (int argc, char** argv)
{
    if (argc<4)
        throw AppException("Arguments is missing.");
    std::list<std::string_view> listFiles;
    std::string_view inputMethod(argv[1]), input(argv[2]), subs_output(argv[3]);

    subs::Function::Init();
    subs::condition::Condition::Init();
    
    for (int i=4;i<argc;++i)
        listFiles.emplace_back(argv[i]);

    subs::Container::ptr m_cont;
    if (inputMethod=="help")
        print_help();
    else if (inputMethod=="regex")
        regex_mode(input, subs_output, listFiles);
    else if (inputMethod=="rename")
        rename_mode(input, subs_output);
    else if (inputMethod=="json")
        json_mode(input, subs_output, listFiles);
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
        std::cerr << "std::regex_error thrown\n";
        std::cerr << e.what() << " (code "<<e.code()<<")\n";
        print_help();
        return 1;
    }
    catch(const nlohmann::json::exception& e)
    {
        std::cerr << "nlohmann::json::exception thrown\n";
        std::cerr << e.what() << std::endl;
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

