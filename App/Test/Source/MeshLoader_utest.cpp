#include "Application/MeshLoader.h"

#include <gtest/gtest.h>

using namespace Utilitary;

TEST(MeshLoaderTest, LoadOFF_ValidFile_ShouldLoadMesh)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/cube.off");
	ASSERT_NE(mesh, nullptr);
	EXPECT_EQ(mesh->GetVertexCount(), 8);
	EXPECT_EQ(mesh->GetFaceCount(), 12);
}
