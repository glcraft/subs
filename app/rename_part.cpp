#include "common.h"
#include <string_view>
#include <filesystem>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;
void rename_mode(const std::string_view& input, const std::string_view& subs_out)
{
    subs::Compile subs_compiler;
    std::shared_ptr<MyRegexContainer> regCont = std::make_shared<MyRegexContainer>();
    subs_compiler.setContainer(regCont);
    subs_compiler.addModuleFactory("fparser", std::make_shared<FactoryType<subs::FParser>>());
    auto subs_object = subs_compiler.parse(std::string(subs_out));


    fs::path current_dir = fs::current_path();
    std::cout << "Current directory     " << current_dir << std::endl;
    std::cout << "Files" << std::endl;
    std::regex rg(input.begin(), input.end());
    uint64_t nfile=1;
    std::map<fs::path, fs::path> transFiles;
    for (auto& file : fs::directory_iterator(current_dir))
    {
        if (file.is_directory())
            continue;
        fs::path fsfilepath = file.path().parent_path();
        fs::path fsfilename = file.path().filename();
        std::string filename = fsfilename.string();
        regCont->setLine(nfile);
        regCont->setFilename(filename);
        std::smatch sm;
        if (std::regex_match(filename, sm, rg))
        {
            regCont->set(sm);
            fs::path newFilename(subs_object->get());
            std::cout << fsfilename << " >> " << newFilename << std::endl;
            transFiles[file.path()] = fsfilepath/newFilename;
        }
        nfile++;
    }
    std::string rep;
    bool ok=true;
    while (true)
    {
        std::cout << "Confirm ? (Y/n) ";
        getline(std::cin, rep);
        if (rep.empty() || rep=="Y" || rep=="y")
            break;
        else if (rep=="N" || rep=="n")
        {
            ok=false;
            break;
        }
    }
    if (ok)
    {
        for (auto& [from, to] : transFiles)
        {
            try
            {
                fs::rename(from, to);
            }
            catch(const std::filesystem::filesystem_error& e)
            {
                std::cerr << "Error moving file \"" << from << "\" to \"" << to << "\" : " << e.what() << '\n';
            }
        }
    }
}