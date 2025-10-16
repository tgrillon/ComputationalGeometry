#include "Application/Mesh.h"
#include "Application/MeshIntegrity.h"
#include "Application/PrimitiveProxy.h"
#include "TestHelpers.h"

#include <gtest/gtest.h>

using namespace BaseType;
using namespace Utilitary::Surface;
using namespace Data::Surface;
using namespace Data::Primitive;

TEST(MeshTest, CopyConstructor_ShouldDeepCopyMesh)
{
	Mesh originalMesh = TestHelpers::CreateValidMesh();

	// Copy the mesh
	Mesh copiedMesh(originalMesh);

	// Verify that the copied mesh has the same number of vertices and faces
	EXPECT_EQ(copiedMesh.GetVertexCount(), originalMesh.GetVertexCount());
	EXPECT_EQ(copiedMesh.GetFaceCount(), originalMesh.GetFaceCount());

	// Modify the original mesh and verify that the copied mesh remains unchanged
	originalMesh.GetVertexData(0).Position = { 2., 2., 2. };
	EXPECT_NE(copiedMesh.GetVertexData(0).Position, originalMesh.GetVertexData(0).Position);

	originalMesh.GetFaceData(0).Vertices[0] = 3;
	EXPECT_NE(copiedMesh.GetFaceData(0).Vertices[0], originalMesh.GetFaceData(0).Vertices[0]);
}

TEST(MeshTest, AddVertexAndFace_ShouldReturnCorrectIndices)
{
	Mesh mesh;

	// Add vertices
	VertexIndex vIdx1 = mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentFaceIdx = { -1 } });
	VertexIndex vIdx2 = mesh.AddVertex({ .Position = { 1., 1., 1. }, .IncidentFaceIdx = { -1 } });

	// Verify vertex indices
	EXPECT_EQ(vIdx1, 0);
	EXPECT_EQ(vIdx2, 1);
	EXPECT_EQ(mesh.GetVertexCount(), 2);

	// Add a face
	FaceIndex fIdx = mesh.AddFace(
		{ .Vertices = { static_cast<int>(vIdx1), static_cast<int>(vIdx2), -1 }, .Neighbors = { -1, -1, -1 } });

	// Verify face index
	EXPECT_EQ(fIdx, 0);
	EXPECT_EQ(mesh.GetFaceCount(), 1);
}

TEST(MeshTest, GetVertexAndFace_ShouldReturnProxies)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Get vertex proxy
	VertexProxy vertexProxy = mesh.GetVertex(0);
	EXPECT_EQ(vertexProxy.GetIndex(), 0);
	EXPECT_EQ(vertexProxy.GetPosition(), (Vec3{ 0., 0., 0. }));

	// Get face proxy
	FaceProxy faceProxy = mesh.GetFace(0);
	EXPECT_EQ(faceProxy.GetIndex(), 0);
	EXPECT_EQ(faceProxy.GetVertex(0), 0);
	EXPECT_EQ(faceProxy.GetVertex(1), 1);
	EXPECT_EQ(faceProxy.GetVertex(2), 2);
}

TEST(MeshTest, GetVertexAndFaceData_ShouldReturnReferences)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Get vertex data
	Vertex& vertexData0 = mesh.GetVertexData(0);
	EXPECT_EQ(vertexData0.Position, (Vec3{ 0., 0., 0. }));

	// Modify vertex data
	vertexData0.Position = { 2., 2., 2. };
	EXPECT_EQ(mesh.GetVertexData(0).Position, (Vec3{ 2., 2., 2. }));

	// Get face data
	Face& faceData = mesh.GetFaceData(0);
	EXPECT_EQ(faceData.Vertices[0], 0);
	EXPECT_EQ(faceData.Vertices[1], 1);
	EXPECT_EQ(faceData.Vertices[2], 2);

	// Modify face data
	faceData.Vertices[0] = 3;
	EXPECT_EQ(mesh.GetFaceData(0).Vertices[0], 3);
}

TEST(MeshTest, GetConstVertexAndFaceData_ShouldReturnReferences)
{
	const Mesh mesh = TestHelpers::CreateValidMesh();

	// Get vertex data
	const Vertex& vertexData1 = mesh.GetVertexData(1);
	EXPECT_EQ(vertexData1.Position, (Vec3{ 1., 0., 0. }));

	// Get face data
	const Face& faceData1 = mesh.GetFaceData(1);
	EXPECT_EQ(faceData1.Vertices[0], 0);
	EXPECT_EQ(faceData1.Vertices[1], 2);
	EXPECT_EQ(faceData1.Vertices[2], 3);
}

TEST(MeshTest, AddExtraDataContainers_ShouldAddContainers)
{
	Mesh mesh = TestHelpers::CreateValidMesh();

	// Initially, the mesh should not have extra data containers
	EXPECT_FALSE(mesh.HasVertexExtraData());
	EXPECT_FALSE(mesh.HasFaceExtraData());

	// Add extra data containers
	mesh.AddVerticesExtraDataContainer();
	mesh.AddFacesExtraDataContainer();

	// Now, the mesh should have extra data containers
	EXPECT_TRUE(mesh.HasVertexExtraData());
	EXPECT_TRUE(mesh.HasFaceExtraData());
}

TEST(MeshTest, Clone_ShouldReturnDeepCopy)
{
	Mesh originalMesh = TestHelpers::CreateValidMesh();

	// Clone the mesh
	std::unique_ptr<Mesh> clonedMesh = originalMesh.Clone();

	// Verify that the cloned mesh has the same number of vertices and faces
	EXPECT_EQ(clonedMesh->GetVertexCount(), originalMesh.GetVertexCount());
	EXPECT_EQ(clonedMesh->GetFaceCount(), originalMesh.GetFaceCount());

	// Modify the original mesh and verify that the cloned mesh remains unchanged
	originalMesh.GetVertexData(0).Position = { 2., 2., 2. };
	EXPECT_NE(clonedMesh->GetVertexData(0).Position, originalMesh.GetVertexData(0).Position);

	originalMesh.GetFaceData(0).Vertices[0] = 3;
	EXPECT_NE(clonedMesh->GetFaceData(0).Vertices[0], originalMesh.GetFaceData(0).Vertices[0]);
}

TEST(MeshTest, UpdateMeshConnectivity_ShouldSetNeighborsAndIncidentFaces)
{
	Mesh mesh;

	// Create a simple square mesh with two faces sharing an edge
	mesh.AddVertex({ .Position = { 0., 0., 0. } }); // Vertex 0
	mesh.AddVertex({ .Position = { 1., 0., 0. } }); // Vertex 1
	mesh.AddVertex({ .Position = { 1., 1., 0. } }); // Vertex 2
	mesh.AddVertex({ .Position = { 0., 1., 0. } }); // Vertex 3

	mesh.AddFace({ .Vertices = { 0, 1, 2 } }); // Face 0
	mesh.AddFace({ .Vertices = { 0, 2, 3 } }); // Face 1

	// Update connectivity
	mesh.UpdateMeshConnectivity();

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshTest, AddVertex_ValidMeshWithED_ShouldAddExtraDataContainer)
{
	Mesh mesh = TestHelpers::CreateValidMeshWithED();

	mesh.AddVertex({ .Position = { 1, 2, 1 } });
	EXPECT_EQ(mesh.GetVertex(mesh.GetVertexCount() - 1).GetExtraData<void>(), nullptr);

	mesh.AddFace({ .Vertices = { 2, 4, 3 } });
	EXPECT_EQ(mesh.GetFace(mesh.GetFaceCount() - 1).GetExtraData<void>(), nullptr);
}
