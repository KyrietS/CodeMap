#pragma once
#include "Canvas2/Elements/IElement.hpp"
#include <iterator>

using ElementId = std::uint64_t;

// TODO: Move this class and rename it to (Node)Stack/Storage/Container/Registry/Context or something like that
class CanvasElements
{
public:
	ElementId Add(std::unique_ptr<Elements::IElement> element)
	{
		m_Elements.push_front({ m_NextId, std::move(element) });
		return m_NextId++;
	}

	bool Contains(ElementId id) const
	{
		return std::any_of(m_Elements.begin(), m_Elements.end(), [id](const auto& pair) { return pair.first == id; });
	}

	bool Empty() const
	{
		return m_Elements.empty();
	}

	Elements::IElement& Get(ElementId id)
	{
		if (auto element = TryGet(id))
			return *element;
		throw std::runtime_error("Element not found");
	}

	void Remove(ElementId id)
	{
		m_Elements.remove_if([id](const auto& pair) { return pair.first == id; });
	}

	void RemoveIf(std::function<bool(const Elements::IElement&)> cond)
	{
		m_Elements.remove_if([&cond](const auto& pair) { return cond(*pair.second); });
	}

	void Clear()
	{
		m_Elements.clear();
	}

	Elements::IElement* TryGet(ElementId id)
	{
		auto it = std::find_if(m_Elements.begin(), m_Elements.end(), [id](const auto& pair) { return pair.first == id; });
		if (it == m_Elements.end())
			return nullptr;
		return it->second.get();
	}
	template<typename T>
	T* TryGet(ElementId id)
	{
		return dynamic_cast<T*>(TryGet(id));
	}

	// Move element by n positions
	void Move(ElementId id, int n)
	{
		auto it = std::find_if(m_Elements.begin(), m_Elements.end(), [id](const auto& pair) { return pair.first == id; });
		if (it == m_Elements.end())
			return;

		auto newPos = it;
		int offset = n > 0 ? n + 1 : n;
		auto bound = n > 0 ? m_Elements.end() : m_Elements.begin();
		std::ranges::advance(newPos, offset, bound);

		m_Elements.splice(newPos, m_Elements, it);
	}

	auto& Data() { return m_Elements; }
	const auto& Data() const { return m_Elements; }

	// range-for loop support
	auto begin() { return m_Elements.begin(); }
	auto end() { return m_Elements.end(); }
	const auto begin() const { return m_Elements.begin(); }
	const auto end() const { return m_Elements.end(); }

private:
	// TODO: Traversing all the elements is very common operation, consider using std::vector instead of std::list
	std::list<std::pair<ElementId, std::unique_ptr<Elements::IElement>>> m_Elements;
	std::uint64_t m_NextId = 1; // 0 is reserved for invalid id
};