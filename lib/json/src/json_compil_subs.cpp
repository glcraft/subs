#include <regex>
#include <sstream>
#include <subs/subs_config.h>
#ifdef USE_SUBS_JSON
#include <subs/container/json_compil_subs.hpp>
namespace subs
{
    JsonContainer::~JsonContainer()
    {

    }
    Item::shared_ptr JsonContainer::getItem(biIterator name)
    {
        if (m_items.find(name) != m_items.end())
            return m_items.at(name);
        else
            return addValue(name);
    }
    Item::shared_ptr JsonContainer::getItem(biIterator name) const
    {
        if (m_items.find(name) != m_items.end())
            return m_items.at(name);
        else
            return Item::shared_ptr();
    }
    void JsonContainer::set(type cont)
    {
        m_container = cont;
        updateItems();
    }
    void JsonContainer::updateItems()
    {
        for(auto & item : m_items)
            reinterpret_cast<JsonItem*>(item.second.get())->set(searchJson(item.first));
    }
    JsonItem::shared_ptr JsonContainer::addValue(biIterator path)
    {
        JsonItem::shared_ptr current_item = std::make_shared<JsonItem>(searchJson(path), path);
        m_items[path] = current_item;
        return current_item;
    }
    nlohmann::json JsonContainer::searchJson(biIterator path) const
    {
        const nlohmann::json* j = &m_container;
        JsonItem::shared_ptr current_item;
        const std::regex name_search("^(?:(\\w+)|(?:\\[(\\d+)\\]))(\\.)?");
        std::smatch sm;
        while (std::regex_search(path.begin, path.end, sm, name_search))
        {
            auto new_j = j->end();
            if (sm[1].matched)
            {
                new_j = j->find(sm[1]);
            }
            else if (sm[2].matched)
            {
                size_t i = stoull(sm[2]);
                if (j->is_array() && i<j->size())
                    new_j=j->begin()+i;
            }
            if (new_j==j->end())
                return nlohmann::json();
            j=&*new_j;
            path.begin = sm[0].second;
        }
        return *j;
    }
    JsonItem::JsonItem(nlohmann::json obj, std::string path)
    {
        set(obj, path);
    }
    std::string JsonItem::get() const
    {
        if (m_item.is_string())
            return m_item.get<std::string>();
        std::ostringstream ostr;
        ostr << m_item;
        return ostr.str();
    }
    bool JsonItem::exists() const
    {
        return !m_item.is_null();
    }
    void JsonItem::set(const nlohmann::json& obj, std::string path)
    {
        m_item = obj;
        if (!path.empty())
            m_path = path;
    }
} // namespace subs
#endif