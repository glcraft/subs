#include <subs/maths/subs_fparser.h>
#include <iomanip>
namespace subs
{
    FParser::FParser()
	{
		
		
	}
	FParser::~FParser()
	{
        
	}
	std::string FParser::get() const
	{
		checkExpr();
        constexpr const double d=0.f;
		std::ostringstream ostr;
		ostr << std::setprecision(6) << fparser.Eval(&d);
		return ostr.str();
	}
	bool FParser::ask() const
	{
		checkExpr();
        constexpr const double d=0.f;
		return fparser.Eval(&d)!=0.;
	}
	void FParser::checkExpr() const
	{
        std::string expr = m_arguments[0]->get();
        size_t currentHash = std::hash<std::string>()(expr);
		if (currentHash != m_hashExpr)
        {
            fparser.Parse(expr, "__internal__");
            m_hashExpr = currentHash;
        }
	}
}
    