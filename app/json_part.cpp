#include "common.h"
#include <string_view>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <subs/container/json_compil_subs.hpp>


void json_mode(const std::string_view& input, const std::string_view& subs_out, const std::list<std::string_view>& files)
{
    using json = nlohmann::json;
    
    
    subs::Compile subs_compiler;
    std::shared_ptr<subs::JsonContainer> jCont = std::make_shared<subs::JsonContainer>();
    
    subs_compiler.setContainer(jCont);
#if USE_SUBS_FPARSER
    subs_compiler.addModuleFactory("fparser", std::make_shared<FactoryType<subs::FParser>>());
#endif
#if USE_SUBS_EXPRTK
    subs_compiler.addModuleFactory("exprtk", std::make_shared<FactoryType<subs::ExprTk>>());
#endif
    auto subs_object = subs_compiler.parse(std::string(subs_out));
    if (files.empty())
    {
        json jsonObj = json::parse(input.data(), input.data()+input.length());
        jCont->set(jsonObj);
        std::cout << subs_object->get();
    }
    else
    {
        for(auto& file : files)
        {
            std::ifstream t(file.data());
            if (!t)
            {
                std::cerr << file << " isn't accessible. continue..." << std::endl;
                continue;
            }
            std::string str((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());

            json jsonObj;
            jsonObj.parse(str);
            jCont->set(jsonObj);
            std::cout << subs_object->get();
        }
    }
    
}