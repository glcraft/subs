#pragma once
#include <subs/core/subs_compile.h>
#include <functional>
#include <map>
namespace subs
{
	class Container;
	namespace condition
	{
		using condition_t = std::function<bool(const arguments_t&)>;

		class Question
		{
		public:
			PTR_DEF(Question);
			virtual bool ask() const = 0;
		};

		class Condition : public Question, public ArgsSet //Pour le moment, uniquement statique
		{
		public:
			PTR_DEF(Condition);

			static void Init();
			static void AddCondition(std::string name, condition_t func);
			static void DeleteCondition(std::string name);
			static const condition_t& GetCondition(std::string name);
			static std::vector<std::string> ListCondition();

			void setCondition(condition_t cond);
			bool ask() const;
		protected:
			condition_t m_condition;
		private:
			static std::map<std::string /*func_name*/, condition_t /*func*/> m_conditions;
		};
		
		using operator_t = std::function<bool(bool, bool)>;

		class Operator : public Question
		{
		public:
			PTR_DEF(Operator);

			void setOperator(operator_t op);
			void setConditions(Question::ptr, Question::ptr);
			bool ask() const;
		protected:
			operator_t m_op;
			Question::ptr m_conditions[2];
		};

		class Existance : public Question
		{
		public:
			PTR_DEF(Existance);

			void setItem(Item::shared_ptr item);
			bool ask() const;
		protected:
			Item::shared_ptr m_item;
		};
		
		class Parse
		{
		public:
			Parse();
			Parse(std::shared_ptr<Container> cont);

			Question::ptr parse(biIterator format);
			Question::ptr parse(const std::string& format);
			void setContainer(std::shared_ptr<Container> cont);
			void setSubsParser(subs::Compile* subsparser);
		private:
			
			std::shared_ptr<Container> m_container;
			subs::Compile* m_subsParser = nullptr;
		};
	}
	class Conditional : public Object
	{
	public:
		virtual std::string get() const;
		void setQuestion(condition::Question::ptr);
		void setPassObject(Object::ptr pass);
		void setFailObject(Object::ptr fail);
	protected:
		condition::Question::ptr m_question;
		Object::ptr m_pass, m_fail;
	};
	
}
