#pragma once

#include "Application/Mesh.h"
#include "Application/PrimitiveProxy.h"

namespace TestHelpers
{
/// @brief Create a valid mesh with 4 vertices and 2 faces, and add extra data to vertices.
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

/// @brief Create a valid mesh with extra data for each vertex.
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

	// Add extra data containers for each face.
	mesh.AddFacesExtraDataContainer();

	// Get Face proxies to add extra data.
	Data::Primitive::FaceProxy f0 = mesh.GetFace(0);
	Data::Primitive::FaceProxy f1 = mesh.GetFace(1);

	// Add face normals
	f0.GetOrCreateExtraData<BaseType::Vec3>() = { 0, 0, 1 };
	f1.GetOrCreateExtraData<BaseType::Vec3>() = { 0, 0, 1 };

	return mesh;
}

/// @brief Create a grid mesh with (nRow+1)*(nCol+1) vertices and 2*nRow*nCol faces.
/// @image html Docs/Images/GridMesh2x2.svg "Generated mesh for nRow=nCol=2"
/// @image html Docs/Images/GridMesh3x2.svg "Generated mesh for nRow=3 and nCol=2"
inline Data::Surface::Mesh CreateGridMesh(int nRow = 1, int nCol = 1)
{
	Data::Surface::Mesh mesh;

	for(int iRow = 0; iRow <= nRow; ++iRow)
	{
		float y = static_cast<float>(iRow);
		for(int iCol = 0; iCol <= nCol; ++iCol)
		{
			// Add vertex
			float x = static_cast<float>(iCol);
			mesh.AddVertex({ .Position = { x, y, 0 } });

			// Add faces
			if(iRow > 0 && iCol > 0)
			{
				int prevRow = iRow - 1;
				int prevCol = iCol - 1;
				int nVertexCol = nCol + 1;
				mesh.AddFace({ .Vertices = { prevRow * nVertexCol + prevCol,
											 prevRow * nVertexCol + iCol,
											 iRow * nVertexCol + iCol } });
				mesh.AddFace({ .Vertices = { prevRow * nVertexCol + prevCol,
											 iRow * nVertexCol + iCol,
											 iRow * nVertexCol + prevCol } });
			}
		}
	}

	// Update mesh connectivity (neighbors and incident faces)
	mesh.UpdateMeshConnectivity();

	return mesh;
}
} // namespace TestHelpers
