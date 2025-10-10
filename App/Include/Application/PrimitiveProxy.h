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
	FaceProxy(Data::Surface::Mesh& mesh, Data::Primitive::Face& face);

	/// @brief Check if the proxy is valid (i.e., the mesh and face are valid).
	bool IsValid() const;

	/// @brief Get extra data of type T associated with the face, or nullptr if not found.
	template<typename T>
	T* GetExtraData() const
	{
		return m_Mesh->m_FaceExtraData[m_Face->Index].Get<T>();
	}

	/// @brief Get or create extra data of type T associated with the face.
	template<typename T>
	T& GetOrCreateExtraData() const
	{
		return m_Mesh->m_FaceExtraData[m_Face->Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the face.
	template<typename T>
	void SetExtraData(T&& data)
	{
		return m_Mesh->m_FaceExtraData[m_Face->Index].Set<T>(data);
	}

	/// @brief Check if extra data of type T is associated with the face.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->m_FaceExtraData[m_Face->Index].Has<T>();
	}

	/// @brief Get the index of the face in the mesh.
	BaseType::IndexType GetIndex() const;

	/// @brief Get the vertex at the given index (0, 1, or 2) of the face.
	Vertex* GetVertex(const uint8_t index) const;

	/// @brief Get all three vertices of the face.
	std::array<Vertex*, 3> GetVertices() const;

	/// @brief Get the neighboring face at the given index (0, 1, or 2) of the face.
	Face* GetNeighbor(const uint8_t index) const;

	/// @brief Get all three neighboring faces of the face.
	std::array<Face*, 3> GetNeighbors() const;

private:
	/// @brief Pointer to the mesh containing the face.
	Data::Surface::Mesh* m_Mesh;
	/// @brief Pointer to the face being proxied.
	Data::Primitive::Face* m_Face;
};

/// @brief Proxy class for a vertex in a mesh, providing safe access and extra data storage.
class VertexProxy
{
public:
	/// @brief Construct a VertexProxy from a mesh and a vertex.
	VertexProxy(Data::Surface::Mesh& mesh, Data::Primitive::Vertex& vertex);

	VertexProxy(const VertexProxy& other);
	VertexProxy(VertexProxy&& other);

	VertexProxy& operator=(const VertexProxy& other);
	VertexProxy& operator=(VertexProxy&& other);

	/// @brief Check if the proxy is valid (i.e., the mesh and vertex are valid).
	bool IsValid() const;

	/// @brief Get extra data of type T associated with the vertex, or nullptr if not found.
	template<typename T>
	T* GetExtraData() const
	{
		return m_Mesh->m_VertexExtraData[m_Vertex->Index].Get<T>();
	}

	/// @brief Get or create extra data of type T associated with the vertex.
	template<typename T>
	T& GetOrCreateExtraData() const
	{
		return m_Mesh->m_VertexExtraData[m_Vertex->Index].GetOrCreate<T>();
	}

	/// @brief Set extra data of type T associated with the vertex.
	template<typename T>
	void SetExtraData(T&& data)
	{
		return m_Mesh->m_VertexExtraData[m_Vertex->Index].Set<T>(data);
	}

	/// @brief Check if extra data of type T is associated with the vertex.
	template<typename T>
	bool HasExtraData() const
	{
		return m_Mesh->m_VertexExtraData[m_Vertex->Index].Has<T>();
	}

	/// @brief Get the index of the vertex in the mesh.
	BaseType::IndexType GetIndex() const;

	/// @brief Get the position of the vertex.
	BaseType::Vec3& GetPosition();

	/// @brief Get the position of the vertex (const version).
	const BaseType::Vec3& GetPosition() const;

	/// @brief Get the incident face of the vertex.
	Face* GetIncidentFace();

	/// @brief Get the incident face of the vertex (const version).
	const Face* GetIncidentFace() const;

private:
	/// @brief Pointer to the mesh containing the vertex.
	Data::Surface::Mesh* m_Mesh;
	/// @brief Pointer to the vertex being proxied.
	Data::Primitive::Vertex* m_Vertex;
};
} // namespace Data::Primitive
