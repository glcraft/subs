#include "common.h"
#include <subs/core/Lambda.hpp>

void nothing_mode(const std::string_view& subs_out)
{
    subs::Compile subs_compiler;
#if USE_SUBS_FPARSER
    subs_compiler.addModuleFactory("fparser", std::make_shared<FactoryType<subs::FParser>>());
#endif
#if USE_SUBS_EXPRTK
    subs_compiler.addModuleFactory("exprtk", std::make_shared<FactoryType<subs::ExprTk>>());
#endif
    auto subs_object = subs_compiler.parse(std::string(subs_out));
    std::cout << subs_object->get() << std::endl;
}