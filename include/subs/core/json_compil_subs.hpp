#pragma once
#include <subs/core/subs_compile.h>
#include <json/json.h>
namespace subs
{
	class JsonItem : public Item
	{
	public:
		SHPTR_DEF(JsonItem)
		virtual std::string get() const;
		virtual bool exists() const;
		
		void set(const Json::Value& cont);
		void set(const Json::Value& cont, biIterator path);
	protected:
		struct JsonPathHelper
		{
			std::string jsname;
			uint32_t index=0;
			enum Type {
				IsValue,
				IsArray
			} type;
			const Json::Value& getFrom(const Json::Value&);
		};
		void parse(biIterator);

		Json::Value m_item;
		std::list<JsonPathHelper> m_path;
	};

	class JsonContainer : public Container
	{
	public:
		typedef Json::Value type;

		void set(Json::Value value);
		virtual Item::shared_ptr getItem(biIterator) const;
	protected:
		void updateItems();
		mutable std::map<std::string, JsonItem::shared_ptr> m_items;
		type m_cont;
	};
}