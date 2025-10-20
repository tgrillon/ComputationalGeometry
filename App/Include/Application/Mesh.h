#pragma once

#include "Application/BaseType.h"
#include "Application/ExtraDataContainer.h"
#include "Application/Primitive.h"

#include <memory>
#include <vector>

/// Forward declaration
namespace Utilitary::Surface
{
class MeshExporter;
class MeshIntegrity;
class MeshLoader;
} // namespace Utilitary::Surface

namespace Data::Primitive
{
class TriangleProxy;
class VertexProxy;
} // namespace Data::Primitive

namespace Data::Surface
{
/// @brief Class representing a 3D triangular mesh.
class Mesh
{
public:
	friend Utilitary::Surface::MeshExporter;
	friend Utilitary::Surface::MeshIntegrity;
	friend Utilitary::Surface::MeshLoader;

	friend Data::Primitive::TriangleProxy;
	friend Data::Primitive::VertexProxy;

public:
	/// @brief Default ctor.
	Mesh() = default;
	/// @brief Copy ctor.
	Mesh(const Mesh& other);
	~Mesh() = default;

	/// @brief Deep clone of the mesh.
	std::unique_ptr<Mesh> Clone() const;

	/// @brief Get the number of vertices in the mesh.
	uint32_t GetVertexCount() const;

	/// @brief Get the number of triangles in the mesh.
	uint32_t GetTriangleCount() const;

	/// @brief Get a proxy to the vertex at the given index.
	Data::Primitive::VertexProxy GetVertex(const BaseType::VertexIndex index);
	/// @brief Get a proxy to the triangle at the given index.
	Data::Primitive::TriangleProxy GetTriangle(const BaseType::TriangleIndex index);

	/// @brief Get the vertex data at the given index.
	Data::Primitive::Vertex& GetVertexData(const BaseType::VertexIndex index);
	/// @brief Get the vertex data at the given index.
	const Data::Primitive::Vertex& GetVertexData(const BaseType::VertexIndex index) const;
	/// @brief Get the triangle data at the given index.
	Data::Primitive::Triangle& GetTriangleData(const BaseType::TriangleIndex index);
	/// @brief Get the triangle data at the given index.
	const Data::Primitive::Triangle& GetTriangleData(const BaseType::TriangleIndex index) const;

	/// @brief Add a vertex to the mesh and return its index.
	BaseType::VertexIndex AddVertex(const Data::Primitive::Vertex& vertex);
	/// @brief Add a triangle to the mesh and return its index.
	BaseType::TriangleIndex AddTriangle(const Data::Primitive::Triangle& triangle);

	/// @brief Add extra data container for each vertex.
	void AddVerticesExtraDataContainer();
	/// @brief Add extra data container for each triangle.
	void AddTrianglesExtraDataContainer();

	/// @brief Update neighbor informations on each triangle and incident triangle for each vertex.
	void UpdateMeshConnectivity();

	/// @brief Get the vertices data.
	std::vector<Data::Primitive::Vertex>& GetVertices();
	/// @brief Get the vertices data.
	const std::vector<Data::Primitive::Vertex>& GetVertices() const;
	/// @brief Get the triangles data.
	std::vector<Data::Primitive::Triangle>& GetTriangles();
	/// @brief Get the triangles data.
	const std::vector<Data::Primitive::Triangle>& GetTriangles() const;

	/// @brief Check if the mesh has extra data containers for vertices.
	bool HasVerticesExtraDataContainer() const;
	/// @brief Check if the mesh has extra data containers for triangles.
	bool HasTrianglesExtraDataContainer() const;

