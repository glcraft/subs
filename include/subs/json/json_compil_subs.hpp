#include <subs/core/subs_compile.h>
#include <nlohmann/json.hpp>
namespace subs
{
    class JsonContainer : public Container
    {
    public:
        virtual ~JsonContainer();
		virtual Item::shared_ptr getItem(biIterator) const = 0;
    private:
    };
    class JsonItem : public Item
    {
    public:
    private:
    };
}