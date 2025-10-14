#include "Application/MeshIntegrity.h"
#include "Application/MeshLoader.h"

#include <gtest/gtest.h>

using namespace Utilitary::Surface;

TEST(MeshLoaderTest, LoadOFF_ValidFile_ShouldLoadMesh)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOFF("TestFiles/Off/cube.off");
	ASSERT_NE(mesh, nullptr);

	EXPECT_EQ(mesh->GetVertexCount(), 8);
	EXPECT_EQ(mesh->GetFaceCount(), 12);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(*mesh), MeshIntegrity::ExitCode::MeshOK);
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

TEST(MeshLoaderTest, LoadOBJ_ValidFile_ShouldLoadMesh)
{
	std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/cube.obj");
	ASSERT_NE(mesh, nullptr);

	EXPECT_EQ(mesh->GetVertexCount(), 9);
	EXPECT_EQ(mesh->GetFaceCount(), 14);

	EXPECT_EQ(MeshIntegrity::CheckIntegrity(*mesh), MeshIntegrity::ExitCode::MeshOK);
}

TEST(MeshLoaderTest, LoadOBJ_InvalidFile_ShouldReturnNullptr)
{
	{ // Wrong file extension.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Off/cube.off");
		EXPECT_EQ(mesh, nullptr);
	}

	{ // Can't open file.
		std::unique_ptr<Data::Surface::Mesh> mesh = MeshLoader::LoadOBJ("TestFiles/Obj/notAFile.obj");
		EXPECT_EQ(mesh, nullptr);
	}
}
