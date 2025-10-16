#include "Application/MeshLoader.h"

#include "Application/PrimitiveProxy.h"
#include "Application/VertexPair.h"
#include "Core/PrintHelpers.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>

using namespace Data::Surface;
using namespace Data::Primitive;
using namespace BaseType;

namespace
{
// Helper lambda to skip comments and empty lines
void SkipCommentsAndWhitespace(std::ifstream& file)
{
	while(file.peek() == '#' || file.peek() == '\n' || file.peek() == '\r')
	{
		if(file.peek() == '#')
		{
			// Skip the entire comment line
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			// Skip whitespace
			file.get();
		}
	}
}
} // namespace

namespace Utilitary::Surface
{
std::unique_ptr<Mesh> MeshLoader::LoadOFF(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);

	// Checking file opening
	if(!file.is_open())
	{
		Error("Failed to open file: {}", filepath.string());
		return nullptr;
	}

	// Checking file type
	SkipCommentsAndWhitespace(file);
	std::string type;
	file >> type;
	if(type != "OFF")
	{
		Error("Wrong file format (must be OFF) : {}", filepath.string());
		return nullptr;
	}

	auto mesh = std::make_unique<Mesh>();

	// Retrieving the number of vertices / faces
	SkipCommentsAndWhitespace(file);
	int vertexCount, faceCount, unusedEdgeCount;
	file >> vertexCount >> faceCount >> unusedEdgeCount;

	// Reading vertices
	mesh->m_Vertices.resize(vertexCount);
	for(auto&& curVertex : mesh->m_Vertices)
	{
		SkipCommentsAndWhitespace(file);
		file >> curVertex.Position.x >> curVertex.Position.y >> curVertex.Position.z;
	}

	// Map to store edges and their corresponding face and edge index
	std::unordered_map<VertexPair, std::pair<FaceIndex, EdgeIndex>> neighborMap;

	mesh->m_Faces.resize(faceCount);
	for(int iFace = 0; iFace < faceCount; ++iFace)
	{
		Face& curFace = mesh->m_Faces[iFace];

		SkipCommentsAndWhitespace(file);

		// Set the face index
		file >> vertexCount;

		for(int iEdge = 0; iEdge < vertexCount; ++iEdge)
		{
			VertexIndex curVertexIdx;
			file >> curVertexIdx;

			// Get pointers to the vertex.
			Vertex& curVertex = mesh->m_Vertices[curVertexIdx];

			// Set IncidentFaceIdx for the vertex if it's not already the case.
			if(curVertex.IncidentFaceIdx == -1)
				curVertex.IncidentFaceIdx = iFace;

			// Set vertices of the face.
			curFace.Vertices[iEdge] = curVertexIdx;
		}

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](VertexIndex firstVertexIdx, VertexIndex secondVertexIdx, uint8_t edgeIdx)
		{
			if(neighborMap.find({ firstVertexIdx, secondVertexIdx }) == neighborMap.end())
			{
				neighborMap[{ firstVertexIdx, secondVertexIdx }] = { iFace, edgeIdx };
			}
			else // The edge with firstVertex and secondVertex is already registered in map
			{
				auto [faceNeighborIdx, neighborEdgeIdx] = neighborMap[{ firstVertexIdx, secondVertexIdx }];
				mesh->m_Faces[faceNeighborIdx].Neighbors[neighborEdgeIdx] = iFace;
				curFace.Neighbors[edgeIdx] = faceNeighborIdx;
			}
		};

		const VertexIndex v0Idx = static_cast<VertexIndex>(curFace.Vertices[0]);
		const VertexIndex v1Idx = static_cast<VertexIndex>(curFace.Vertices[1]);
		const VertexIndex v2Idx = static_cast<VertexIndex>(curFace.Vertices[2]);

		// Edge v0-v1
		SetFacesNeigbhor(v0Idx, v1Idx, 2);
		// Edge v1-v2
		SetFacesNeigbhor(v1Idx, v2Idx, 0);
		// Edge v2-v0
		SetFacesNeigbhor(v2Idx, v0Idx, 1);
	}

	file.close();
	return mesh;
}

