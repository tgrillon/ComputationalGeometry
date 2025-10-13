#include "Application/MeshIntegrity.h"

#include <gtest/gtest.h>

using namespace Utilitary::Mesh;
using namespace Data::Surface;

namespace
{
Mesh CreateValidMesh()
{
	Mesh mesh;

	// Add vertices
	mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 1., 1., 1. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 1., 0., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 0., 1., 0. }, .IncidentFaceIdx = { 1 } });

	// Add faces
	mesh.AddFace({ .Vertices = { 0, 1, 2 }, .Neighbors = { -1, 1, -1 } });
	mesh.AddFace({ .Vertices = { 0, 2, 3 }, .Neighbors = { -1, -1, 0 } });

	return mesh;
}
} // namespace

TEST(MeshIntegrityTest, ValidMesh_ShouldReturnMeshOK)
{
	Mesh mesh = CreateValidMesh();

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::MeshOK);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnVertexHasNullIncidentFace)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the first vertex by setting its incident face to -1.
	mesh.GetVertexData(0).IncidentFaceIdx = -1;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::VertexHasNullIncidentFace);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnVertexNotInFace)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the first vertex by setting its incident face to a face that doesn't contain it.
	mesh.GetFaceData(0).Vertices[0] = 3;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::VertexNotInFace);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceHasNullVertex)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the first vertex of the first face.
	mesh.AddFace({ .Vertices = { 0, 1, -1 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::FaceHasNullVertex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceNeighborNotReciprocal)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the neighbor relationship between the two faces.
	mesh.GetFaceData(0).Neighbors[1] = -1;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::FaceNeighborNotReciprocal);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceHasDuplicatedVertices)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the last face by duplicating two of its vertices.
	mesh.AddFace({ .Vertices = { 0, 1, 1 }, .Neighbors = { -1, -1, -1 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::FaceHasDuplicatedVertices);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnFaceIsItsOwnNeighbor)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the neighbor index of the last face to point to itself.
	mesh.AddFace({ .Vertices = { 0, 1, 3 }, .Neighbors = { -1, -1, 2 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::FaceIsItsOwnNeighbor);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidVertexIndex)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the first vertex by setting its index in a face to an out-of-bounds value.
	mesh.AddFace({ .Vertices = { 0, 1, 4 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::InvalidVertexIndex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidIncidentFaceIndex)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the first vertex by setting its incident face to an out-of-bounds value.
	mesh.GetVertexData(0).IncidentFaceIdx = 2;

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 2);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::InvalidIncidentFaceIndex);
}

TEST(MeshIntegrityTest, InvalidMesh_ShouldReturnInvalidNeighborFaceIndex)
{
	Mesh mesh = CreateValidMesh();

	// Invalidate the neighbor index of the last face to an out-of-bounds value.
	mesh.AddFace({ .Vertices = { 0, 1, 3 }, .Neighbors = { -1, -1, 3 } });

	EXPECT_EQ(mesh.GetVertexCount(), 4);
	EXPECT_EQ(mesh.GetFaceCount(), 3);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::Status::InvalidNeighborFaceIndex);
}
