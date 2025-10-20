#include "Application/MeshIntegrity.h"
#include "Application/TestHelpers.h"

#include <gtest/gtest.h>

using namespace Utilitary::Surface;
using namespace Data::Surface;

TEST(MeshIntegrityTest, ValidMesh_ShouldReturnMeshOK)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnVertexHasNullIncidentFace)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its incident triangle to -1.
	mesh.GetVertexData(0).IncidentTriangleIdx = -1;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::VertexHasNullIncidentTriangle);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnVertexNotInFace)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its incident triangle to a triangle that doesn't contain it.
	mesh.GetTriangleData(0).Vertices[0] = 3;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::VertexNotInTriangle);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnTriangleHasNullVertex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex of the first triangle.
	mesh.AddTriangle({ .Vertices = { 0, 1, -1 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::TriangleHasNullVertex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnTriangleNeighborNotReciprocal)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the neighbor relationship between the two triangles.
	mesh.GetTriangleData(0).Neighbors[1] = -1;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::TriangleNeighborNotReciprocal);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnTriangleHasDuplicatedVertices)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the last triangle by duplicating two of its vertices.
	mesh.AddTriangle({ .Vertices = { 0, 1, 1 }, .Neighbors = { -1, -1, -1 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::TriangleHasDuplicatedVertices);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnTriangleIsItsOwnNeighbor)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the neighbor index of the last triangle to point to itself.
	mesh.AddTriangle({ .Vertices = { 0, 1, 3 }, .Neighbors = { -1, -1, 2 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::TriangleIsItsOwnNeighbor);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidVertexIndex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its index in a triangle to an out-of-bounds value.
	mesh.AddTriangle({ .Vertices = { 0, 1, 4 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::InvalidVertexIndex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidIncidentTriangleIndex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its incident triangle to an out-of-bounds value.
	mesh.GetVertexData(0).IncidentTriangleIdx = 2;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::InvalidIncidentTriangleIndex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidNeighborTriangleIndex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the neighbor index of the last triangle to an out-of-bounds value.
	mesh.AddTriangle({ .Vertices = { 0, 1, 3 }, .Neighbors = { -1, -1, 3 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetTriangleCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::InvalidNeighborTriangleIndex);
}
