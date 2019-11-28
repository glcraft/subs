
#include <subs/core/subs_conditional.h>
#include <memory>
// #include <subs/maths/subs_maths.h>
namespace subs
{
	using namespace std;
	
	std::string Conditional::get() const
	{
		return m_question->ask() ? m_pass->get() : m_fail ? m_fail->get() : std::string();
	}
	void Conditional::setQuestion(condition::Question::ptr question)
	{
		m_question.swap(question);
	}
	void Conditional::setPassObject(Object::ptr pass)
	{
		m_pass.swap(pass);
	}
	void Conditional::setFailObject(Object::ptr fail)
	{
		m_fail.swap(fail);
	}
	namespace condition
	{
		std::map<std::string /*func_name*/, condition_t /*func*/> Condition::m_conditions;
		
		void Condition::AddCondition(std::string name, condition_t func)
		{
			m_conditions[name].swap(func);
		}
		void Condition::DeleteCondition(std::string name)
		{
			auto found = m_conditions.find(name);
			if (found == m_conditions.end())
				throw std::out_of_range("Function::DeleteFunction : name not found");
			m_conditions.erase(found);
		}
		const condition_t & Condition::GetCondition(std::string name)
		{
			auto found = m_conditions.find(name);
			if (found == m_conditions.end())
				return m_conditions["null"];
			return found->second;
		}
		std::vector<std::string> Condition::ListCondition()
		{
			std::vector<std::string> result;
			for (auto& func : m_conditions)
				result.push_back(func.first);
			return result;
		}
		void Condition::setCondition(condition_t cond)
		{
			m_condition.swap(cond);
		}
		bool Condition::ask() const
		{
			return m_condition(m_arguments);
		}
		void Operator::setOperator(operator_t op)
		{
			m_op = op;
		}
		void Operator::setConditions(Question::ptr cond1, Question::ptr cond2)
		{
			m_conditions[0].swap(cond1);
			m_conditions[1].swap(cond2);
		}
		bool Operator::ask() const
		{
			if (!m_conditions[0] || !m_conditions[1])
				return false;
			return m_op(m_conditions[0]->ask(), m_conditions[1]->ask());
		}
		void Existance::setItem(Item::shared_ptr item)
		{
			m_item.swap(item);
		}
		bool Existance::ask() const
		{
			return m_item->exists();
		}
		Parse::Parse()
		{
		}
		Parse::Parse(std::shared_ptr<Container> cont)
		{
			setContainer(cont);
		}
		Question::ptr Parse::parse(biIterator format)
		{
			std::vector<Question::ptr> questions;
			std::list<int> indexes;
			biIterator currentPart(format);
			// Priorit� calcul : se renseigner sur https://en.wikipedia.org/wiki/Shunting-yard_algorithm

			for (auto it = format.begin; it != format.end; ++it)
			{
				switch (*it)
				{
				case '(':
				{
					auto nameQuest = biIterator(currentPart.begin, it);
					nameQuest.trim();
					biIterator cond;
					Question::ptr newQuest;
					const auto& moduleFactories = m_subsParser->getModuleFactories();
					
					if (nameQuest.empty())
					{
						cond = parse_helper::getSubCondition(biIterator(std::next(it), format.end));
						newQuest = parse(cond);
					}
					else
					{
						cond = biIterator(next(it), format.end);
						auto listargs = parse_helper::getArguments(cond);
						bool haveArgs = true;
						if (nameQuest == "exists")
						{
							if (listargs.size()==1)
							{
								auto quest = std::unique_ptr<Existance>(new Existance());
								quest->setItem(m_container->getItem(listargs.front()));
								newQuest = move(quest);
								haveArgs = false;
							}
							else
							{
								//THROW
								auto quest = std::unique_ptr<Condition>(new Condition());
								quest->setCondition(Condition::GetCondition("False"));
								newQuest = move(quest);
							}
						}
						else if (moduleFactories.find(nameQuest)!=moduleFactories.end())
						{
							newQuest = moduleFactories.at(nameQuest)->make_question();
						}
						else
						{
							auto quest = std::unique_ptr<Condition>(new Condition());
							quest->setCondition(Condition::GetCondition(nameQuest));
							newQuest = move(quest);
						}
						if (haveArgs)
						{
							auto* argset = dynamic_cast<ArgsSet*>(newQuest.get());
							if (m_subsParser)
								for (auto arg : listargs)
									argset->addArgument(m_subsParser->parse(arg));
							else
								for (auto arg : listargs)
									argset->addArgument(std::unique_ptr<subs::Text>(new subs::Text(arg)));
						}
					}

					questions.push_back(move(newQuest));
					indexes.push_back(questions.size()-1);
					currentPart.begin = next(cond.end);
					it = cond.end;
				}
					break;
				case '^':
				case '&':
				case '|':
					//if (indexes.back()<0)
					//	//THROW
					switch (*it)
					{
					case '^':
						indexes.push_back(-3);
						break;
					case '&':
						indexes.push_back(-2);
						break;
					case '|':
						indexes.push_back(-1);
						break;
					}
					currentPart.begin = next(it);
					break;
				
				}
			}
			//https://fr.cppreference.com/w/cpp/language/operator_precedence
			operator_t ops[] = {
				[](bool v1, bool v2) {return v1 || v2; },
				[](bool v1, bool v2) {return v1 && v2; },
				[](bool v1, bool v2) {return !(v1 == v2); },//XOR
				//[](bool v1, bool v2) {return !v1; },
			};
			int op = -3;
			while (op<0)
			{
				for (auto it = indexes.begin(); it != indexes.end(); ++it)
				{
					if (*it == op)
					{
						std::unique_ptr<Operator> opQuest = std::unique_ptr<Operator>(new Operator());
						opQuest->setConditions(std::move(questions[*prev(it)]), std::move(questions[*next(it)]));
						opQuest->setOperator(ops[-op-1]);
						questions.push_back(std::move(opQuest));
						auto itErase = prev(it);
						itErase = indexes.erase(itErase);
						itErase = indexes.erase(itErase);
						itErase = indexes.erase(itErase);
						it = indexes.insert(itErase, questions.size()-1);
					}
				}
				++op;
			}
			return std::move(questions.back());
		}
		Question::ptr Parse::parse(const std::string & format)
		{
			return parse(biIterator(format.begin(), format.end()));
		}
		void Parse::setContainer(std::shared_ptr<Container> cont)
		{
			m_container = cont;
		}
		void Parse::setSubsParser(subs::Compile * subsParser)
		{
			m_subsParser = subsParser;
		}
	}
}

