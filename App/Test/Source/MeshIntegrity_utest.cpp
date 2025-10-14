#include "Application/MeshIntegrity.h"
#include "TestHelpers.h"

#include <gtest/gtest.h>

using namespace Utilitary::Surface;
using namespace Data::Surface;

TEST(MeshIntegrityTest, ValidMesh_ShouldReturnMeshOK)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnVertexHasNullIncidentFace)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its incident face to -1.
	mesh.GetVertexData(0).IncidentFaceIdx = -1;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::VertexHasNullIncidentFace);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnVertexNotInFace)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its incident face to a face that doesn't contain it.
	mesh.GetFaceData(0).Vertices[0] = 3;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::VertexNotInFace);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceHasNullVertex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex of the first face.
	mesh.AddFace({ .Vertices = { 0, 1, -1 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::FaceHasNullVertex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceNeighborNotReciprocal)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the neighbor relationship between the two faces.
	mesh.GetFaceData(0).Neighbors[1] = -1;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::FaceNeighborNotReciprocal);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceHasDuplicatedVertices)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the last face by duplicating two of its vertices.
	mesh.AddFace({ .Vertices = { 0, 1, 1 }, .Neighbors = { -1, -1, -1 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::FaceHasDuplicatedVertices);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceIsItsOwnNeighbor)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the neighbor index of the last face to point to itself.
	mesh.AddFace({ .Vertices = { 0, 1, 3 }, .Neighbors = { -1, -1, 2 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::FaceIsItsOwnNeighbor);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidVertexIndex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its index in a face to an out-of-bounds value.
	mesh.AddFace({ .Vertices = { 0, 1, 4 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::InvalidVertexIndex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidIncidentFaceIndex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the first vertex by setting its incident face to an out-of-bounds value.
	mesh.GetVertexData(0).IncidentFaceIdx = 2;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::InvalidIncidentFaceIndex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidNeighborFaceIndex)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Invalidate the neighbor index of the last face to an out-of-bounds value.
	mesh.AddFace({ .Vertices = { 0, 1, 3 }, .Neighbors = { -1, -1, 3 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::InvalidNeighborFaceIndex);
}
