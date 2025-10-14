#pragma once

#include "Application/Mesh.h"
#include "Application/PrimitiveProxy.h"

namespace TestHelpers
{
inline Data::Surface::Mesh CreateValidMesh()
{
	Data::Surface::Mesh mesh;

	// Add vertices
	mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 1., 0., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 1., 1., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 0., 1., 0. }, .IncidentFaceIdx = { 1 } });

	// Add faces
	mesh.AddFace({ .Vertices = { 0, 1, 2 }, .Neighbors = { -1, 1, -1 } });
	mesh.AddFace({ .Vertices = { 0, 2, 3 }, .Neighbors = { -1, -1, 0 } });

	return mesh;
}

inline Data::Surface::Mesh CreateValidMeshWithED()
{
	Data::Surface::Mesh mesh;

	// Add vertices
	mesh.AddVertex({ .Position = { 0., 0., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 1., 0., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 1., 1., 0. }, .IncidentFaceIdx = { 0 } });
	mesh.AddVertex({ .Position = { 0., 1., 0. }, .IncidentFaceIdx = { 1 } });

	// Add extra data containers for each vertex.
	mesh.AddVerticesExtraDataContainer();

	// Get Vertex proxies to add extra data.
	Data::Primitive::VertexProxy v0 = mesh.GetVertex(0);
	Data::Primitive::VertexProxy v1 = mesh.GetVertex(1);
	Data::Primitive::VertexProxy v2 = mesh.GetVertex(2);
	Data::Primitive::VertexProxy v3 = mesh.GetVertex(3);

	// Add vertices texCoords
	v0.GetOrCreateExtraData<BaseType::Vec2>() = { 0, 0 };
	v1.GetOrCreateExtraData<BaseType::Vec2>() = { 1, 0 };
	v2.GetOrCreateExtraData<BaseType::Vec2>() = { 1, 1 };
	v3.GetOrCreateExtraData<BaseType::Vec2>() = { 0, 1 };

	// Add vertices normal
	v0.GetOrCreateExtraData<BaseType::Vec3>() = { 0, 0, 1 };
	v1.GetOrCreateExtraData<BaseType::Vec3>() = { 0, 0, 1 };
	v2.GetOrCreateExtraData<BaseType::Vec3>() = { 0, 0, 1 };
	v3.GetOrCreateExtraData<BaseType::Vec3>() = { 0, 0, 1 };

	// Add faces
	mesh.AddFace({ .Vertices = { 0, 1, 2 }, .Neighbors = { -1, 1, -1 } });
	mesh.AddFace({ .Vertices = { 0, 2, 3 }, .Neighbors = { -1, -1, 0 } });

	return mesh;
}
} // namespace TestHelpers
