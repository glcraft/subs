#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

#include <algorithm>
#include <functional>
#include <sstream>
#include <map>
#include "BiIterator.hpp"
#include "regex_literals.h"
#include "base64.h"
#include "Lambda.hpp"
#include "string_helper.h"
// #include "Utils.h"
namespace subs
{
	class ParseException : public std::exception
	{
	public:
		typedef char Char;
		ParseException() noexcept : range(), messageNrange()
		{

		}
		ParseException(std::string message, biIterator ran) noexcept : range(ran)
		{
			messageNrange = message + "\nError at \"" + std::basic_string<Char>(ran.begin, ran.begin) + "\"";
		}
		virtual ~ParseException() noexcept {}
		virtual const char* what() const noexcept
		{
			return messageNrange.c_str();
		}

		const biIterator& getRange() const noexcept
		{
			return range;
		}
	protected:
		biIterator range;
		std::basic_string<Char> messageNrange;
	};
	template <typename Container>
	class Interpret
	{
	public:
		typedef char Char;
		typedef std::basic_string<Char> _string;
		typedef std::basic_ostringstream<Char> _ostringstream;
		//typedef biIterator<Char> biIterator;
		typedef std::vector<_string> Arguments;
		Interpret()
		{
			using namespace std::string_literals;
			addFunction("uppercase", [this](Arguments args) {
				if (args.empty()) return ""s;
				_string result=args[0];
				std::transform(result.begin(), result.end(), result.begin(), ::toupper);//std::for_each(result.begin(), result.end(), [](char& c) {c=std::toupper(c)});
				return result;
			});
			addFunction("lowercase", [this](Arguments args) {
				if (args.empty()) return ""s;
				_string result = args[0];
				std::transform(result.begin(), result.end(), result.begin(), ::tolower); //std::for_each(result.begin(), result.end(), [](char& c) {c = std::tolower(c)});
				return result;
			});
			auto CapNCamel = [this](Arguments args, bool isNotCamel) {
				if (args.empty()) return ""s;
				_string result = args[0];
				bool inWord = false;
				bool firstChar = false;
				std::for_each(result.begin(), result.end(), [&inWord, &isNotCamel, &firstChar](char& c)
				{
					bool isAlpha = ::isalpha(c);
					if (isAlpha)
						c = (!inWord && (isNotCamel || firstChar)) ? ::toupper(c) : ::tolower(c);
					firstChar = true;
					inWord = (isAlpha);
				});
				if (args.size() >= 2)
				{
					args[1] = strh::trim(args[1], " ");
					if (args[1] == "remove_space")
					{
						typename _string::iterator end_pos = std::remove(result.begin(), result.end(), ' ');
						result.erase(end_pos, result.end());
					}
				}
				return result;
			};
			addFunction("time", [this](Arguments args) {
				auto now = std::chrono::system_clock::now();
				auto in_time_t = std::chrono::system_clock::to_time_t(now);

				std::stringstream ss;
				ss << std::put_time(std::localtime(&in_time_t), (args.empty()) ? "%H:%M:%S" : args[0].c_str());
				return ss.str();

			});
			addFunction("date", [this](Arguments args) {
				auto now = std::chrono::system_clock::now();
				auto in_time_t = std::chrono::system_clock::to_time_t(now);

				std::ostringstream ss;
				ss << std::put_time(std::localtime(&in_time_t), (args.empty()) ? "%d/%m/%Y" : args[0].c_str());
				return ss.str();
			});

			addFunction("capitalize", [this, CapNCamel](Arguments args) { return CapNCamel(args, true); });
			addFunction("camelcase", [this, CapNCamel](Arguments args) { return CapNCamel(args, false); });

			auto AlphaNum = [this](Arguments args, bool getAlpha, bool getNum) {
				if (args.empty()) return ""s;
				_string result = args[0];
				typename _string::iterator end_pos = std::remove_if(result.begin(), result.end(), [getAlpha, getNum](unsigned char ch) {
					bool b1 = getAlpha && isalpha(ch);
					bool b2 = getNum && isdigit(ch);
					return !(b1 || b2);
				});
				result.erase(end_pos, result.end());
				return result;
			};
			addFunction("alpha", [this, AlphaNum](Arguments args) { return AlphaNum(args, true, false); });
			addFunction("alphanum", [this, AlphaNum](Arguments args) { return AlphaNum(args, true, true); });
			addFunction("num", [this, AlphaNum](Arguments args) { return AlphaNum(args, false, true); });
			addFunction("replace", [this](Arguments args) { 
				if (args.size()<3) return "{replace: Needed 3 argument (text, old_pattern, new_pattern)}"s;
				_string result;
				_string _oldString = args[0];
				_string _oldPattern = args[1];
				_string _newPattern = args[2];
				size_t found = _oldString.find(_oldPattern), prev=0;
				size_t len = _oldPattern.length();
				while (found!=_string::npos)
				{
					result+=_oldString.substr(prev,found-prev)+_newPattern;
					prev = found+len;
					found = _oldString.find(_oldPattern,prev);
				}
				result+=_oldString.substr(prev);
				return result; 
			});
			addFunction("count_chars", [this](Arguments args) { 
				if (args.empty()) return "0"s;
				return std::to_string(args[0].length());
			});
			addFunction("line_number", [this](Arguments args) {
				return std::to_string(m_countLines);
			});
			addFunction("counter", [this](Arguments args) {
				return std::to_string(m_counter++);
			});
			addFunction("shuffle", [this](Arguments args) {
				if (args.empty()) return ""s;
				_string result = args[0];
				std::random_shuffle(result.begin(), result.end());
				return result;
			});
			addFunction("randomize", [this](Arguments args) {
				if (args.size() < 1 || args.size() > 2)
					return "{Wrong argument number}"s;
				_string chars;
				if (args.size() == 2)
					chars = args[1];
				else
					chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ";
				size_t nbChars = stoul(args[0]);
				_string result(nbChars, ' ');
				std::for_each(result.begin(), result.end(), [&chars](char& ch) {ch = chars[rand() % chars.size()]; });
				return result;
			});
			
			addFunction("base64_encode", [this](Arguments args) {
				if (args.empty()) return ""s;
				return base64_encode(reinterpret_cast<const unsigned char*>(args[0].c_str()), args[0].size());
			});
			addFunction("base64_decode", [this](Arguments args) {
				if (args.empty()) return ""s;
				return base64_decode(args[0]);
			});
			addAnalyser("empty", [this](Arguments args) {
				
				return args.empty() ? true : args[0].empty();
			});
			addAnalyser("equals", [this](Arguments args) {
				if (args.size() != 2)
					throw std::runtime_error("compare : "s+(args.size()>2?"trop d'arguments":"pas assez d'argument")+u8", deux arguments sont nécéssaires"s);
				return args[0] == args[1];
			});
			addAnalyser("True", [this](Arguments args) {
				return true;
			});

			addAnalyser("False", [this](Arguments args) {
				return false;
			});
			m_depth = 0;
		};
		~Interpret()
		{
			eraseContainer();
		};
		void addAnalyser(_string name, std::function<bool(Arguments)> analyser)
		{
			m_analysers[name] = analyser;
		}
		void addFunction(_string name, std::function<_string(Arguments)> function)
		{
			m_functions[name] = function;
		}
		_string parse(_string format) const
		{
			return parse(biIterator(format.begin(), format.end()));
		}
		_string parse(biIterator format) const
		{
			if (m_depth==0)
				m_counter = 1;//m_countLines = 1

			auto prevPos = format.begin;
			short currentLevel = 0;
			bool isVariable = false;
			_ostringstream finalString;
			m_depth++;
			for (auto itCh = format.begin; itCh != format.end; itCh++)
			{
				switch (*itCh)
				{
				case '\n':
					if (currentLevel == 0)
						m_countLines++;
					break;
				case '\\':
					if (currentLevel == 0 && !isVariable)
					{
						finalString << _string(prevPos, itCh);
						prevPos = itCh++ + 1;
					}
					break;
				case '{':
					if (currentLevel == 0)
					{
						finalString << _string(prevPos, itCh);
						prevPos = itCh + 1;
					}
					++currentLevel;
					break;
				case '}':
					--currentLevel;
					if (currentLevel == 0)
					{
						finalString << parse_block(biIterator(prevPos, itCh));
						prevPos = itCh + 1;
					}
					break;
				case '$':
					if (currentLevel == 0)
					{
						finalString << _string(prevPos, itCh);
						prevPos = itCh + 1;
						isVariable = true;
					}
					break;
				case ';':
					if (currentLevel == 0)
					{
						if (!isVariable)
							throw std::runtime_error("\";\" sans \"$\" avant.");
						finalString << get_value(biIterator(prevPos, itCh));
						prevPos = itCh + 1;
						isVariable = false;
					}
				}
			}
			if (prevPos != format.end)
				finalString << _string(prevPos, format.end);
			m_depth--;
			return finalString.str();
		}
		void createContainer()
		{
			eraseContainer();
			m_cont = new Container;
			m_isRef = false;
		}
		void setContainer(Container& cont)
		{
			eraseContainer();
			m_cont = &cont;
			m_isRef = true;
		}
		Container& getContainer()
		{
			checkContainer();
			return *m_cont;
		}
		const Container& getContainer() const
		{
			checkContainer();
			return *m_cont;
		}
		bool hasContainer() const
		{
			return m_cont;
		}
		bool isContainerRef() const
		{
			return m_isRef;
		}
		void checkContainer() const
		{
			if (!m_cont)
				throw std::runtime_error(u8"Aucun container assigné");
		}
		std::vector<std::string> getFunctionsName()
		{
			std::vector<std::string> res;
			res.reserve(m_functions.size());
			for (auto& func : m_functions)
				res.push_back(func.first);
			return std::move(res);
		}
		std::vector<std::string> getAnalyserName()
		{
			std::vector<std::string> res;
			res.reserve(m_analysers.size());
			for (auto& analyser : m_analysers)
				res.push_back(analyser.first);
			return std::move(res);
		}
	protected:
		void eraseContainer()
		{
			if (m_cont == nullptr && !m_isRef)
				delete m_cont;
			m_cont = nullptr;
		}
		virtual std::string get_value(biIterator arg0) const = 0;
		mutable int m_counter, m_countLines, m_depth;
	private:
		Arguments get_args(biIterator format, bool parseArgs = true) const
		{
			auto prevPos = format.begin;
			short currentLevel = 0;
			Arguments args;
			for (auto itCh = format.begin; itCh != format.end; itCh++)
			{
				switch (*itCh)
				{
				case '\\':
					itCh++;
					break;
				case '{':
				case '(':
					currentLevel++;
					break;
				case '}':
				case ')':
					currentLevel--;
					break;
				case ',':
					if (currentLevel == 0)
					{
						if (parseArgs)
							args.push_back(parse(biIterator(prevPos, itCh)));
						else
							args.push_back(biIterator(prevPos, itCh));
						prevPos = itCh + 1;
					}
					break;
				}
			}
			if (prevPos != format.end)
			{
				if (parseArgs)
					args.push_back(parse(biIterator(prevPos, format.end)));
				else
					args.push_back(biIterator(prevPos, format.end));
			}
			return args;
		}
		_string parse_block(biIterator format) const
		{
			using namespace std::string_literals;
			auto prevPos = format.begin;
			short currentLevel = 0;
			biIterator partStr[3];
			int currentID = 0;
			for (auto itCh = format.begin; itCh != format.end; itCh++)
			{
				switch (*itCh)
				{
				case '\\':
					if (currentLevel == 0)
						itCh++;
					break;
				case '(':
				case '{':
					++currentLevel;
					break;
				case ')':
				case '}':
					--currentLevel;
					break;
				case '?':
					if (currentLevel == 0)
					{
						if (currentID != 0)
							throw std::runtime_error(u8R"(Syntaxe invalide : "?" mal positionné.)");
						partStr[currentID++] = biIterator(prevPos, itCh);
						prevPos = itCh + 1;
					}
					break;
				case ':':
					if (currentLevel == 0)
					{
						if (currentID != 1)
							throw std::runtime_error(u8R"(Syntaxe invalide : ":" mal positionné.)");
						partStr[currentID++] = biIterator(prevPos, itCh);
						prevPos = itCh + 1;
					}
					break;
				}
			}
			if (currentLevel != 0)
				throw std::runtime_error(u8R"(Erreur de syntaxe : le nombre de parenthèse/accolade ne correspond pas.)");
			if (currentID == 0)
				return parse_function(parse(format));
			else
			{
				partStr[currentID] = biIterator(prevPos, format.end);
				if (parse_expression(partStr[0]))
					return parse(partStr[1]);
				else if (currentID == 2)
					return parse(partStr[2]);
				else return ""s;
			}
		}
		bool parse_expression(biIterator format) const
		{
			using namespace std::string_literals;
			auto prevPos = format.begin;
			short currentLevel = 0;
			bool invert=false;
			char ops = 2; // OR by default
			biIterator currentAnalyser;
			bool finalBool = false; // FALSE by default
			for (auto itCh = format.begin; itCh != format.end; itCh++)
			{
				switch (*itCh)
				{
				case '\\':
					if (currentLevel == 0)
						 itCh++;
					break;
				case '!':
				{
					biIterator testValidity(prevPos, itCh);
					testValidity.trim();
					if (!testValidity.empty())
						throw std::runtime_error(R"(Erreur de syntaxe: "!" mal positionné)");
				}
					invert = true;
					prevPos = itCh + 1;
					break;
				case '&':
				case '|':
					if (currentLevel == 0)
					{
						ops = 1 + (*itCh == '|');
						prevPos = itCh + 1;
					}
					break;
				case '(':
					if (currentLevel == 0)
					{
						if (ops == 0)
							throw std::runtime_error("Erreur de syntaxe: opérateur manquant");

						currentAnalyser.set(prevPos, itCh);
						currentAnalyser.trim();
						prevPos = itCh + 1;
					}
					currentLevel++;
					break;
				case ')':
					currentLevel--;
					if (currentLevel == 0)
					{
						biIterator currentArgs = biIterator(prevPos, itCh);
						bool result;
						if (currentAnalyser.empty())
							result = parse_expression(biIterator(prevPos, itCh));
						else if (currentAnalyser == "calc")
						{
							std::string arg = parse(currentArgs);
							result = parse_calc(arg) != 0;
						}
						else
						{
							_string analy = _string(currentAnalyser.begin, currentAnalyser.end);
							auto itFunc = m_analysers.find(analy);
							if (itFunc != m_analysers.end())
								result = itFunc->second(get_args(currentArgs));
							
							else
								throw std::runtime_error("Analyseur inconnu: "s + analy);
						}
						if (invert)
						{
							result = !result;
							invert = false;
						}
						if (ops == 1)
							finalBool = finalBool && result;
						else
							finalBool = finalBool || result;

						ops = 0;
						prevPos = itCh + 1;
					}
					break;
				}
			}
			return finalBool;
		}
		_string parse_function(biIterator format) const
		{
			using namespace std::string_literals;
			auto prevPos = format.begin;
			short currentLevel = 0;
			_ostringstream finalString;
			biIterator currentFunction;
			for (auto itCh = format.begin; itCh != format.end; itCh++)
			{
				switch (*itCh)
				{
				case '\\':
					if (currentLevel == 0)
						itCh++;
					break;
				case '(':
					if (currentLevel == 0)
					{
						currentFunction = biIterator(prevPos, itCh);
						currentFunction.trim();
						prevPos = itCh + 1;
					}
					currentLevel++;
					break;
				case ')':
					currentLevel--;
					if (currentLevel == 0)
					{
						biIterator currentArgs = biIterator(prevPos, itCh);
						if (currentFunction.empty())
						{
							//Commentaires 
						}
						else if (currentFunction == "calc")
						{
							std::string arg = parse(currentArgs);
							finalString << std::to_string(parse_calc(biIterator(arg)));
						}
						else if (currentFunction == "repeat")
						{
							auto args = get_args(currentArgs, false);
							finalString << loop_repeat(std::stoi(args[0]), args[1]);
						}
						else if (currentFunction == "while")
						{
							auto args = get_args(currentArgs, false);
							finalString << loop_while(args[0], args[1]);
						}
						else if (currentFunction == "lambda")
						{
							auto args = get_args(currentArgs, false);
							Lambda::Get().insert(std::pair<_string, Lambda>(args[0], Lambda(args[1])));
							
							//itFunc = m_functions.find(_string(currentFunction.begin, currentFunction.end));
						}
						else
						{
							auto itFunc = m_functions.find(currentFunction);
							auto itLamb = Lambda::Get().find(currentFunction);
							if (itFunc != m_functions.end())
								finalString << itFunc->second(get_args(currentArgs));
							else if(itLamb != Lambda::Get().end())
								finalString << itLamb->second(get_args(currentArgs));
							else
								finalString << "{Unknown function}";
						}

						prevPos = itCh + 1;
					}
					break;
				}
			}
			return finalString.str();
		}
		long long parse_calc(biIterator format) const
		{
			using namespace std::string_literals;
			auto prevPos = format.begin;
			short currentLevel = 0;
			long long finalNumber=0;
			bool isVariable = false;
			char op = '+'; //+-*/%
			auto opNumber = [&op, &finalNumber](long long numb){
				switch (op)
				{
				case '+': finalNumber += numb; break;
				case '-': finalNumber -= numb; break;
				case '*': finalNumber *= numb; break;
				case '/': finalNumber /= numb; break;
				case '%': finalNumber %= numb; break;
				case '>': finalNumber = finalNumber>numb; break;
				case '<': finalNumber = finalNumber<numb; break;
				case '=': finalNumber = finalNumber==numb; break;
				case '[': finalNumber = finalNumber<=numb; break; // <=
				case ']': finalNumber = finalNumber>=numb; break; // >=
				}
				op = 0;
			};
			for (auto itCh = format.begin; itCh != format.end; itCh++)
			{
				if (currentLevel==0 && *itCh >= '0' && *itCh <= '9' && !isVariable)
				{
					prevPos = itCh;
					do
						itCh++;
					while (itCh != format.end && *itCh >= '0' && *itCh <= '9');
					biIterator arg0(prevPos, itCh);
					itCh--;
					arg0.trim();
					long long numb = std::stoll(arg0);
					opNumber(numb);
					
					prevPos = itCh + 1;
					
				}
				else
				{
					switch (*itCh)
					{
					case '(':
						if (currentLevel == 0)
							prevPos = itCh + 1;
						currentLevel++;
						break;
					case ')':
						currentLevel--;
						if (currentLevel == 0)
						{
							opNumber(parse_calc(biIterator(prevPos, itCh)));
							prevPos = itCh + 1;
						}
						break;
					case '$':
						if (currentLevel == 0)
						{
							prevPos = itCh + 1;
							isVariable = true;
						}
						break;
					case ';':
					{
						if (currentLevel != 0)
							break;
						if (!isVariable)
							throw std::runtime_error("\";\" sans \"$\" avant.");
						checkContainer();
						long long numb = std::stoll(get_value (biIterator(prevPos, itCh)) );
						opNumber(numb);
						prevPos = itCh + 1;
						isVariable = false;
					}
						break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '%':
					case '>':
					case '<':
						if (currentLevel == 0)
							op = *itCh;
						break;
					case '=':
						if (currentLevel == 0)
						{
							if (op=='<' || op == '>')
								op += '[' - '<';
							else
								op = *itCh;
						}
						break;
					}
				}
			}
			return finalNumber;
		}
		_string loop_repeat(int numb, biIterator expr) const
		{
			_ostringstream result;
			for (int i = 0; i < numb; i++)
				result<<parse(expr);
			return result.str();
		}
		_string loop_while(biIterator cond, biIterator expr) const
		{
			_ostringstream result;
			while(parse_expression(cond))
				result << parse(expr);
			return result.str();
		}
		Container* m_cont = nullptr;
		bool m_isRef = false;
		std::map<_string, std::function<bool(Arguments)>> m_analysers;
		std::map<_string, std::function<_string(Arguments)>> m_functions;
		
	};
}
