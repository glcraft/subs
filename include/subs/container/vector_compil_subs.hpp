#pragma once
#include <subs/core/subs_compile.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <regex>
namespace subs
{
	template <typename Type>
	class VectorBasedContainer : public Container
	{
	public:
		typedef Type type;
		VectorBasedContainer() = default;
		VectorBasedContainer(type container)
		{
			set(container);
		}
		VectorBasedContainer(const VectorBasedContainer&) = default;
		VectorBasedContainer(VectorBasedContainer&&) = default;
		virtual ~VectorBasedContainer()
		{
		}
		
		void set(type container)
		{
			m_cont = container;
			updateItems();
		}
		void freeItems()
		{
			m_items.clear();
		}
		virtual Item::shared_ptr getItem(biIterator name) const
		{
			auto found = m_items.end();
			if (name == "size")
				found = m_items.find(-1);
			else
				found = m_items.find(getValue(name));
			if (found != m_items.end())
				return found->second;
			else
				return Item::shared_ptr();
		}
		virtual Item::shared_ptr getItem(biIterator name)  
		{
			if (name == "size")
			{
				auto found = m_items.find(-1);
				VectorBasedItem<Type>::shared_ptr shPtr;
				if (found == m_items.end())
				{
					shPtr = std::make_shared<VectorBasedInfo<Type>>(m_cont, 1);
					m_items[-1] = shPtr;
				}
				else
					shPtr = found->second;
				return shPtr;
			}
			else
			{
				auto val = getValue(name);
				if (val != -2)
				{
					auto found = m_items.find(val);
					VectorBasedItem<Type>::shared_ptr shPtr;
					if (found == m_items.end())
					{
						shPtr = std::make_shared<VectorBasedItem<Type>>(m_cont, val);
						m_items[val] = shPtr;
					}
					else
						shPtr = found->second;

					return shPtr;
				}
			}
			return Item::shared_ptr();
		}
	protected:
		void updateItems()
		{
			for (auto item : m_items)
			{
				reinterpret_cast<VectorBasedItem<Type>*>(item.second.get())->set(m_cont);
			}
		}
		size_t getValue(biIterator name) const
		{
			size_t i;
			try
			{
				i = std::stoull(name);
			}
			catch (std::invalid_argument exc)
			{
				std::cerr << __FUNCTION__ << " exception : Argument invalide. Argument passé : " << name.str() << std::endl;
				return -2;
			}
			catch (std::out_of_range exc)
			{
				std::cerr << __FUNCTION__ << " exception : Nombre hors champ. Argument passé : " << name.str() << std::endl;
				return -2;
			}
			return i;
		}
		
		bool exists(size_t i) const
		{
			return i >= 0 && i < m_cont.size();
		}
		Type m_cont;

		mutable std::map<size_t, Item::shared_ptr> m_items;
	};
	template <typename Type>
	class VectorBasedItem : public Item
	{
		using vectype = typename VectorBasedContainer<Type>::type;
	public:
		VectorBasedItem() = default;
		VectorBasedItem(const vectype& array, size_t index)
		{
			set(array, index);
		}
		VectorBasedItem(const VectorBasedItem&) = default;
		VectorBasedItem(VectorBasedItem&&) = default;
		virtual std::string get() const
		{
			return m_item ? m_item[m_index] : std::string();
		}
		virtual bool exists() const
		{
			return m_item;
		}
	protected:
		friend VectorBasedContainer<Type>;
		virtual void set(const vectype& array, size_t index=-1)
		{
			if (index != -1)
				m_index = index;
			if (m_index < array.size())
				m_item = &array[0];
			else
				m_item = nullptr;
		}
		
		size_t m_index=-1;
		const typename vectype::value_type* m_item;
	};
	template <typename Type>
	class VectorBasedInfo: public Item
	{
		using vectype = typename VectorBasedContainer<Type>::type;
	public:
		VectorBasedInfo() = default;
		VectorBasedInfo(const vectype& array, size_t index)
		{
			set(array, index);
		}
		VectorBasedInfo(const VectorBasedInfo&) = default;
		VectorBasedInfo(VectorBasedInfo&&) = default;
		enum Information
		{
			Size=1
		};
		virtual std::string get() const
		{
			return m_info;
		}
		virtual bool exists() const
		{
			return true;
		}
	protected:
		friend VectorBasedContainer<Type>;
		virtual void set(const vectype& array, size_t index = -1)
		{
			if (index != -1)
				m_infoType = Information(index);
			switch (m_infoType)
			{
			case Size:
				m_info = std::to_string(array.size());
				break;
			}
		}
		Information m_infoType;
		std::string m_info;
	};
	/*template <>
	class VectorBasedItem<std::smatch> : public Item
	{
		virtual std::string get() const
		{
			return m_item ? m_item[0] : "";
		}
	};*/
	using VectorContainer = VectorBasedContainer<std::vector<std::string>>;
	using RegexContainer = VectorBasedContainer<std::smatch>;
}