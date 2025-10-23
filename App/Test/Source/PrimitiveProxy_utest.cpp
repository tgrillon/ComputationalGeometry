#include "Application/PrimitiveProxy.h"
#include "Application/TestHelpers.h"

#include <gtest/gtest.h>

using namespace Utilitary::Surface;
using namespace Data::Surface;
using namespace Data::Primitive;

TEST(PrimitiveProxyTest, VertexProxy_ValidVertex_ShouldBeValid)
{
	Mesh mesh = TestHelpers::CreateValidMesh();
	VertexProxy vertexProxy = mesh.GetVertex(0);

	EXPECT_EQ(vertexProxy.GetIndex(), 0);
	EXPECT_EQ(vertexProxy.GetPosition(), (Core::BaseType::Vec3{ 0., 0., 0. }));
	EXPECT_EQ(vertexProxy.GetIncidentTriangle(), 0);

	Vertex& vertex = vertexProxy.GetVertex();
	vertex.Position = { 2., 2., 2. };
	EXPECT_EQ(vertexProxy.GetPosition(), (Core::BaseType::Vec3{ 2., 2., 2. }));
}

TEST(PrimitiveProxyTest, VertexProxy_ConstValidVertex_ShouldBeValid)
{
	Mesh mesh = TestHelpers::CreateValidMesh();
	const VertexProxy vertexProxy = mesh.GetVertex(0);

	EXPECT_EQ(vertexProxy.GetIndex(), 0);
	EXPECT_EQ(vertexProxy.GetPosition(), (Core::BaseType::Vec3{ 0., 0., 0. }));
	EXPECT_EQ(vertexProxy.GetIncidentTriangle(), 0);
}

TEST(PrimitiveProxyTest, FaceProxy_ValidFace_ShouldBeValid)
{
	Mesh mesh = TestHelpers::CreateValidMesh();
	TriangleProxy faceProxy = mesh.GetTriangle(0);

	EXPECT_EQ(faceProxy.GetIndex(), 0);
	EXPECT_EQ(faceProxy.GetVertex(0), 0);
	EXPECT_EQ(faceProxy.GetVertices(), (std::array<int, 3>{ 0, 1, 2 }));
	EXPECT_EQ(faceProxy.GetNeighbor(1), 1);
	EXPECT_EQ(faceProxy.GetNeighbors(), (std::array<int, 3>{ -1, 1, -1 }));

	Triangle& triangle = faceProxy.GetTriangle();
	triangle.Vertices[0] = 3;
	EXPECT_EQ(faceProxy.GetVertex(0), 3);
}

TEST(PrimitiveProxyTest, FaceProxy_ConstValidFace_ShouldBeValid)
{
	Mesh mesh = TestHelpers::CreateValidMesh();
	const TriangleProxy faceProxy = mesh.GetTriangle(0);

	EXPECT_EQ(faceProxy.GetIndex(), 0);
	EXPECT_EQ(faceProxy.GetVertex(0), 0);
	EXPECT_EQ(faceProxy.GetVertices(), (std::array<int, 3>{ 0, 1, 2 }));
	EXPECT_EQ(faceProxy.GetNeighbor(1), 1);
	EXPECT_EQ(faceProxy.GetNeighbors(), (std::array<int, 3>{ -1, 1, -1 }));
}

TEST(PrimitiveProxyTest, VertexProxy_ExtraData_ShouldHandleCorrectly)
{
	Mesh mesh = TestHelpers::CreateValidMesh();
	VertexProxy vertexProxy = mesh.GetVertex(0);

	struct ExtraData
	{
		int Value{ 42 };
	};

	// Initially, there should be no extra data.
	EXPECT_FALSE(vertexProxy.HasExtraData<ExtraData>());
	mesh.AddVerticesExtraDataContainer();
	EXPECT_EQ(vertexProxy.GetExtraData<ExtraData>(), nullptr);

	// Create extra data.
	ExtraData& data = vertexProxy.GetOrCreateExtraData<ExtraData>();
	EXPECT_TRUE(vertexProxy.HasExtraData<ExtraData>());
	EXPECT_EQ(data.Value, 42);

	// Modify extra data.
	data.Value = 100;
	EXPECT_EQ(vertexProxy.GetExtraData<ExtraData>()->Value, 100);

	// Set new extra data.
	ExtraData ed{ .Value = 200 };
	vertexProxy.SetExtraData(ed);
	EXPECT_EQ(vertexProxy.GetExtraData<ExtraData>()->Value, 200);
}

TEST(PrimitiveProxyTest, FaceProxy_ExtraData_ShouldHandleCorrectly)
{
	Mesh mesh = TestHelpers::CreateValidMesh();
	TriangleProxy faceProxy = mesh.GetTriangle(0);

	struct ExtraData
	{
		int Value{ 84 };
	};

	// Initially, there should be no extra data.
	EXPECT_FALSE(faceProxy.HasExtraData<ExtraData>());
	mesh.AddTrianglesExtraDataContainer();
	EXPECT_EQ(faceProxy.GetExtraData<ExtraData>(), nullptr);

	// Create extra data.
	ExtraData& data = faceProxy.GetOrCreateExtraData<ExtraData>();
	EXPECT_TRUE(faceProxy.HasExtraData<ExtraData>());
	EXPECT_EQ(data.Value, 84);

	// Modify extra data.
	data.Value = 200;
	EXPECT_EQ(faceProxy.GetExtraData<ExtraData>()->Value, 200);

	// Set new extra data.
	ExtraData ed{ .Value = 300 };
	faceProxy.SetExtraData(ed);
	EXPECT_EQ(faceProxy.GetExtraData<ExtraData>()->Value, 300);
}
