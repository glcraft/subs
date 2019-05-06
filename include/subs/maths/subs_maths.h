#pragma once
#include <subs/core/subs_libinit.h>
#include <subs/core/subs_compile.h>
#include <subs/core/subs_conditional.h>
namespace subs
{
	class SUBSMATH_API Maths : public Function, public condition::Question
	{
	public:
		Maths();
		Maths(const Maths&) = default;
		Maths(Maths&&) = default;
		~Maths();
		virtual std::string get() const;
		virtual bool ask() const;
		//void setExpression(Object::ptr expr);
	
		//using symbol_table_t = exprtk::symbol_table<double>; // NO VARIABLE
	protected:
		void checkExpr() const;
		mutable size_t m_hashExpr;
		Object::ptr m_expr;
	private:
		mutable void* m_expression = nullptr;
		mutable void* m_parser = nullptr;
	};
}