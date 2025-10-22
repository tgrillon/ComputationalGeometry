#include "Application/Mesh.h"
#include "Application/TestHelpers.h"

#include <gtest/gtest.h>

using namespace BaseType;
using namespace Utilitary::Surface;
using namespace Data::Surface;
using namespace Data::Primitive;

TEST(MeshCirculatorTest, GetVerticesAroundVertex_OneFace_ShouldIterateCorrectly)
{
	Mesh mesh;

	mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentTriangleIdx = 0 }); // Vertex 0
	mesh.AddVertex({ .Position = { 1., 0., 0. }, .IncidentTriangleIdx = 0 }); // Vertex 1
	mesh.AddVertex({ .Position = { 1., 1., 0. }, .IncidentTriangleIdx = 0 }); // Vertex 2

	mesh.AddTriangle({ .Vertices = { 0, 1, 2 } }); // Triangle 0

	// Get the range of vertices around vertex 0
	auto vertexRange = mesh.GetVerticesAroundVertex(0);

	// Collect the vertices around vertex 0
	std::vector<VertexIndex> collectedVertices;
	std::copy(vertexRange.begin(), vertexRange.end(), std::back_inserter(collectedVertices));

	// Vertex 0 is connected to vertices 1 and 2 in counter-clockwise order
	std::vector<VertexIndex> expectedVertices = { 2, 1 };
	EXPECT_EQ(collectedVertices, expectedVertices);
}

TEST(MeshCirculatorTest, GetVerticesAroundVertex_CirculatorWithOpenedRing_ShouldIterateCorrectly)
{
	Mesh mesh = TestHelpers::CreateGridMesh(2, 2);
	{
		// Get the range of vertices around vertex 5
		auto vertexRange = mesh.GetVerticesAroundVertex(5);

		// Collect the vertices around vertex 5
		std::vector<VertexIndex> collectedVertices;
		std::copy(vertexRange.begin(), vertexRange.end(), std::back_inserter(collectedVertices));

		// Vertex 5 is connected to the one-ring vertices in counter-clockwise order
		std::vector<VertexIndex> expectedVertices = { 2, 1, 4, 8 };
		EXPECT_EQ(collectedVertices, expectedVertices);
	}

	{
		// Get the range of vertices around vertex 3
		auto vertexRange = mesh.GetVerticesAroundVertex(3);

		// Collect the vertices around vertex 3
		std::vector<VertexIndex> collectedVertices;
		std::copy(vertexRange.begin(), vertexRange.end(), std::back_inserter(collectedVertices));

		// Vertex 3 is connected to the one-ring vertices in counter-clockwise order
		std::vector<VertexIndex> expectedVertices = { 4, 7, 6, 0 };
		EXPECT_EQ(collectedVertices, expectedVertices);
	}
}

TEST(MeshCirculatorTest, GetVerticesAroundVertex_CirculatorWithClosedRing_ShouldIterateCorrectly)
{
	Mesh mesh = TestHelpers::CreateGridMesh(2, 2);

	// Get the range of vertices around vertex 4
	auto vertexRange = mesh.GetVerticesAroundVertex(4);

	// Collect the vertices around vertex 4
	std::vector<VertexIndex> collectedVertices;
	std::copy(vertexRange.begin(), vertexRange.end(), std::back_inserter(collectedVertices));

	// Vertex 4 is connected to the one-ring vertices in counter-clockwise order
	std::vector<VertexIndex> expectedVertices = { 1, 5, 8, 7, 3, 0 };
	EXPECT_EQ(collectedVertices, expectedVertices);
}

TEST(MeshCirculatorTest, GetFacesAroundVertex_OneFace_ShouldIterateCorrectly)
{
	Mesh mesh;

	mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentTriangleIdx = 0 }); // Vertex 0
	mesh.AddVertex({ .Position = { 1., 0., 0. }, .IncidentTriangleIdx = 0 }); // Vertex 1
	mesh.AddVertex({ .Position = { 1., 1., 0. }, .IncidentTriangleIdx = 0 }); // Vertex 2

	mesh.AddTriangle({ .Vertices = { 0, 1, 2 } }); // Triangle 0

	// Get the range of vertices around vertex 0
	auto faceRange = mesh.GetTrianglesAroundVertex(0);

	// Collect the vertices around vertex 0
	std::vector<TriangleIndex> collectedFaces;
	std::copy(faceRange.begin(), faceRange.end(), std::back_inserter(collectedFaces));

	// Vertex 0 is connected to one triangle.
	std::vector<TriangleIndex> expectedFaces = { 0 };
	EXPECT_EQ(collectedFaces, expectedFaces);
}

TEST(MeshCirculatorTest, GetFacesAroundVertex_CirculatorWithOpenedRing_ShouldIterateCorrectly)
{
	Mesh mesh = TestHelpers::CreateGridMesh(2, 2);

	// Get the range of vertices around vertex 5
	auto faceRange = mesh.GetTrianglesAroundVertex(5);

	// Collect the vertices around vertex 5
	std::vector<TriangleIndex> collectedFaces;
	std::copy(faceRange.begin(), faceRange.end(), std::back_inserter(collectedFaces));

	// Vertex 5 is connected to the one-ring vertices in counter-clockwise order
	std::vector<TriangleIndex> expectedFaces = { 2, 3, 6 };
	EXPECT_EQ(collectedFaces, expectedFaces);
}

TEST(MeshCirculatorTest, GetFacesAroundVertex_CirculatorWithClosedRing_ShouldIterateCorrectly)
{
	Mesh mesh = TestHelpers::CreateGridMesh(2, 2);

	// Get the range of vertices around vertex 4
	auto faceRange = mesh.GetTrianglesAroundVertex(4);

	// Collect the vertices around vertex 4
	std::vector<TriangleIndex> collectedFaces;
	std::copy(faceRange.begin(), faceRange.end(), std::back_inserter(collectedFaces));

	// Vertex 4 is connected to the one-ring incident faces in counter-clockwise order
	std::vector<TriangleIndex> expectedFaces = { 0, 3, 6, 7, 4, 1 };
	EXPECT_EQ(collectedFaces, expectedFaces);
}
