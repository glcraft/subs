
#include <subs/core/json_compil_subs.hpp>
namespace subs
{
	using namespace std;
	using namespace std::string_literals;
	
	std::string JsonItem::get() const
	{
		if (!m_item || m_item.type() == Json::ValueType::nullValue)
			return ""s;
		if (m_item.type() == Json::ValueType::stringValue)
			return m_item.asString();
		std::ostringstream ostr;
		ostr << m_item;
		return ostr.str();
	}
	void JsonContainer::set(Json::Value value)
	{
		m_cont = value;
		updateItems();
	}
	Item::shared_ptr JsonContainer::getItem(biIterator name) const
	{
		auto found = m_items.find(name.str());
		if (found != m_items.end())
			return found->second;
		else
		{
			std::shared_ptr<JsonItem> itm = make_shared<JsonItem>();
			itm->set(m_cont, name);
			m_items[name]=itm;
			return std::move(itm);
		}
	}
	void JsonContainer::updateItems()
	{
		for (auto item : m_items)
			item.second->set(m_cont);
	}

	bool JsonItem::exists() const
	{
		return (!!m_item);
	}

	void JsonItem::set(const Json::Value& cont)
	{
		const Json::Value* current = &cont;
		for (auto& pathname : m_path)
			current = &pathname.getFrom(*current);
		m_item = *current;
	}

	void JsonItem::set(const Json::Value& value, biIterator path)
	{
		parse(path);
		set(value);
	}

	void JsonItem::parse(biIterator path)
	{
		auto genPathHelper = [](biIterator name)
		{
			JsonPathHelper pathHelper;
			std::string foundName(name);
			std::smatch sm;
			if (R"((.+)\[(\d+)\])"_rg.match(foundName, sm))
			{
				pathHelper.jsname = sm[1];
				pathHelper.index = std::stoul(sm[2]);
				pathHelper.type = JsonPathHelper::IsArray;
			}
			else
			{
				pathHelper.jsname = foundName;
				pathHelper.type = JsonPathHelper::IsValue;
			}
			return pathHelper;
		};
		
		for (auto it = path.begin; it != path.end; it++)
		{
			if (*it == '.')
			{
				m_path.emplace_back(genPathHelper(biIterator(path.begin, it)));
				path.begin = std::next(it);
			}
		}
		if (!path.empty())
			m_path.emplace_back(genPathHelper(path));
	}

	
	const Json::Value& JsonItem::JsonPathHelper::getFrom(const Json::Value& val)
	{
		if (type == IsArray)
			return val[jsname][index];
		else
			return val[jsname];
	}
}