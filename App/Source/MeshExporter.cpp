#include "Application/MeshExporter.h"

#include "Application/BaseType.h"
#include "Application/ExtraDataContainer.h"
#include "Core/PrintHelpers.h"

using namespace Data::Surface;
using namespace Data::Primitive;
using namespace Data::ExtraData;
using namespace BaseType;

#include <cassert>
#include <fstream>
#include <iostream>

namespace Utilitary::Surface
{
void MeshExporter::ExportOFF(const Mesh& mesh, const std::filesystem::path& filepath)
{
	std::ofstream file(filepath, std::ios::trunc);

	Debug("Writing to {}", filepath.string());

	// Write the OFF header.
	file << "OFF" << '\n';

	// Write the number of vertices, faces, and edges (0 for edges as per OFF format).
	file << mesh.GetVertexCount() << ' ' << mesh.GetFaceCount() << ' ' << 0 << '\n';

	// Write vertex positions.
	for(auto&& curVertex : mesh.m_Vertices)
		file << curVertex.Position.x << ' ' << curVertex.Position.y << ' ' << curVertex.Position.z << '\n';

	// Write face definitions.
	for(auto&& curFace : mesh.m_Faces)
		file << curFace.Vertices.size() << ' ' << (curFace.Vertices[0] + 1) << ' ' << (curFace.Vertices[1] + 1) << ' '
			 << (curFace.Vertices[2] + 1) << '\n';

	file.close();
}

void MeshExporter::ExportOBJ(const Mesh& mesh, const std::filesystem::path& filepath)
{
	std::ofstream file(filepath, std::ios::trunc);

	Debug("Writing to {}", filepath.string());

	// Write vertex positions.
	for(auto&& curVertex : mesh.m_Vertices)
		file << 'v' << ' ' << curVertex.Position.x << ' ' << curVertex.Position.y << ' ' << curVertex.Position.z << '\n';

	// Write texture coordinates if available.
	// Assuming texture coordinates are stored as Vec2 in the ExtraDataContainer of each vertex.
	if(mesh.HasVerticesExtraDataContainer() && mesh.m_VerticesExtraDataContainer[0].Has<Vec2>())
	{
		for(auto&& curVertexED : mesh.m_VerticesExtraDataContainer)
		{
			const Vec2* texCoords = curVertexED.Get<Vec2>();
			assert(texCoords != nullptr && "All vertices must have texture coordinates if one has it");
			file << "vt" << ' ' << texCoords->x << ' ' << texCoords->y << '\n';
		}
	}

	// Write vertex normals if available.
	// Assuming normals are stored as Vec3 in the ExtraDataContainer of each vertex.
	if(mesh.HasVerticesExtraDataContainer() && mesh.m_VerticesExtraDataContainer[0].Has<Vec3>())
	{
		for(auto&& curVertexED : mesh.m_VerticesExtraDataContainer)
		{
			const Vec3* vertexNormal = curVertexED.Get<Vec3>();
			assert(vertexNormal != nullptr && "All vertices must have a vertex normal if one has it");
			file << "vn" << ' ' << vertexNormal->x << ' ' << vertexNormal->y << ' ' << vertexNormal->z << '\n';
		}
	}

	for(auto&& curFace : mesh.m_Faces)
	{
		file << 'f';
		for(auto vertexIdx : curFace.Vertices)
		{
			// OBJ format uses 1-based indexing
			file << ' ' << (vertexIdx + 1);

			// Check if texture coordinates and normals are available
			bool hasTexCoords = mesh.HasVerticesExtraDataContainer() && mesh.m_VerticesExtraDataContainer[0].Has<Vec2>();
			bool hasNormals = mesh.HasVerticesExtraDataContainer() && mesh.m_VerticesExtraDataContainer[0].Has<Vec3>();

			if(hasTexCoords || hasNormals)
			{
				file << '/';
				if(hasTexCoords)
				{
					// Texture coordinate index is the same as vertex index in this case
					file << (vertexIdx + 1);
				}
				if(hasNormals)
				{
					file << '/';
					// Normal index is the same as vertex index in this case
					file << (vertexIdx + 1);
				}
			}
		}
		file << '\n';
	}

	file.close();
}
} // namespace Utilitary::Surface
