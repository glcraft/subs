#pragma once
#include <subs/subs_config.h>
#if USE_SUBS_FPARSER
#include <fparser/fparser.hh>
#include <subs/core/subs_libinit.h>
#include <subs/core/subs_compile.h>
#include <subs/core/subs_conditional.h>
namespace subs
{
	class SUBSMATH_API FParser : public Function, public condition::Question
	{
	public:
		FParser();
		FParser(const FParser&) = default;
		FParser(FParser&&) = default;
		~FParser();
		virtual std::string get() const;
		virtual bool ask() const;
		//void setExpression(Object::ptr expr);
	
		//using symbol_table_t = exprtk::symbol_table<double>; // NO VARIABLE
	protected:
		void checkExpr() const;
		mutable size_t m_hashExpr;
		Object::ptr m_expr;
	private:
		mutable FunctionParser fparser;
	};
}
#else
#	pragma message ( "Subs : Caution. FParser module is not integrated." )
#endif