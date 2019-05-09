#include <subs/subs_config.h>
#if USE_SUBS_JSON
#include <subs/core/subs_compile.h>
#include <nlohmann/json.hpp>
namespace subs
{
    class JsonItem : public Item
    {
    public:
        JsonItem(nlohmann::json obj, std::string path);
        virtual std::string get() const;
		virtual bool exists() const;
        const std::string& getPath();
    private:
        friend class JsonContainer;
		virtual void set(const nlohmann::json& obj, std::string path=std::string());
        nlohmann::json m_item;
        std::string m_path;
    };
    class JsonContainer : public Container
    {
    public:
        using type = nlohmann::json;
        virtual ~JsonContainer();
		virtual Item::shared_ptr getItem(biIterator name);
		virtual Item::shared_ptr getItem(biIterator name) const;
        void set(type cont);
    private:
        void updateItems();
        JsonItem::shared_ptr addValue(biIterator name);
        nlohmann::json searchJson(biIterator) const;
        nlohmann::json m_container;
        mutable std::map<std::string, JsonItem::shared_ptr> m_items;
    };
}
#else
#	pragma message ( "Subs : Caution. nlohmann::json module is not integrated." )
#endif