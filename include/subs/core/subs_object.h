#pragma once
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <subs/core/BiIterator.hpp>

#define PTR_DEF(type) using ptr = std::unique_ptr<type>;
#define SHPTR_DEF(type) using shared_ptr = std::shared_ptr<type>;
namespace subs
{
	class Object
	{
	public:
		PTR_DEF(Object);
		virtual std::string get() const = 0;
	};
	class Item : public Object
	{
	public:
		SHPTR_DEF(Item);
		virtual std::string get() const = 0;
		virtual bool exists() const = 0;
	};
	class Container
	{
	public:
		PTR_DEF(Container);
		SHPTR_DEF(Container);

		virtual ~Container();

		//virtual bool exists(biIterator) const = 0;
		virtual Item::shared_ptr getItem(biIterator) const = 0;
	};
	
	using arguments_t = std::vector<Object::ptr>;
	using function_t = std::function<std::string(const arguments_t&)>;

	class ArgsSet
	{
	public:
		void addArgument(Object::ptr arg);
		void clearArguments();
	protected:
		arguments_t m_arguments;
	};
}