
#include <subs/core/Lambda.hpp>
#include <subs/core/vector_inter_subs.hpp>
namespace subs
{
	std::map<std::basic_string<char>, Lambda> Lambda::m_lambdas;

	Lambda::Lambda() : m_lambda()
	{
	}
	Lambda::Lambda(_string lbd) : m_lambda()
	{
		setLambda(lbd);
	}
	Lambda::Lambda(const Lambda & lbd)
	{
		m_lambda = lbd.m_lambda;
	}
	Lambda::Lambda(Lambda && lbd)
	{
		m_lambda.swap(lbd.m_lambda);
	}
	void Lambda::setLambda(_string lbd)
	{
		m_lambda.swap(lbd);
	}
	std::basic_string<char> Lambda::operator()(std::vector<std::string> args) const
	{
		integrator::VectorSubs biv;
		biv.setContainer(args);
		return biv.parse(m_lambda);
	}
	std::map<std::basic_string<char>, Lambda>& Lambda::Get()
	{
		return m_lambdas;
	}
}