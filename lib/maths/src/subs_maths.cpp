#include "exprtk.hpp"
#include <subs/maths/subs_maths.h>
#include <iomanip>
namespace subs
{
	using expression_t = typename exprtk::expression<double>;
	using parser_t = typename exprtk::parser<double>;	Maths::Maths()
	{
		
		
	}
	Maths::~Maths()
	{
		if (m_expression)
			delete reinterpret_cast<expression_t*>(m_expression);
		if (m_parser)
			delete reinterpret_cast<expression_t*>(m_parser);
	}
	std::string Maths::get() const
	{
		checkExpr();
		std::ostringstream ostr;
		ostr << std::setprecision(6) << reinterpret_cast<expression_t*>(m_expression)->value();
		return ostr.str();
	}
	bool Maths::ask() const
	{
		checkExpr();
		return reinterpret_cast<expression_t*>(m_expression)->value()!=0.;
	}
	void Maths::checkExpr() const
	{
		if (!m_expression)
			m_expression = new expression_t;
		if (!m_parser)
			m_parser = new parser_t;


		std::string expr = m_arguments[0]->get();
		size_t currentHash = std::hash<std::string>()(expr);
		if (currentHash != m_hashExpr)
		{
			reinterpret_cast<parser_t*>(m_parser)->compile(expr, *reinterpret_cast<expression_t*>(m_expression));
			//if (!parser.compile(expr, m_expression))
			//	//THROW
			m_hashExpr = currentHash;
		}
	}
}