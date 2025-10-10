#include "Application/MeshLoader.h"

#include "Application/PrimitiveProxy.h"
#include "Application/VertexPair.h"
#include "Core/PrintHelpers.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>

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

namespace Utilitary::Mesh
{
std::unique_ptr<Data::Surface::Mesh> MeshLoader::LoadOFF(const std::filesystem::path& filepath)
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

	auto mesh = std::make_unique<Data::Surface::Mesh>();

	// Retrieving the number of vertices / faces
	SkipCommentsAndWhitespace(file);
	uint32_t nVertices, nFaces, nEdges;
	file >> nVertices >> nFaces >> nEdges;

	// Reading vertices
	uint32_t vertexIndex = 0;
	mesh->m_Vertices.resize(nVertices);
	for(auto&& vertex : mesh->m_Vertices)
	{
		SkipCommentsAndWhitespace(file);
		vertex.Index = vertexIndex++;
		file >> vertex.Position.x >> vertex.Position.y >> vertex.Position.z;
	}

	// Map to store edges and their corresponding face and edge index
	std::unordered_map<VertexPair, std::pair<Face*, uint8_t>> neighborMap;

	uint32_t faceIndex = 0;
	mesh->m_Faces.resize(nFaces);
	for(auto&& face : mesh->m_Faces)
	{
		SkipCommentsAndWhitespace(file);

		// Create a new face
		face.Index = faceIndex++;

		// Set the face index
		file >> nVertices;

		for(uint8_t idx = 0; idx < nVertices; ++idx)
		{
			IndexType curVertexIdx;
			file >> curVertexIdx;

			// Get pointers to the vertex.
			Vertex& curVertex = mesh->m_Vertices[curVertexIdx];

			// Set IncidentFace for the vertex if it's not already the case.
			if(curVertex.IncidentFace == nullptr)
				curVertex.IncidentFace = &face;

			// Set vertices of the face.
			face.Vertices[idx] = &curVertex;
		}

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](const Vertex& firstVertex, const Vertex& secondVertex, uint8_t edgeIndex)
		{
			if(neighborMap.find({ firstVertex, secondVertex }) == neighborMap.end())
			{
				neighborMap[{ firstVertex, secondVertex }] = { &face, edgeIndex };
			}
			else // The edge with firstVertex and secondVertex is already registered in map
			{
				auto [faceNeighbor, neighborEdgeIndex] = neighborMap[{ firstVertex, secondVertex }];
				faceNeighbor->Neighbors[neighborEdgeIndex] = &face;
				face.Neighbors[edgeIndex] = faceNeighbor;
			}
		};

		// Edge v0-v1
		SetFacesNeigbhor(*face.Vertices[0], *face.Vertices[1], 2);
		// Edge v1-v2
		SetFacesNeigbhor(*face.Vertices[1], *face.Vertices[2], 0);
		// Edge v2-v0
		SetFacesNeigbhor(*face.Vertices[2], *face.Vertices[0], 1);
	}

	file.close();
	return mesh;
}

std::unique_ptr<Data::Surface::Mesh> MeshLoader::LoadOBJ(const std::filesystem::path& filepath)
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

	auto mesh = std::make_unique<Data::Surface::Mesh>();

	// Map to store edges and their corresponding face and edge index
	std::unordered_map<VertexPair, std::pair<Face*, uint8_t>> neighborMap;

	std::string type;
	IndexType curIndexV = 0;
	IndexType curIndexVt = 0;
	IndexType curIndexVn = 0;
	IndexType curIndexF = 0;
	while(file.peek() != EOF)
	{
		SkipCommentsAndWhitespace(file);

		if(file.peek() == EOF)
			break;

		file >> type;
		// Process line based on its type
		if(type == "v")
		{ // Vertex position
			Vertex vertex;
			vertex.Index = curIndexV++;
			file >> vertex.Position.x >> vertex.Position.y >> vertex.Position.z;
			mesh->m_Vertices.emplace_back(vertex);
			mesh->m_VertexExtraData.emplace_back();
		}
		else if(type == "vt")
		{ // Vertex texture coordinate
			const VertexProxy& curVertex = mesh->GetVertex(curIndexVt++);
			auto texCoordED = curVertex.GetOrCreateExtraData<Vec2>();
			file >> texCoordED.x >> texCoordED.y;
		}
		else if(type == "vn")
		{ // Vertex normal
			const VertexProxy& curVertex = mesh->GetVertex(curIndexVn++);
			auto normalED = curVertex.GetOrCreateExtraData<Vec3>();
			file >> normalED.x >> normalED.y >> normalED.z;
		}
		else if(type == "f")
		{ // Face (triangle)
			Face& curFace = mesh->m_Faces.emplace_back();
			curFace.Index = curIndexF++;

			// Read three vertices for the triangle
			for(uint8_t idx = 0; idx < 3; idx++)
			{
				IndexType vertexIdx;
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
				vertexIdx = std::stoi(buffer);

				Vertex& curVertex = mesh->m_Vertices[vertexIdx - 1];
				curFace.Vertices[idx] = &curVertex;

				// Skip texture/normal indices until next vertex or whitespace
				while(file.peek() != EOF && !std::isspace(file.peek()))
				{
					file.ignore(1);
				}
			}
		}
	}

	for(auto&& curFace : mesh->m_Faces)
	{
		for(auto&& vertex : curFace.Vertices)
		{
			if(vertex->IncidentFace == nullptr)
				vertex->IncidentFace = &curFace;
		}

		// Set neighboring faces using the edges
		auto SetFacesNeigbhor = [&](const Vertex& firstVertex, const Vertex& secondVertex, uint8_t edgeIndex)
		{
			if(neighborMap.find({ firstVertex, secondVertex }) == neighborMap.end())
			{
				neighborMap[{ firstVertex, secondVertex }] = { &curFace, edgeIndex };
			}
			else // The edge with firstVertex and secondVertex is already registered in map
			{
				auto [faceNeighbor, neighborEdgeIndex] = neighborMap[{ firstVertex, secondVertex }];
				faceNeighbor->Neighbors[neighborEdgeIndex] = &curFace;
				curFace.Neighbors[edgeIndex] = faceNeighbor;
			}
		};

		// Edge v0-v1
		SetFacesNeigbhor(*curFace.Vertices[0], *curFace.Vertices[1], 2);
		// Edge v1-v2
		SetFacesNeigbhor(*curFace.Vertices[1], *curFace.Vertices[2], 0);
		// Edge v2-v0
		SetFacesNeigbhor(*curFace.Vertices[2], *curFace.Vertices[0], 1);
	}

	file.close();
	return mesh;
}

} // namespace Utilitary::Mesh
