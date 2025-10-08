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
		PrintHelpers::Error("MeshLoader::LoadOFF() take a file with the .off extension : {}", filepath.string());
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
		vertex = std::make_unique<Vertex>();
		vertex->Index = vertexIndex++;
		file >> vertex->Position.x >> vertex->Position.y >> vertex->Position.z;
	}

	// Map to store edges and their corresponding face and edge index
	std::unordered_map<VertexPair, std::pair<Triangle*, uint8_t>> neighborMap;

	uint32_t faceIndex = 0;
	mesh->m_Faces.resize(nFaces);
	for(auto&& face : mesh->m_Faces)
	{
		// Create a new face
		face = std::make_unique<Triangle>();
		face->Index = faceIndex++;

		// Set the face index
		uint32_t vIdx0, vIdx1, vIdx2;
		file >> nVertices >> vIdx0 >> vIdx1 >> vIdx2;

		// Get pointers to the vertices
		Vertex* vertexPtr0 = mesh->m_Vertices[vIdx0].get();
		Vertex* vertexPtr1 = mesh->m_Vertices[vIdx1].get();
		Vertex* vertexPtr2 = mesh->m_Vertices[vIdx2].get();

		// Set IncidentFace for each vertex if it's not already the case.
		Triangle* incidentFaceV0 = vertexPtr0->IncidentFace;
		if(incidentFaceV0 == nullptr)
			incidentFaceV0 = face.get();
		Triangle* incidentFaceV1 = vertexPtr1->IncidentFace;
		if(incidentFaceV1 == nullptr)
			incidentFaceV1 = face.get();
		Triangle* incidentFaceV2 = vertexPtr2->IncidentFace;
		if(incidentFaceV2 == nullptr)
			incidentFaceV2 = face.get();

		// Set vertices of the face
		face->Vertices[0] = vertexPtr0;
		face->Vertices[1] = vertexPtr1;
		face->Vertices[2] = vertexPtr2;

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](const Vertex& v0, const Vertex& v1, uint8_t edgeIndex)
		{
			if(neighborMap.find({ v0, v1 }) == neighborMap.end())
			{
				neighborMap[{ v0, v1 }] = { face.get(), 2 };
			}
			else // The edge with v0 and v1 is already registered in map
			{
				auto [FaceNeighbor, edgeIndex] = neighborMap[{ v0, v1 }];
				FaceNeighbor->Neighbors[edgeIndex] = face.get();
				face->Neighbors[edgeIndex] = FaceNeighbor;
			}
		};

		// Edge v0-v1
		SetFacesNeigbhor(*vertexPtr0, *vertexPtr1, 2);
		// Edge v1-v2
		SetFacesNeigbhor(*vertexPtr1, *vertexPtr2, 0);
		// Edge v2-v0
		SetFacesNeigbhor(*vertexPtr2, *vertexPtr0, 1);
	}

	file.close();
	return mesh;
}

} // namespace Utilitary