	/// @brief Compute normal for each triangle of the mesh (Normalized).
	/// @param computeSmoothVertexNormals If true, compute smooth vertex normals (Normalized).
	/// @note The computed normals are stored as extra data on the triangles and vertices.
	void ComputeTriangleNormals(bool computeSmoothVertexNormals = false);

public:
	/// @brief Circulator to iterate over the vertices around a given vertex.
	/// @note The circulator will iterate in counter-clockwise direction first, then in clock-wise direction if it reaches a boundary.
	/// @image html Docs/Images/VerticesAroundVertexClosedRing.svg "Case where the one-ring neighborhood is closed."
	/// @image html Docs/Images/VerticesAroundVertexOpenedRing.svg "Case where the one-ring neighborhood is opened."
	class VerticesAroundVertexCirculator
	{
	public:
		using iterator_category = std::input_iterator_tag;
		using value_type = BaseType::VertexIndex;
		using difference_type = std::ptrdiff_t;
		using pointer = BaseType::VertexIndex*;
		using reference = BaseType::VertexIndex&;

	public:
		/// @brief Construct a circulator to iterate over the vertices around a given vertex in the mesh.
		/// @param mesh The mesh containing the vertex.
		/// @param index The index of the vertex around which to circulate.
		VerticesAroundVertexCirculator(const Data::Surface::Mesh& mesh, const BaseType::VertexIndex index);

		/// @brief Equality operator.
		bool operator==(const VerticesAroundVertexCirculator& rhs) const;
		/// @brief Inequality operator.
		bool operator!=(const VerticesAroundVertexCirculator& rhs) const;

		/// @brief Pre-increment operator.
		VerticesAroundVertexCirculator& operator++();

		/// @brief Dereference operator to get the current vertex index.
		BaseType::VertexIndex operator*() const;

		/// @brief Set whether the circulator is active or not.
		void SetIsActive(bool value);

	private:
		/// @brief Update the current vertex index in counter-clock-wise direction.
		void UpdateCurVertexIndexInCCWOrder();
		/// @brief Update the current vertex index in clock-wise direction.
		void UpdateCurVertexIndexInCWOrder();

	private:
		/// @brief Reference to the mesh.
		const Mesh& m_Mesh;

		/// @brief Index of the central vertex around which we circulate.
		BaseType::VertexIndex m_CentralVertexIdx;

		/// @brief Current triangle index in the circulation.
		int m_CurTriangleIdx;
		/// @brief Previous triangle index in the circulation.
		int m_PrevTriangleIdx;

		/// @brief Current vertex local index in the current triangle.
		BaseType::EdgeIndex m_CurVertexLocalIdx;
		/// @brief Current vertex index in the circulation.
		BaseType::VertexIndex m_CurVertexIdx;

		/// @brief Number of jumps made in the circulation (used to detect boundaries).
		uint8_t m_JumpCount{ 0 };
		/// @brief Whether we are currently circulating in counter-clockwise direction.
		bool m_IsInCCWOrder{ true };
		/// @brief Whether the circulator is active (to detect the end of the circulation).
		bool m_IsActive{ true };
	};

	/// @brief Range to iterate over the vertices around a given vertex.
	/// @note The range will iterate in counter-clockwise direction first, then in clock-wise direction if it reaches a boundary.
	class VerticesAroundVertexRange
	{
	public:
		friend VerticesAroundVertexCirculator;

	public:
		/// @brief Construct a range to iterate over the vertices around a given vertex in the mesh.
		/// @param mesh The mesh containing the vertex.
		/// @param index The index of the vertex around which to circulate.
		VerticesAroundVertexRange(const Mesh& mesh, const BaseType::VertexIndex index);

		/// @brief Get the begin circulator.
		VerticesAroundVertexCirculator begin() const;
		/// @brief Get the end circulator.
		VerticesAroundVertexCirculator end() const;

	private:
		/// @brief Reference to the mesh.
		const Mesh& m_Mesh;
		/// @brief Index of the central vertex around which we circulate.
		BaseType::VertexIndex m_VertexIdx;
	};

	/// @brief Get a range to iterate over the vertices around a given vertex.
	/// @param index The index of the vertex around which to circulate.
	/// @return A range to iterate over the vertices around the given vertex.
	VerticesAroundVertexRange GetVerticesAroundVertex(const BaseType::VertexIndex index) const;

