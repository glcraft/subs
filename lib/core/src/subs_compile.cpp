#include <sstream>
#include <subs/core/subs_compile.h>
#include <subs/core/subs_conditional.h>
// #include <subs/maths/subs_maths.h>
using namespace std;
namespace subs
{
	std::map<std::string /*func_name*/, function_t /*func*/> Function::m_functions;

	Text::Text()
	{

	}
	Text::Text(std::string str) : m_value(str)
	{

	}
	Text::Text(const Text& txt) : m_value(txt.m_value)
	{

	}
	Text::Text(Text&& txt)
	{
		swap(txt);
	}
	void Text::swap(Text& txt)
	{
		m_value.swap(txt.m_value);
	}
	std::string Text::get() const 
	{
		return m_value;
	}
	
	void Function::AddFunction(std::string name, function_t func)
	{
		m_functions[name].swap(func);
	}
	void Function::DeleteFunction(std::string name)
	{
		auto found = m_functions.find(name);
		if (found == m_functions.end())
			throw std::out_of_range("Function::DeleteFunction : name not found");
		m_functions.erase(found);
	}
	const function_t& Function::GetFunction(std::string name)
	{
		auto found=m_functions.find(name);
		if (found == m_functions.end())
			return m_functions["null"];
		return found->second;
	}
	std::vector<std::string> Function::ListFunctions()
	{
		std::vector<std::string> result;
		for (auto& func : m_functions)
			result.push_back(func.first);
		return result;
	}
	
	
	FunctionStatic::FunctionStatic(function_t func)
	{
		setFunction(func);
	}
	std::string FunctionStatic::get() const
	{
		if (!m_function)
			return "{Fonction inconnue}";
		return m_function(m_arguments);
	}
	void FunctionStatic::setFunction(function_t func)
	{
		m_function.swap(func);
	}
	FunctionDynamic::FunctionDynamic(Object::ptr funcname)
	{
		m_name.swap(funcname);
	}
	std::string FunctionDynamic::get() const
	{
		auto func = Function::GetFunction(m_name->get());
		return func(m_arguments);
	}
	void FunctionDynamic::setName(Object::ptr name)
	{
		m_name.swap(name);
	}
	std::string Collection::get() const
	{
		ostringstream ostr;
		for (auto& obj : m_list)
			ostr << (obj ? obj->get() : std::string());
		return ostr.str();
	}
	void Collection::push_front(Object::ptr obj)
	{
		m_list.push_front(std::move(obj));
	}
	void Collection::push_back(Object::ptr obj)
	{
		m_list.push_back(std::move(obj));
	}
	Object::ptr Collection::pop_front()
	{
		Object::ptr res = std::move(m_list.back());
		m_list.pop_front();
		return res;
	}
	Object::ptr Collection::pop_back()
	{
		Object::ptr res = std::move(m_list.back());
		m_list.pop_back();
		return res;
	}
	size_t Collection::size()
	{
		return m_list.size();
	}
	Container::~Container()
	{
	}
	Object::ptr Compile::parse(biIterator format)
	{
		using namespace parse_helper;
		std::unique_ptr<Collection> result = std::unique_ptr<Collection>(new Collection());
		biIterator currentSeq;
		currentSeq.begin = format.begin;
		for (auto it = format.begin; it != format.end; it++)
		{
			switch (*it)
			{
			case '\\':
				make_Text(result, currentSeq, it);
				++currentSeq.begin;
				++it;
				break;
			case '$':
			{
				make_Text(result, currentSeq, it);
				++currentSeq.begin;
				biIterator var = getVariable(biIterator(currentSeq.begin, format.end));
				result->push_back(std::unique_ptr<SharedRef<Item>>(new SharedRef<Item>(m_container->getItem(var))));
				it = var.end;
				currentSeq.begin = std::next(var.end);
			}
				break;
			case '{':
			{
				make_Text(result, currentSeq, it);
				biIterator block = getBlock(biIterator(std::next(it), format.end));
				result->push_back(genBlock(block));
				it = block.end;
				currentSeq.begin = std::next(block.end);
			}
				break;
			case '}':
			case ';':
				//THROW EXCEPTION
				break;
			}
		}
		currentSeq.end = format.end;
		if (currentSeq.length() > 0)
			result->push_back(std::unique_ptr<Text>(new Text(currentSeq)));
		if (result->size() == 1)
			return result->pop_back();
		return result;
	}
	Object::ptr Compile::parse(const std::string & format)
	{
		return parse(biIterator(format.begin(), format.end()));
	}
	void Compile::setContainer(std::shared_ptr<Container> cont)
	{
		m_container = cont;
	}
	void Compile::addModuleFactory(std::string name, std::shared_ptr<Factory> fact)
	{
		m_factories[name].swap(fact);
	}
	void Compile::make_Text(std::unique_ptr<Collection>& list, biIterator & format, std::string::const_iterator it)
	{
		format.end = it;
		if (format.length() > 0)
			list->push_back(std::unique_ptr<Text>(new Text(format)));
		format.begin = it;
	}
	biIterator parse_helper::getVariable(biIterator range)
	{
		for (auto it = range.begin; it != range.end; it++)
		{
			switch (*it)
			{
			case '\\':
				++it;
				break;
			case ';':
				return biIterator(range.begin, it);
				break;
			}
		}
		return biIterator(range.end, range.end);
	}
	biIterator parse_helper::getBlock(biIterator range)
	{
		int level = 1;
		for (auto it = range.begin; it != range.end; it++)
		{
			switch (*it)
			{
			case '\\':
				++it;
				break;
			case '{':
				++level;
				break;
			case '}':
				if (--level==0)
					return biIterator(range.begin, it);
				break;
			}
		}
		return biIterator(range.end, range.end);
	}
	std::list<biIterator> parse_helper::getArguments(biIterator& range)
	{
		std::list<biIterator> listArgs;
		int level = 1;
		biIterator currentPart(range);
		for (auto it = range.begin; it != range.end; it++)
		{
			switch (*it)
			{
			case '\\':
				++it;
				break;
			case '(':
				++level; 
				break;
			case ',':
				if (level == 1)
				{
					currentPart.end = it;
					listArgs.push_back(currentPart);
					currentPart.begin = next(it);
				}
				break;
			case ')':
				if (--level == 0)
				{
					currentPart.end = it;
					it = std::prev(range.end);
				}
				break;
			}
		}
		if (!currentPart.empty())
			listArgs.push_back(currentPart);
		range.end = currentPart.end;
		return listArgs;
	}
	biIterator parse_helper::getSubCondition(biIterator range)
	{
		int level = 1;
		for (auto it = range.begin; it != range.end; it++)
		{
			switch (*it)
			{
			case '(':
				++level;
				break;
			case ')':
				if (--level == 0)
					return biIterator(range.begin, it);
				break;
			}
		}
		//THROW
		return biIterator();
	}
	Object::ptr Compile::genBlock(biIterator range)
	{
		biIterator parts[3];
		int ipart = 0;
		int level = 0;
		biIterator currentPart(range);
		for (auto it = range.begin; it != range.end; it++)
		{
			switch (*it)
			{
			case '\\':
				++it;
				break;
			case '{':
				++level;
				break;
			case '}':
				--level;
				break;
			case '?':	
			case ':':
				if (level == 0)
				{
					currentPart.end = it;
					parts[ipart++] = currentPart;
					currentPart.begin = next(it);
				}
				break;
			}
			//if (ipart > 3)
				////THROW
		}
		currentPart.end = range.end;
		parts[ipart++] = currentPart;
		if (ipart == 1)
			return genFunctions(parts[0]);
		if (ipart == 2 || ipart == 3)
			return genConditional(parts[0], parts[1], parts[2]);
		return Object::ptr();
	}
	Object::ptr Compile::genFunctions(biIterator range)
	{
		using namespace parse_helper;
		std::unique_ptr<Collection> listFunc = std::unique_ptr<Collection>(new Collection());
		biIterator currentPart(range);
		biIterator currentNameFunc;
		std::unique_ptr<Function> currentFunc;
		int level = 0;
		for (auto it = range.begin; it != range.end; it++)
		{
			switch (*it)
			{
			case '{':
				++level;
				break;
			case '}':
				--level;
				break;
			case '(':
				if (level==0)
			{
				currentPart.end = it;
				currentNameFunc = currentPart;
				currentNameFunc.trim();
				if (currentNameFunc.empty()) // comment
				{
					currentPart = biIterator(next(it), range.end);
					getArguments(currentPart);
					it = currentPart.end;
					currentPart.begin = std::next(currentPart.end);
					break;
				}
				

				{
					auto parsedName = parse(currentNameFunc);
					if (dynamic_cast<subs::Text*>(parsedName.get()))
					{
						std::string name1=parsedName->get();
						if (m_factories.find( name1)!=m_factories.end())
							currentFunc = m_factories[name1]->make_function();
						else
							currentFunc = std::unique_ptr<FunctionStatic>(new FunctionStatic(Function::GetFunction(currentNameFunc)));
					}
						
					else
						currentFunc = std::unique_ptr<FunctionDynamic>(new FunctionDynamic(std::move(parsedName)));
				}
				currentPart = biIterator(next(it), range.end);
				auto listargs = getArguments(currentPart);
				for (auto arg : listargs)
					currentFunc->addArgument(parse(arg));
				listFunc->push_back(std::move(currentFunc));
				it = currentPart.end;
				currentPart.begin = std::next(currentPart.end);
			}
				break;
			case ')':
				//THROW
				break;
			}
		}
		if (listFunc->size() == 1)
			return listFunc->pop_back();
		return listFunc;
	}
	Object::ptr Compile::genConditional(biIterator condition, biIterator pass, biIterator fail)
	{
		std::unique_ptr<Conditional> conditional = std::unique_ptr<Conditional>(new Conditional());
		conditional->setPassObject(parse(pass));
		if (!fail.empty())
			conditional->setFailObject(parse(fail));
		subs::condition::Parse condParser;
		condParser.setContainer(m_container);
		condParser.setSubsParser(this);
		conditional->setQuestion(condParser.parse(condition));
		return conditional;
	}
	void Init()
	{
		subs::Function::Init();
		subs::condition::Condition::Init();
	}
	void ArgsSet::addArgument(Object::ptr arg)
	{
		m_arguments.push_back(std::move(arg));
	}
	void ArgsSet::clearArguments()
	{
		m_arguments.clear();
	}
}