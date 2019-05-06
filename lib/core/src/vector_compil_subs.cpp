
//#include "vector_compil_subs.hpp"
//#include <iostream>
//using namespace std;
//namespace subs
//{
//	VectorContainer::VectorContainer(type container)
//	{
//		set(container);
//	}
//	VectorContainer::~VectorContainer()
//	{
//	}
//	void VectorContainer::set(type container)
//	{
//		m_cont = container;
//		updateItems();
//	}
//	void VectorContainer::freeItems()
//	{
//		m_items.clear();
//	}
//	int VectorContainer::size() const
//	{
//		return m_cont.size();
//	}
//
//	Item::shared_ptr VectorContainer::getItem(biIterator name) const
//	{
//		if (name == "size")
//		{
//			auto found = m_items.find(-1);
//			VectorItem::shared_ptr shPtr;
//			if (found == m_items.end())
//			{
//				shPtr = make_shared<VectorInfo>(m_cont, 1);
//				m_items[-1] = shPtr;
//			}
//			else
//				shPtr = found->second;
//			return shPtr;
//		}
//		else
//		{
//			auto val = getValue(name);
//			if (val != -1)
//			{
//				auto found = m_items.find(val);
//				VectorItem::shared_ptr shPtr;
//				if (found == m_items.end())
//				{
//					shPtr = make_shared<VectorItem>(m_cont, val);
//					m_items[val] = shPtr;
//				}
//				else
//					shPtr = found->second;
//
//				return shPtr;
//			}
//		}
//		return Item::shared_ptr();
//	}
//	void VectorContainer::updateItems()
//	{
//		for (auto item : m_items)
//		{
//			reinterpret_cast<VectorItem*>(item.second.get())->set(m_cont);
//		}
//	}
//	size_t VectorContainer::getValue(biIterator name) const
//	{
//		size_t i;
//		try
//		{
//			i = std::stoull(name);
//		}
//		catch (std::invalid_argument exc)
//		{
//			cerr << __FUNCTION__ << " exception : Argument invalide. Argument passé : " << name.str();
//			return -1;
//		}
//		catch (std::out_of_range exc)
//		{
//			cerr << __FUNCTION__ << " exception : Nombre hors champ. Argument passé : " << name.str();
//			return -1;
//		}
//		return i;
//	}
//	bool VectorContainer::exists(size_t i) const
//	{
//		return i >= 0 && i < m_cont.size();
//	}
//	VectorItem::VectorItem(const VectorContainer::type& array, size_t index)
//	{
//		set(array, index);
//	}
//	std::string VectorItem::get() const
//	{
//		return m_item ? *m_item : "";
//	}
//	bool VectorItem::exists() const
//	{
//		return m_item;
//	}
//	void VectorItem::set(const VectorContainer::type& array, size_t index)
//	{
//		if (index != -1)
//			m_index = index;
//		if (m_index < array.size())
//			m_item = &array[m_index];
//		else
//			m_item = nullptr;
//	}
//
//	VectorInfo::VectorInfo(const VectorContainer::type & array, size_t index)
//	{
//		set(array, index);
//	}
//
//	std::string VectorInfo::get() const
//	{
//		return m_info;
//	}
//
//	bool VectorInfo::exists() const
//	{
//		return true;
//	}
//
//	void VectorInfo::set(const VectorContainer::type & array, size_t index)
//	{
//		if (index != -1)
//			m_infoType = Information(index);
//		switch (m_infoType)
//		{
//		case Size:
//			m_info = to_string(array.size());
//			break;
//		}
//	}
//
//}