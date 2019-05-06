#pragma once
#include "subs.hpp"
#include <sstream>
#include <iomanip>
#include <memory>
#include "BiIterator.hpp"
#include <json/json.h>
#include <iostream>
namespace subs
{
	namespace integrator
	{
		template <typename Container>
		class _SubsJsonBased : public Interpret<Container>
		{
			using Arguments = typename Interpret<Container>::Arguments;
			using _string = typename Interpret<Container>::_string;
		public:
			_SubsJsonBased() : Interpret<Container>()
			{
				using namespace std::string_literals;
				
				
				/*this->addAnalyser("exists", [this](Arguments args) {
					this->checkContainer();
					return this->getContainer().find(args[0]) != this->getContainer().end();
				});*/
			}
		protected:
			virtual _string get_value(biIterator arg) const
			{
				using namespace std::string_literals;
				using _const_iterator = typename _string::const_iterator;

				this->checkContainer();
				if (arg.empty())
					return ""s;
				_string arg0 = trim(arg);
				// size_t prev = 0, pos = 0;
				biIterator word, arrayID;
				word.begin = arg0.begin();
				int level = 0;
				auto currentJson = this->getContainer();
				auto sendError = [&arg0](const char* msg, biIterator biit) {
					std::ostringstream ostr;
					int dist = std::distance<_const_iterator>(arg0.begin(), biit.begin);
					ostr << "Json : " << msg << " (col " << dist << ").\n";
					ostr << arg0 << "\n";
					ostr << std::string(dist, ' ') << std::string(biit.size(), '~');
					throw std::runtime_error(ostr.str());
				};
				
				std::ostringstream oResult;
				std::string final_name;
				for (auto itCh = arg0.begin(); itCh != arg0.end(); itCh++)
				{
					switch (*itCh)
					{
					case '\\':
						if (level == 0)
						{
							word.end = itCh;
							final_name += word.str();
							itCh++;
							word.begin = itCh;
						}
						break;
					case '.':
						if (level == 0)
						{
							word.end = itCh;
							final_name += word.str();
							std::string w = final_name;
							if (currentJson.isMember(w))
								currentJson = currentJson[w];
							else
								return "{(Champ Introuvable)}"s;
							word.begin = itCh + 1;
							final_name.clear();
						}
						break;
					case '[':
						if (level == 0)
						{
							word.end = itCh;
							final_name += word.str();
							std::string w = final_name;
							if (currentJson.isMember(w))
								currentJson = currentJson[w];
							else
								return "{(Champ Introuvable)}"s;
							arrayID.begin = itCh + 1;
						}
						else if (level == 1)
							sendError("Crochet ouvrant en trop", biIterator(itCh, itCh + 1));
						level++;
						break;
					case ']':
						level--;
						if (level == 0)
						{
							arrayID.end = itCh;
							int id = 0;
							try
							{
								id = std::stoi(arrayID);
							}
							catch (std::invalid_argument exc)
							{
								sendError("Indice de tableau incorrect.", arrayID);
							}
							if (currentJson.isValidIndex(id))
								currentJson = currentJson[id];
							else
								sendError("Index en dehors du tableau", arrayID);
							word.begin = itCh + 1;
						}
						break;
					}
				}
				if (level != 0)
				{
					sendError("Crochet fermant ']' manquant.", biIterator(arrayID.begin, arg0.end()));
				}
				word.end = arg0.end();
				if (word.size() != 0)
				{
					final_name += word.str();
					std::string w = final_name;
					if (currentJson.isMember(w))
						currentJson = currentJson[w];
					else
						return "{(Champ Introuvable)}"s;
				}
				if (currentJson.isObject() || currentJson.isArray())
				{
					using namespace Json;
					StreamWriterBuilder builder;
					builder["commentStyle"] = "None";
					builder["enableYAMLCompatibility"] = true;
					builder["indentation"] = "";  // or whatever you like
					std::unique_ptr<Json::StreamWriter> writer(
						builder.newStreamWriter());
					writer->write(currentJson, &oResult);
				}
				else
					oResult << currentJson.asString();
				return oResult.str();
			}
		};
		using JsonCppSubs = _SubsJsonBased<Json::Value>;
	}
}