	/// @brief Circulator to iterate over the triangles around a given vertex.
	/// @note The circulator will iterate in counter-clockwise direction first, then in clock-wise direction if it reaches a boundary.
	/// @image html Docs/Images/FacesAroundVertexClosedRing.svg "Case where the one-ring neighborhood is closed."
	/// @image html Docs/Images/FacesAroundVertexOpenedRing.svg "Case where the one-ring neighborhood is opened."
	class TrianglesAroundVertexCirculator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = BaseType::TriangleIndex;
		using difference_type = std::ptrdiff_t;
		using pointer = const BaseType::TriangleIndex*;
		using reference = const BaseType::TriangleIndex&;

	public:
		/// @brief Construct a circulator to iterate over the triangles around a given vertex in the mesh.
		/// @param mesh The mesh containing the vertex.
		/// @param index The index of the vertex around which to circulate.
		TrianglesAroundVertexCirculator(const Mesh& mesh, const BaseType::VertexIndex index);

		/// @brief Equality operator.
		bool operator==(const TrianglesAroundVertexCirculator& rhs) const;
		/// @brief Inequality operator.
		bool operator!=(const TrianglesAroundVertexCirculator& rhs) const;

		/// @brief Pre-increment operator.
		TrianglesAroundVertexCirculator& operator++();

		/// @brief Dereference operator to get the current triangle index.
		BaseType::TriangleIndex operator*() const;

		/// @brief Set whether the circulator is active or not.
		void SetIsActive(bool value);

	private:
		/// @brief Reference to the mesh.
		const Mesh& m_Mesh;

		/// @brief Index of the central vertex around which we circulate.
		BaseType::VertexIndex m_CentralVertexIdx;

		/// @brief Current triangle index in the circulation.
		int m_CurTriangleIdx;
		/// @brief Previous triangle index in the circulation.
		int m_PrevTriangleIdx;

		/// @brief Number of jumps made in the circulation (used to detect boundaries).
		uint8_t m_JumpCount{ 0 };
		/// @brief Whether we are currently circulating in counter-clockwise direction.
		bool m_IsInCCWOrder{ true };
		/// @brief Whether the circulator is active (to detect the end of the circulation).
		bool m_IsActive{ true };
	};

	/// @brief Range to iterate over the triangles around a given vertex.
	/// @note The range will iterate in counter-clockwise direction first, then in clock-wise direction if it reaches a boundary.
	class TrianglesAroundVertexRange
	{
	public:
		friend TrianglesAroundVertexCirculator;

	public:
		/// @brief Construct a range to iterate over the triangles around a given vertex in the mesh.
		/// @param mesh The mesh containing the vertex.
		/// @param index The index of the vertex around which to circulate.
		TrianglesAroundVertexRange(const Mesh& mesh, const BaseType::VertexIndex index);

		/// @brief Get the begin circulator.
		TrianglesAroundVertexCirculator begin() const;
		/// @brief Get the end circulator.
		TrianglesAroundVertexCirculator end() const;

	private:
		/// @brief Reference to the mesh.
		const Mesh& m_Mesh;
		/// @brief Index of the central vertex around which we circulate.
		BaseType::VertexIndex m_VertexIdx;
	};

	/// @brief Get a range to iterate over the triangles around a given vertex.
	/// @param index The index of the vertex around which to circulate.
	/// @return A range to iterate over the triangles around the given vertex.
	TrianglesAroundVertexRange GetTrianglesAroundVertex(const BaseType::VertexIndex index) const;

private:
	/// @brief List of vertices.
	std::vector<Data::Primitive::Vertex> m_Vertices{};

	/// @brief List of triangles.
	std::vector<Data::Primitive::Triangle> m_Triangles{};

	/// @brief Extra data containers for each vertex.
	std::vector<Data::ExtraData::ExtraDataContainer> m_VerticesExtraDataContainer{};
	/// @brief Extra data containers for each triangle.
	std::vector<Data::ExtraData::ExtraDataContainer> m_TrianglesExtraDataContainer{};
};
} // namespace Data::Surface
