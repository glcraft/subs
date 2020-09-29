
#include <subs/core/subs_compile.h>
#include <subs/core/Lambda.hpp>
#include <subs/core/subs_conditional.h>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <random>
using namespace std;
namespace subs
{
	void Function::Init()
	{
		AddFunction("uppercase", [](const arguments_t& args) -> std::string {
			if (args.empty()) return "";
			string result = args[0]->get();
			std::transform(result.begin(), result.end(), result.begin(), ::toupper);//std::for_each(result.begin(), result.end(), [](char& c) {c=std::toupper(c)});
			return result;
		});
		AddFunction("lowercase", [](const arguments_t& args) -> std::string {
			if (args.empty()) return "";
			string result = args[0]->get();
			std::transform(result.begin(), result.end(), result.begin(), ::tolower);//std::for_each(result.begin(), result.end(), [](char& c) {c=std::toupper(c)});
			return result;
		});
		AddFunction("capitalize", [](const arguments_t& args) -> std::string {
			static const std::string wordSep(" `~!@#$%^&*()-=+[{]}\\|;:'\",.<>/?");
			if (args.empty()) return "";
			string result = args[0]->get();
			bool firstletter=true;
			std::transform(result.begin(), result.end(), result.begin(), [&firstletter](char c) {
				if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
				{
					if (firstletter)
					{
						firstletter = false;
						return ::toupper(c);
					}
					else
						return ::tolower(c);
				}
				else
				{
					//THROW nan c'est pas vrai, juste pour attirer l'attention ici pour les wordSeparators
					if (wordSep.find_first_of(c)!=string::npos)
						firstletter = true;
					return static_cast<int>(c);
				}
			});//std::for_each(result.begin(), result.end(), [](char& c) {c=std::toupper(c)});
			return result;
		});
		AddFunction("random", [](const arguments_t& args) {
			int from, to;
			if (args.size() == 1)
				from = 0, to = stoull(args[0]->get());
			else
				from = stoull(args[0]->get()), to = stoull(args[1]->get());

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<uint64_t> dist(from, to);
			return to_string(dist(mt));
		});
		AddFunction("repeat", [](const arguments_t& args) {
			int numb = stoull(args[0]->get());
			std::stringstream ss;
			for (int i = 0; i < numb; i++)
			{
				ss << args[1]->get();
			}
			return ss.str();
		});
		AddFunction("substring", [](const arguments_t& args) {
			std::string result = args[0]->get();
			string test = args[1]->get();
			int from = stoi(test);
			if (from < 0) from = result.length() + from;
			if (args.size() == 3)
			{
				int to = stoi(args[2]->get());
				if (to < 0) to = result.length() + to;
				return result.substr(from, to - from);
			}
			return result.substr(from);
		});
		AddFunction("find", [](const arguments_t& args) {
			std::string txt = args[0]->get();
			auto result = txt.find(args[1]->get());
			return to_string(result);
		});
		AddFunction("find_reverse", [](const arguments_t& args) {
			std::string txt = args[0]->get();
			auto result = txt.rfind(args[1]->get());
			return to_string(result);
		});
		AddFunction("time", [](const arguments_t& args) {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			std::stringstream ss;
			ss << std::put_time(std::localtime(&in_time_t), (args.empty()) ? "%H:%M:%S" : args[0]->get().c_str());
			return ss.str();
		});
		AddFunction("date", [](const arguments_t& args) {
			auto now = std::chrono::system_clock::now();
			auto in_time_t = std::chrono::system_clock::to_time_t(now);

			std::ostringstream ss;
			ss << std::put_time(std::localtime(&in_time_t), (args.empty()) ? "%d/%m/%Y" : args[0]->get().c_str());
			return ss.str();
		});
		AddFunction("display", [](const arguments_t& args) {
			std::ostringstream ss;
			for (auto& obj : args)
				ss << obj->get();
			return ss.str();
		});
		AddFunction("length", [](const arguments_t& args) {
			std::ostringstream ss;
			for (auto& obj : args)
				ss << obj->get();
			return to_string(ss.str().length());
		});
	}
	namespace condition
	{
		void Condition::Init()
		{
			AddCondition("True", [](const arguments_t& args) {
				return true;
			});
			AddCondition("False", [](const arguments_t& args) {
				return false;
			});
			AddCondition("Equals", [](const arguments_t& args) {
				if (args.size() == 0)
					return false;
				else if (args.size() == 1)
					return true;
				bool result = true;
				std::string prevStr = args[0]->get();
				for (int i = 1; i < args.size() && result; ++i)
				{
					string currentStr(args[i]->get());
					result = currentStr == prevStr;
					prevStr.swap(currentStr);
				}
				return result;
			});
		}
	}
}