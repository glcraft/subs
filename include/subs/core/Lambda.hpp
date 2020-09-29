#pragma once
#include <subs/core/BiIterator.hpp>
#include <subs/container/vector_compil_subs.hpp>
#include <vector>
#include <map>
#include <unordered_map>
namespace subs
{
	class Lambda
	{
	public:
		typedef std::basic_string<char> _string;
		Lambda();
		Lambda(_string lbd);
		Lambda(const Lambda& lbd);
		Lambda(Lambda&& lbd);

		void setLambda(_string lbd);
		std::basic_string<char> operator()(std::vector<std::string> args) const;
		static std::map<std::basic_string<char>, Lambda>& Get();
	protected:
		static std::map<std::basic_string<char>, Lambda> m_lambdas;
		_string m_lambda;
	};
	class Lambda2Object;
	class Lambda2 : public Function
	{
	public:
		Lambda2();
		Lambda2(const Lambda2&) = default;
		Lambda2(Lambda2&&) = default;
		~Lambda2();
		virtual std::string get() const;
		std::string get_from_object(const subs::arguments_t&) const;
		static void create_lambda(std::string name, std::string lbd);
		static bool Exists(std::string name);
		static std::unique_ptr<Lambda2Object> Get(std::string name);
	protected:
		static std::unordered_map<std::basic_string<char>, Lambda2> m_lambdas;
		std::shared_ptr<VectorContainer> m_args_lambdas;
		Object::ptr m_obj;
	};
	class Lambda2Object : public Function
	{
	public:
		Lambda2Object(Lambda2&);
		virtual std::string get() const;
	protected:
		Lambda2& m_lambda;
	};
	class Lambda2Factory : Factory
	{
		virtual std::unique_ptr<Function> make_function();
	};
}