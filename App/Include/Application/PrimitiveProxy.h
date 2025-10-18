#pragma once

#include "Application/BaseType.h"
#include "Application/Mesh.h"

namespace Data::Primitive
{
/// @brief Proxy class for a face in a mesh, providing safe access and extra data storage.
class FaceProxy
{
public:
	/// @brief Construct a FaceProxy from a mesh and a face.
	FaceProxy(Data::Surface::Mesh& mesh, const BaseType::FaceIndex index);

	/// @brief Get extra data of type T associated with the face, or nullptr if not found.
	template<typename T>
	T* GetExtraData()
	{
		assert(m_Mesh->HasFaceExtraData());
		return m_Mesh->m_FacesExtraDataContainer[m_Index].Get<T>();
	}

	/// @brief Get extra data of type T associated with the face, or nullptr if not found.
	template<typename T>
	const T* GetExtraData() const
	{
		assert(m_Mesh->HasFaceExtraData());
		return m_Mesh->m_FacesExtraDataContainer[m_Index].Get<const T>();
	}

	/// @brief Get or create extra data of type T associated with the face.
	template<typename T>
	T& GetOrCreateExtraData() const
	{
		assert(m_Mesh->HasFaceExtraData());
		return m_Mesh->m_FacesExtraDataContainer[m_Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the face.
	template<typename T>
	void SetExtraData(T&& data)
	{
		assert(m_Mesh->HasFaceExtraData());
		return m_Mesh->m_FacesExtraDataContainer[m_Index].Set<T>(data);
	}

	/// @brief Erase extra data of type T associated with the face.
	template<typename T>
	void EraseExtraData()
	{
		assert(m_Mesh->HasFacesExtraDataContainer() && HasExtraData<T>());
		return m_Mesh->m_FacesExtraDataContainer[m_Index].Erase<T>();
	}

	/// @brief Check if extra data of type T is associated with the face.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->HasFaceExtraData() && m_Mesh->m_FacesExtraDataContainer[m_Index].Has<T>();
	}

	/// @brief Get the index of the face in the mesh.
	BaseType::FaceIndex GetIndex() const;

	/// @brief Get the face being proxied.
	Face& GetFace();
	/// @brief Get the face being proxied (const version).
	const Face& GetFace() const;

	/// @brief Get the vertex at the given index of the face.
	BaseType::VertexIndex GetVertex(const BaseType::EdgeIndex index) const;

	/// @brief Get all vertex indices of the face.
	std::array<int, 3> GetVertices() const;

	/// @brief Get the neighbor face at the given index of the face.
	int GetNeighbor(const BaseType::EdgeIndex index) const;

	/// @brief Get all neighbor face indices of the face.
	std::array<int, 3> GetNeighbors() const;

private:
	/// @brief Pointer to the mesh containing the face.
	mutable Data::Surface::Mesh* m_Mesh;
	/// @brief Index to the face being proxied.
	BaseType::FaceIndex m_Index;
};

/// @brief Proxy class for a vertex in a mesh, providing safe access and extra data storage.
class VertexProxy
{
public:
	/// @brief Construct a VertexProxy from a mesh and a vertex.
	VertexProxy(Data::Surface::Mesh& mesh, const BaseType::VertexIndex index);

	/// @brief Get extra data of type T associated with the vertex, or nullptr if not found.
	template<typename T>
	T* GetExtraData()
	{
		assert(m_Mesh->HasVertexExtraData());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Get<T>();
	}

	/// @brief Get extra data of type T associated with the vertex, or nullptr if not found.
	template<typename T>
	const T* GetExtraData() const
	{
		assert(m_Mesh->HasVertexExtraData());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Get<T>();
	}

	/// @brief Get or create extra data of type T associated with the vertex.
	template<typename T>
	T& GetOrCreateExtraData()
	{
		assert(m_Mesh->HasVertexExtraData());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the vertex.
	template<typename T>
	void SetExtraData(T&& data)
	{
		assert(m_Mesh->HasVertexExtraData());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Set<T>(data);
	}

	/// @brief Check if extra data of type T is associated with the vertex.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->HasVerticesExtraDataContainer() && m_Mesh->m_VerticesExtraDataContainer[m_Index].Has<T>();
	}

	/// @brief Erase extra data of type T associated with the vertex.
	template<typename T>
	void EraseExtraData() const
	{
		assert(m_Mesh->HasVerticesExtraDataContainer() && HasExtraData<T>());
		return m_Mesh->m_VerticesExtraDataContainer[m_Index].Erase<T>();
	}

	/// @brief Get the index of the vertex in the mesh.
	BaseType::VertexIndex GetIndex() const;

	/// @brief Get the vertex being proxied.
	Vertex& GetVertex();
	/// @brief Get the vertex being proxied (const version).
	const Vertex& GetVertex() const;

	/// @brief Get the position of the vertex.
	BaseType::Vec3& GetPosition();

	/// @brief Get the position of the vertex (const version).
	const BaseType::Vec3& GetPosition() const;

	/// @brief Get the incident face of the vertex.
	int GetIncidentFace() const;

private:
	/// @brief Pointer to the mesh containing the vertex.
	Data::Surface::Mesh* m_Mesh;
	/// @brief Index to the vertex being proxied.
	BaseType::VertexIndex m_Index;
};
} // namespace Data::Primitive
