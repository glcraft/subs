#include <subs/maths/subs_fparser.h>
#include <subs/maths/subs_exprtk.h>

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
