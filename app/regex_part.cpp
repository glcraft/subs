#include "common.h"
#include <regex>
#include <iostream>
#include <string_view>
#include <fstream>
#include <subs/container/vector_compil_subs.hpp>

void regex_mode(const std::string_view& input, const std::string_view& subs_out, const std::list<std::string_view>& files)
{
    std::regex rgxp(input.data(), input.length());
    subs::Compile subs_compiler;
    std::shared_ptr<MyRegexContainer> regCont = std::make_shared<MyRegexContainer>();
    subs_compiler.setContainer(regCont);
    subs_compiler.addModuleFactory("fparser", std::make_shared<FactoryType<subs::FParser>>());
    auto subs_object = subs_compiler.parse(std::string(subs_out));
    #ifdef WIN32
    HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    ZeroMemory(&screenInfo, sizeof(screenInfo));
    GetConsoleScreenBufferInfo(consolehwnd, &screenInfo);
    
    const int red=FOREGROUND_RED|FOREGROUND_INTENSITY, def=screenInfo.wAttributes;
    #else
    constexpr const char* red="\033[1;31m", def = "\033[1;39m";
    #endif
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
#ifdef WIN32
                SetConsoleTextAttribute(consolehwnd, red);
                std::cout << subs_object->get() << std::flush;
                SetConsoleTextAttribute(consolehwnd, def);
#else
                std::cout << red << subs_object->get() << def;
#endif
                begin=sm.suffix().first;
            } 
            std::cout << std::string(begin, end) << std::endl;
        }
    }
}