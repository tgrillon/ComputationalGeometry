#include "Application/MeshLoader.h"

#include <gtest/gtest.h>

using namespace Utilitary;

TEST(MeshLoaderTest, LoadOFF_ValidFile_ShouldLoadMesh)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/cube.off");
	ASSERT_NE(mesh, nullptr);
	glm::vec3 p;

	EXPECT_EQ(mesh->GetVertexCount(), 8);
	EXPECT_EQ(mesh->GetFaceCount(), 12);
}

TEST(MeshLoaderTest, LoadOFF_InvalidFile_ShouldReturnNullptr)
{
	{ // Wrong file extension.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Obj/cube.obj");
		EXPECT_EQ(mesh, nullptr);
	}

	{ // Can't open file.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/notAFile.off");
		EXPECT_EQ(mesh, nullptr);
	}
}