std::unique_ptr<Mesh> MeshLoader::LoadOBJ(const std::filesystem::path& filepath)
{
	std::ifstream file(filepath);

	// Checking file opening
	if(!file.is_open())
	{
		Error("Failed to open file: {}", filepath.string());
		return nullptr;
	}

	// Check file extension
	if(filepath.extension() != ".obj")
	{
		Error("Wrong file extension (must be .obj): {}", filepath.string());
		return nullptr;
	}

	auto mesh = std::make_unique<Mesh>();

	// Map to store edges and their corresponding face and edge index
	std::unordered_map<VertexPair, std::pair<FaceIndex, EdgeIndex>> neighborMap;

	std::string type;
	VertexIndex curIndexVt = 0;
	VertexIndex curIndexVn = 0;
	while(file.peek() != EOF)
	{
		SkipCommentsAndWhitespace(file);

		if(file.peek() == EOF)
			break;

		file >> type;
		// Process line based on its type
		if(type == "v")
		{ // Vertex position
			Vertex curVertex;
			file >> curVertex.Position.x >> curVertex.Position.y >> curVertex.Position.z;
			mesh->m_Vertices.emplace_back(curVertex);
			mesh->m_VerticesExtraDataContainer.emplace_back();
		}
		else if(type == "vt")
		{ // Vertex texture coordinate
			VertexProxy curVertex = mesh->GetVertex(curIndexVt++);
			auto texCoordED = curVertex.GetOrCreateExtraData<Vec2>();
			file >> texCoordED.x >> texCoordED.y;
		}
		else if(type == "vn")
		{ // Vertex normal
			VertexProxy curVertex = mesh->GetVertex(curIndexVn++);
			auto normalED = curVertex.GetOrCreateExtraData<Vec3>();
			file >> normalED.x >> normalED.y >> normalED.z;
		}
		else if(type == "f")
		{ // Face (triangle)
			FaceIndex curFaceIdx = mesh->GetFaceCount();
			Face& curFace = mesh->m_Faces.emplace_back();

			// Read three vertices for the triangle
			for(int iEdge = 0; iEdge < 3; ++iEdge)
			{
				VertexIndex curVertexIdx;
				char buffer[16];
				file >> std::ws; // Skip leading whitespace

				// Read until first '/' or whitespace
				int i = 0;
				while(file.peek() != EOF && !std::isspace(file.peek()) && file.peek() != '/')
				{
					buffer[i++] = file.get();
				}
				buffer[i] = '\0';

				// Convert string to index
				curVertexIdx = std::stoi(buffer) - 1;

				Vertex& curVertex = mesh->m_Vertices[curVertexIdx];
				if(curVertex.IncidentFaceIdx == -1)
					curVertex.IncidentFaceIdx = curFaceIdx;

				curFace.Vertices[iEdge] = curVertexIdx;

				// Skip texture/normal indices until next vertex or whitespace
				while(file.peek() != EOF && !std::isspace(file.peek()))
				{
					file.ignore(1);
				}
			}

			// Set neighboring faces using the edges
			auto SetFacesNeigbhor = [&](VertexIndex firstVertexIdx, VertexIndex secondVertexIdx, EdgeIndex edgeIdx)
			{
				if(neighborMap.find({ firstVertexIdx, secondVertexIdx }) == neighborMap.end())
				{
					neighborMap[{ firstVertexIdx, secondVertexIdx }] = { curFaceIdx, edgeIdx };
				}
				else // The edge with firstVertex and secondVertex is already registered in map
				{
					auto [faceNeighborIdx, neighborEdgeIdx] = neighborMap[{ firstVertexIdx, secondVertexIdx }];
					mesh->m_Faces[faceNeighborIdx].Neighbors[neighborEdgeIdx] = curFaceIdx;
					curFace.Neighbors[edgeIdx] = faceNeighborIdx;
				}
			};

			const VertexIndex v0Idx = static_cast<VertexIndex>(curFace.Vertices[0]);
			const VertexIndex v1Idx = static_cast<VertexIndex>(curFace.Vertices[1]);
			const VertexIndex v2Idx = static_cast<VertexIndex>(curFace.Vertices[2]);

			// Edge v0-v1
			SetFacesNeigbhor(v0Idx, v1Idx, 2);
			// Edge v1-v2
			SetFacesNeigbhor(v1Idx, v2Idx, 0);
			// Edge v2-v0
			SetFacesNeigbhor(v2Idx, v0Idx, 1);
		}
	}

	file.close();
	return mesh;
}

} // namespace Utilitary::Surface
