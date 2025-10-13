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
	FaceProxy(Data::Surface::Mesh& mesh, const BaseType::IndexType faceIdx);

	/// @brief Get extra data of type T associated with the face, or nullptr if not found.
	template<typename T>
	T* GetExtraData() const
	{
		return m_Mesh->m_FaceExtraData[m_Index].Get<T>();
	}

	/// @brief Get or create extra data of type T associated with the face.
	template<typename T>
	T& GetOrCreateExtraData() const
	{
		return m_Mesh->m_FaceExtraData[m_Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the face.
	template<typename T>
	void SetExtraData(T&& data)
	{
		return m_Mesh->m_FaceExtraData[m_Index].Set<T>(data);
	}

	/// @brief Check if extra data of type T is associated with the face.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->m_FaceExtraData[m_Index].Has<T>();
	}

	/// @brief Get the index of the face in the mesh.
	BaseType::IndexType GetIndex() const;

	/// @brief Get the face being proxied.
	Face& GetFace();
	/// @brief Get the face being proxied (const version).
	const Face& GetFace() const;

	/// @brief Get the vertex at the given index of the face.
	BaseType::IndexType GetVertex(const uint8_t index) const;

	/// @brief Get all vertex indices of the face.
	std::array<int, 3> GetVertices() const;

	/// @brief Get the neighbor face at the given index of the face.
	int GetNeighbor(const uint8_t index) const;

	/// @brief Get all neighbor face indices of the face.
	std::array<int, 3> GetNeighbors() const;

private:
	/// @brief Pointer to the mesh containing the face.
	Data::Surface::Mesh* m_Mesh;
	/// @brief Index to the face being proxied.
	BaseType::IndexType m_Index;
};

/// @brief Proxy class for a vertex in a mesh, providing safe access and extra data storage.
class VertexProxy
{
public:
	/// @brief Construct a VertexProxy from a mesh and a vertex.
	VertexProxy(Data::Surface::Mesh& mesh, const BaseType::IndexType vertexIdx);

	/// @brief Get extra data of type T associated with the vertex, or nullptr if not found.
	template<typename T>
	T* GetExtraData()
	{
		return m_Mesh->m_VertexExtraData[m_Index].Get<T>();
	}

	/// @brief Get or create extra data of type T associated with the vertex.
	template<typename T>
	T& GetOrCreateExtraData()
	{
		return m_Mesh->m_VertexExtraData[m_Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the vertex.
	template<typename T>
	void SetExtraData(T&& data)
	{
		return m_Mesh->m_VertexExtraData[m_Index].Set<T>(data);
	}

	/// @brief Check if extra data of type T is associated with the vertex.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->m_VertexExtraData[m_Index].Has<T>();
	}

	/// @brief Get the index of the vertex in the mesh.
	BaseType::IndexType GetIndex() const;

	/// @brief Get the vertex being proxied.
	Vertex& GetVertex();
	/// @brief Get the vertex being proxied (const version).
	const Vertex& GetVertex() const;

	/// @brief Get the position of the vertex.
	BaseType::Vec3& GetPosition();

	/// @brief Get the position of the vertex (const version).
	const BaseType::Vec3& GetPosition() const;

	/// @brief Get the incident face of the vertex.
	int GetIncidentFace();

	/// @brief Get the incident face of the vertex (const version).
	const int GetIncidentFace() const;

private:
	/// @brief Pointer to the mesh containing the vertex.
	Data::Surface::Mesh* m_Mesh;
	/// @brief Index to the vertex being proxied.
	BaseType::IndexType m_Index;
};
} // namespace Data::Primitive
