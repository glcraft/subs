
#include <subs/core/Lambda.hpp>
#include <subs/core/vector_inter_subs.hpp>
namespace subs
{
	std::map<std::basic_string<char>, Lambda> Lambda::m_lambdas;
	std::unordered_map<std::basic_string<char>, Lambda2> Lambda2::m_lambdas;

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

	Lambda2::Lambda2()
	{
		
	}
	Lambda2::~Lambda2()
	{
		
	}
	std::string Lambda2::get() const
	{
		std::vector<std::string> vec_args;
		vec_args.reserve(m_arguments.size());
		for(auto &arg : m_arguments) {
			vec_args.push_back(arg->get());
		}
		m_args_lambdas->set(std::move(vec_args));
		return m_obj->get();
	}
	std::string Lambda2::get_from_object(const subs::arguments_t& arguments) const
	{
		std::vector<std::string> vec_args;
		vec_args.reserve(arguments.size());
		for(auto &arg : arguments) {
			vec_args.push_back(arg->get());
		}
		m_args_lambdas->set(std::move(vec_args));
		return m_obj->get();
	}
	void Lambda2::create_lambda(std::string name, std::string lbd)
	{
		Lambda2 new_lambda;
		new_lambda.m_args_lambdas = std::make_shared<VectorContainer>();
		subs::Compile compiler = Compile::Init(new_lambda.m_args_lambdas);
		
		new_lambda.m_obj = compiler.parse(lbd);
		m_lambdas.emplace(name, std::move(new_lambda));
	}
	bool Lambda2::Exists(std::string name)
	{
		return m_lambdas.find(name) != m_lambdas.end();
	}
	std::unique_ptr<Lambda2Object> Lambda2::Get(std::string name)
	{
		return std::make_unique<Lambda2Object>(m_lambdas.at(name));
	}
	Lambda2Object::Lambda2Object(Lambda2& l) : m_lambda(l){}
	std::string Lambda2Object::get() const {
		return m_lambda.get_from_object(m_arguments);
	}
}