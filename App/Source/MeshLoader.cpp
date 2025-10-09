#include "Application/MeshLoader.h"

#include "Application/VertexPair.h"
#include "Core/PrintHelpers.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>

using namespace Data::Primitive;

namespace Utilitary
{
std::unique_ptr<Data::Surface::Mesh> MeshLoader::LoadOFF(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);

	// Checking file opening
	if(!file.is_open())
	{
		PrintHelpers::Error("Failed to open file: {}", filepath.string());
		return nullptr;
	}

	// Checking file type
	std::string type;
	file >> type;
	if(type != "OFF")
	{
		PrintHelpers::Error("[MeshLoader::LoadOFF()] Wrong file format (must be OFF) : {}", filepath.string());
		return nullptr;
	}

	auto mesh = std::make_unique<Data::Surface::Mesh>();

	// Retrieving the number of vertices / faces
	uint32_t nVertices, nFaces, nEdges;
	file >> nVertices >> nFaces >> nEdges;

	// Reading vertices
	uint32_t vertexIndex = 0;
	mesh->m_Vertices.resize(nVertices);
	for(auto&& vertex : mesh->m_Vertices)
	{
		vertex.Index = vertexIndex++;
		file >> vertex.Position.x >> vertex.Position.y >> vertex.Position.z;
	}

	// Map to store edges and their corresponding face and edge index
	std::unordered_map<VertexPair, std::pair<Face*, uint8_t>> neighborMap;

	uint32_t faceIndex = 0;
	mesh->m_Faces.resize(nFaces);
	for(auto&& face : mesh->m_Faces)
	{
		// Create a new face
		face.Index = faceIndex++;

		// Set the face index
		uint32_t vIdx0, vIdx1, vIdx2;
		file >> nVertices >> vIdx0 >> vIdx1 >> vIdx2;

		// Get pointers to the vertices
		Vertex& vertex0 = mesh->m_Vertices[vIdx0];
		Vertex& vertex1 = mesh->m_Vertices[vIdx1];
		Vertex& vertex2 = mesh->m_Vertices[vIdx2];

		// Set IncidentFace for each vertex if it's not already the case.
		Face* incidentFaceV0 = vertex0.IncidentFace;
		if(incidentFaceV0 == nullptr)
			incidentFaceV0 = &face;
		Face* incidentFaceV1 = vertex1.IncidentFace;
		if(incidentFaceV1 == nullptr)
			incidentFaceV1 = &face;
		Face* incidentFaceV2 = vertex2.IncidentFace;
		if(incidentFaceV2 == nullptr)
			incidentFaceV2 = &face;

		// Set vertices of the face
		face.Vertices[0] = &vertex0;
		face.Vertices[1] = &vertex1;
		face.Vertices[2] = &vertex2;

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](const Vertex& v0, const Vertex& v1, uint8_t edgeIndex)
		{
			if(neighborMap.find({ v0, v1 }) == neighborMap.end())
			{
				neighborMap[{ v0, v1 }] = { &face, 2 };
			}
			else // The edge with v0 and v1 is already registered in map
			{
				auto [FaceNeighbor, edgeIndex] = neighborMap[{ v0, v1 }];
				FaceNeighbor->Neighbors[edgeIndex] = &face;
				face.Neighbors[edgeIndex] = FaceNeighbor;
			}
		};

		// Edge v0-v1
		SetFacesNeigbhor(vertex0, vertex1, 2);
		// Edge v1-v2
		SetFacesNeigbhor(vertex1, vertex2, 0);
		// Edge v2-v0
		SetFacesNeigbhor(vertex2, vertex0, 1);
	}

	file.close();
	return mesh;
}

std::unique_ptr<Data::Surface::Mesh> MeshLoader::LoadOBJ(const std::filesystem::path& filepath)
{
	return std::unique_ptr<Data::Surface::Mesh>();
}

} // namespace Utilitary
