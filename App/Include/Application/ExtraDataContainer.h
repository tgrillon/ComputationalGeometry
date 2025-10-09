#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>

namespace Data::Internal
{
/// @brief A container for storing extra data of arbitrary types, identified by their type.
class ExtraDataContainer
{
public:
	/// @brief Default constructor
	ExtraDataContainer() = default;
	~ExtraDataContainer() = default;

	/// @brief Disable copy semantics
	ExtraDataContainer(const ExtraDataContainer&) = delete;
	ExtraDataContainer& operator=(const ExtraDataContainer&) = delete;

	/// @brief Enable move semantics
	ExtraDataContainer(ExtraDataContainer&&) = default;
	ExtraDataContainer& operator=(ExtraDataContainer&&) = default;

	/// @brief Set the value of type T in the container.
	template<typename T>
	void Set(T&& value)
	{
		m_Data[std::type_index(typeid(T))] = std::forward<T>(value);
	}

	/// @brief Get a pointer to the value of type T in the container, or nullptr if not found.
	template<typename T>
	T* Get() const
	{
		auto it = m_Data.find(std::type_index(typeid(T)));
		if(it == m_Data.end())
			return nullptr;

		return std::any_cast<T>(&it->second);
	}

	/// @brief Get a pointer to the value of type T in the container, creating a default one if not found.
	template<typename T>
	T& GetOrCreate()
	{
		auto it = m_Data.find(std::type_index(typeid(T)));
		if(it == m_Data.end())
		{
			m_Data[std::type_index(typeid(T))] = T{};
			it = m_Data.find(std::type_index(typeid(T)));
		}

		return std::any_cast<T&>(it->second);
	}

	/// @brief Check if the container has a value of type T.
	template<typename T>
	bool Has() const
	{
		return m_Data.find(std::type_index(typeid(T))) != m_Data.end();
	}

	/// @brief Clear container.
	void Clear() { m_Data.clear(); }

	/// @brief Check if the container is empty.
	bool IsEmpty() { return m_Data.empty(); }

	/// @brief Get the number of stored types in the container.
	size_t Size() { return m_Data.size(); }

private:
	/// @brief Map storing the data, indexed by their type.
	std::unordered_map<std::type_index, std::any> m_Data{};
};
} // namespace Data::Internal
