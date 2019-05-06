#pragma once
#include <subs/core/BiIterator.hpp>
#include <vector>
#include <map>
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
}