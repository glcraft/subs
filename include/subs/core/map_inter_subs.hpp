#pragma once
#include "subs.hpp"
#include <map>
#include <unordered_map>
namespace subs
{
	namespace integrator
	{
		template <typename Container>
		class _SubsMapBased : public Interpret<Container>
		{
			using Arguments = typename Interpret<Container>::Arguments;
			using _string = typename Interpret<Container>::_string;
		public:
			_SubsMapBased() : Interpret<Container>()
			{
				using namespace std::string_literals;
				this->addAnalyser("exists", [this](Arguments args) {
					this->checkContainer();
					return this->getContainer().find(args[0]) != this->getContainer().end();
				});
			}
		public:
			virtual _string get_value(biIterator arg0) const
			{
				using namespace std::string_literals;
				this->checkContainer();
				if (arg0.empty())
					return ""s;
				//_string arg0 = trim(args[0]);
				auto it = this->getContainer().find(arg0);
				if (it != this->getContainer().end())
					return it->second;
				return "{(Champ Introuvable)}"s;
			}
		};
		using MapSubs = _SubsMapBased<std::map<std::string, std::string>>;
		using UnorderedMapSubs = _SubsMapBased<std::unordered_map<std::string, std::string>>;
		// Caution : return first iteration of a key, if many
		using MultimapSubs = _SubsMapBased<std::multimap<std::string, std::string>>;
		// Caution : return first iteration of a key, if many
		using UnorderedMultimapSubs = _SubsMapBased<std::unordered_multimap<std::string, std::string>>;
	}
}