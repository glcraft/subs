#pragma once
#include <subs/core/BiIterator.hpp>
#include <list>
#include <map>
#include <subs/core/subs_object.h>
namespace subs
{
	void Init();
	class Collection : public Object
	{
	public:
		Collection() {};
		Collection(const Collection&) = delete;
		virtual std::string get() const;
		void push_front(Object::ptr);
		void push_back(Object::ptr);
		Object::ptr pop_front();
		Object::ptr pop_back();
		size_t size();
	protected:
		std::list<Object::ptr> m_list;
	};
	class Text : public Object
	{
	public:
		Text();
		Text(std::string);
		Text(const Text&);
		Text(Text&&);
		void swap(Text&);
		virtual std::string get() const;
	protected:
		std::string m_value;
	};
	class Function : public Object, public ArgsSet
	{
	public:
		static void Init();
		static void AddFunction(std::string name, function_t func);
		static void DeleteFunction(std::string name);
		static const function_t& GetFunction(std::string name);
		static std::vector<std::string> ListFunctions();
	private:
		static std::map<std::string /*func_name*/, function_t /*func*/> m_functions;
	};
	class FunctionDynamic : public Function
	{
	public:
		FunctionDynamic(Object::ptr funcname);
		virtual std::string get() const;
		void setName(Object::ptr name);
	protected:
		Object::ptr m_name;
	};
	class FunctionStatic : public Function
	{
	public:
		FunctionStatic(function_t func);
		virtual std::string get() const;
		void setFunction(function_t func);
	protected:
		function_t m_function;
	};
	
	template <typename Type>
	class SharedRef : public Object
	{
	public:
		SharedRef()
		{}
		SharedRef(std::shared_ptr<Type> obj) : m_shptr(obj)
		{}
		void setRef(std::shared_ptr<Type> obj)
		{
			m_shptr = obj;
		}
		virtual std::string get() const
		{
			return m_shptr ? m_shptr->get() : "";
		}
	private:
		std::shared_ptr<Type> m_shptr;
	};
	namespace parse_helper
	{
		//Subs
		biIterator getVariable(biIterator range);
		biIterator getBlock(biIterator range);
		std::list<biIterator> getArguments(biIterator& range);
		//Conditionnal
		biIterator getSubCondition(biIterator range);
	}
	class Compile
	{
	public:
		Object::ptr parse(biIterator format);
		Object::ptr parse(const std::string& format);
		void setContainer(std::shared_ptr<Container> cont);
	protected:
		void make_Text(std::unique_ptr<Collection>& list, biIterator& format, std::string::const_iterator it);

		Object::ptr genBlock(biIterator range);
		Object::ptr genFunctions(biIterator range);
		Object::ptr genConditional(biIterator condition, biIterator pass, biIterator fail);

		//std::unique_ptr<Collection> m_list;
		std::shared_ptr<Container> m_container;
	};
}