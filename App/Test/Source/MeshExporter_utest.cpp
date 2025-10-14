#include "Application/MeshExporter.h"
#include "Application/PrimitiveProxy.h"
#include "Core/PrintHelpers.h"
#include "TestHelpers.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <string_view>

using namespace Utilitary::Surface;
using namespace Data::Surface;
using namespace Data::Primitive;
using namespace BaseType;

TEST(MeshExporterTest, ValidMesh_ExportOFFShouldSucceed)
{
	// Create a simple valid mesh and export it.
	Mesh mesh = TestHelpers::CreateValidMesh();
	const std::filesystem::path filepath = std::filesystem::relative("TestFiles/Off/validMesh.off");
	MeshExporter::ExportOFF(mesh, filepath);

	// Read back the file and verify its contents.
	std::ifstream file(filepath);
	Debug("Reading from {}", filepath.string());
	EXPECT_TRUE(file.is_open());

	std::string type;
	file >> type;
	EXPECT_EQ(type, "OFF");

	uint32_t vertexCount, faceCount, edgeCount;
	file >> vertexCount >> faceCount >> edgeCount;
	EXPECT_EQ(vertexCount, mesh.GetVertexCount());
	EXPECT_EQ(faceCount, mesh.GetFaceCount());
	EXPECT_EQ(edgeCount, 0);

	// Read and verify vertex positions.
	for(auto&& curVertex : mesh.GetVertices())
	{
		Vec3 position;
		file >> position.x >> position.y >> position.z;
		EXPECT_EQ(curVertex.Position, position);
	}

	// Read and verify face definitions.
	for(auto&& curFace : mesh.GetFaces())
	{
		file >> vertexCount;
		for(int curVertexIdx : curFace.Vertices)
		{
			int vertexIdx;
			file >> vertexIdx;
			EXPECT_EQ(curVertexIdx + 1, vertexIdx);
		}
	}

	file.close();
}

TEST(MeshExporterTest, ValidMesh_ExportOBJShouldSucceed)
{
	// Create a simple valid mesh and export it.
	Mesh mesh = TestHelpers::CreateValidMesh();
	const std::filesystem::path filepath = std::filesystem::relative("TestFiles/Obj/validMesh.obj");
	MeshExporter::ExportOBJ(mesh, filepath);

	// Read back the file and verify its contents.
	std::ifstream file(filepath);
	Debug("Reading from {}", filepath.string());
	EXPECT_TRUE(file.is_open());

	// Read and verify vertex positions.
	std::string line;
	for(auto&& curVertex : mesh.GetVertices())
	{
		char vChar;
		file >> vChar;
		EXPECT_EQ(vChar, 'v');

		Vec3 position;
		file >> position.x >> position.y >> position.z;
		EXPECT_EQ(curVertex.Position, position);
	}

	// Read and verify face definitions.
	for(auto&& curFace : mesh.GetFaces())
	{
		char fChar;
		file >> fChar;
		EXPECT_EQ(fChar, 'f');

		for(auto curVertexIdx : curFace.Vertices)
		{
			int vertexIdx;
			file >> vertexIdx;
			EXPECT_EQ(curVertexIdx + 1, vertexIdx);
		}
	}

	file.close();
}

TEST(MeshExporterTest, ValidMesh_ExportOBJWithEDShouldSucceed)
{
	// Create a simple valid mesh and export it.
	Mesh mesh = TestHelpers::CreateValidMeshWithED();
	const std::filesystem::path filepath = std::filesystem::relative("TestFiles/Obj/validMeshWithED.obj");
	MeshExporter::ExportOBJ(mesh, filepath);

	// Read back the file and verify its contents.
	std::ifstream file(filepath);
	Debug("Reading from {}", filepath.string());
	EXPECT_TRUE(file.is_open());

	// Read and verify vertex positions.
	for(auto&& curVertex : mesh.GetVertices())
	{
		char vChar;
		file >> vChar;
		EXPECT_EQ(vChar, 'v');

		Vec3 position;
		file >> position.x >> position.y >> position.z;
		EXPECT_EQ(curVertex.Position, position);
	}

	// Read and verify vertex texCoords.
	for(IndexType curVertexIdx = 0; curVertexIdx < mesh.GetVertexCount(); ++curVertexIdx)
	{
		std::string vHead;
		file >> vHead;
		EXPECT_EQ(vHead, "vt");

		Vec2 texCoords;
		file >> texCoords.x >> texCoords.y;

		const VertexProxy& curVertex = mesh.GetVertex(curVertexIdx);
		const Vec2* curVertexTexCoords = curVertex.GetExtraData<Vec2>();
		EXPECT_NE(curVertexTexCoords, nullptr);
		EXPECT_EQ(*curVertexTexCoords, texCoords);
	}

	// Read and verify vertex normals.
	for(IndexType curVertexIdx = 0; curVertexIdx < mesh.GetVertexCount(); ++curVertexIdx)
	{
		std::string vHead;
		file >> vHead;
		EXPECT_EQ(vHead, "vn");

		Vec3 normal;
		file >> normal.x >> normal.y >> normal.z;

		const VertexProxy& curVertex = mesh.GetVertex(curVertexIdx);
		const Vec3* curVertexNormal = curVertex.GetExtraData<Vec3>();
		EXPECT_NE(curVertexNormal, nullptr);
		EXPECT_EQ(*curVertexNormal, normal);
	}

	// Read and verify face definitions.
	for(auto&& curFace : mesh.GetFaces())
	{
		char fChar;
		file >> fChar;
		EXPECT_EQ(fChar, 'f');

		// Read three vertices for the triangle
		for(int curVertexIdx : curFace.Vertices)
		{
			file >> std::ws; // Skip leading whitespace
			for(int ii = 0; ii < 3; ++ii)
			{
				char buffer[16];

				// Read until first '/' or whitespace
				int i = 0;
				while(file.peek() != EOF && !std::isspace(file.peek()) && file.peek() != '/')
				{
					buffer[i++] = file.get();
				}
				buffer[i] = '\0';

				// Convert string to index
				int vertexIdx = std::stoi(buffer);
				EXPECT_EQ(curVertexIdx + 1, vertexIdx);
				file.ignore(1);
			}
		}
	}

	file.close();
}
