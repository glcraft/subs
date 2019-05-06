#pragma once
#include "subs.hpp"
#include "regex_literals.h"
namespace subs
{
	namespace integrator
	{
		template <typename Container>
		class VectorBased : public Interpret<Container>
		{
		public:
			using Arguments = typename Interpret<Container>::Arguments;
			using _string = typename Interpret<Container>::_string;
			VectorBased() : Interpret<Container>()
			{
				using namespace std::string_literals;
				
				this->addAnalyser("exists", [this](Arguments args) {
					this->checkContainer();
					if (R"(\d+)"_rg.search(args[0]))
						return static_cast<bool>(stoul(args[0]) < this->getContainer().size());
					else
						throw std::runtime_error("Mauvais format.");
				});
			}
		protected:
			virtual _string get_value(biIterator arg0) const
			{
				using namespace std::string_literals;
				this->checkContainer();
				if (arg0.empty())
					return ""s;
				//_string arg0 = trim(args[0]);
				if (R"(\d+)"_rg.search(arg0))
				{
					size_t id = std::stoul(arg0);
					if (id >= this->getContainer().size())
						return "{(Nombre hors rang)}"s;
					return static_cast<std::string>(this->getContainer()[id]);
				}
				else if (arg0=="last")
					return static_cast<std::string>(*(this->getContainer().end() - 1));
				else
					throw std::runtime_error("Mauvais format.");
			}
		};
		using VectorSubs = VectorBased<std::vector<std::string>>;
		using BiItVectorSubs = VectorBased<std::vector<biIterator>>;
		using RegexSubs = VectorBased<std::match_results<std::string::const_iterator>>;
	}
}
