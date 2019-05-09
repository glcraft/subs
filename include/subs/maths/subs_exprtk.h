#pragma once
#include <subs/subs_config.h>
#if USE_SUBS_EXPRTK
#include <subs/core/subs_libinit.h>
#include <subs/core/subs_compile.h>
#include <subs/core/subs_conditional.h>
namespace subs
{
	class SUBSMATH_API ExprTk : public Function, public condition::Question
	{
	public:
		ExprTk();
		ExprTk(const ExprTk&) = default;
		ExprTk(ExprTk&&) = default;
		~ExprTk();
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
#else
#	pragma message ( "Subs : Caution. ExprTk module is not integrated." )
#endif