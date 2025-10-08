#include "Application/Mesh.h"

namespace Data::Surface
{
Mesh::Mesh(const Mesh& other)
{
	m_Vertices.reserve(other.m_Vertices.size()); // Pre-allocate space
	m_Faces.reserve(other.m_Faces.size()); // Pre-allocate space

	for(const auto& vertex : other.m_Vertices)
		m_Vertices.emplace_back(std::make_unique<Primitive::Vertex>(*vertex));
	for(const auto& face : other.m_Faces)
		m_Faces.emplace_back(std::make_unique<Primitive::Triangle>(*face));
}

} // namespace Data::Surface
