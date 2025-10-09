#include "Application/AppLayer.h"

#include "Core/Application.h"
#include "Core/PrintHelpers.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>

namespace Application
{
AppLayer::AppLayer()
{
	// Create shaders
	m_Shader = Renderer::CreateGraphicsShader("Data/Shaders/Vertex.glsl", "Data/Shaders/Fragment.glsl");

	// Create geometry
	glCreateVertexArrays(1, &m_VertexArray);
	glCreateBuffers(1, &m_VertexBuffer);

	struct Vertex
	{
		glm::vec2 Position;
		glm::vec2 TexCoord;
	};

	Vertex vertices[] = {
		{ { -1.0f, -1.0f }, { 0.0f, 0.0f } }, // Bottom-left
		{ { 3.0f, -1.0f }, { 2.0f, 0.0f } }, // Bottom-right
		{ { -1.0f, 3.0f }, { 0.0f, 2.0f } } // Top-left
	};

	glNamedBufferData(m_VertexBuffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the VBO to VAO at binding index 0
	glVertexArrayVertexBuffer(m_VertexArray, 0, m_VertexBuffer, 0, sizeof(Vertex));

	// Enable attributes
	glEnableVertexArrayAttrib(m_VertexArray, 0); // position
	glEnableVertexArrayAttrib(m_VertexArray, 1); // uv

	// Format: location, size, type, normalized, relative offset
	glVertexArrayAttribFormat(m_VertexArray, 0, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, Position)));
	glVertexArrayAttribFormat(m_VertexArray, 1, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, TexCoord)));

	// Link attribute locations to binding index 0
	glVertexArrayAttribBinding(m_VertexArray, 0, 0);
	glVertexArrayAttribBinding(m_VertexArray, 1, 0);

	// Print various OpenGL informations to stdout
	std::cout << glGetString(GL_VENDOR) << ": " << glGetString(GL_RENDERER) << '\n';
	std::cout << "GLFW\t " << glfwGetVersionString() << '\n';
	std::cout << "OpenGL\t " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL\t " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";
}

AppLayer::~AppLayer()
{
	glDeleteVertexArrays(1, &m_VertexArray);
	glDeleteBuffers(1, &m_VertexBuffer);

	glDeleteProgram(m_Shader);

	static Renderer::Texture renderTexture;
	static Renderer::Framebuffer renderFramebuffer;

	if(renderTexture.Handle != 0)
	{
		glDeleteTextures(1, &renderTexture.Handle);
		glDeleteFramebuffers(1, &renderFramebuffer.Handle);
	}
}

void AppLayer::OnUpdate(float ts)
{}

void AppLayer::OnRender()
{
	// ImGui stuff
	ImGui::Begin("My Application");

	const float windowWidth = ImGui::GetContentRegionAvail().x;
	const float windowHeight = ImGui::GetContentRegionAvail().y;

	static Renderer::Texture renderTexture;
	static Renderer::Framebuffer renderFramebuffer;

	if(renderTexture.Width != windowWidth || renderTexture.Height != windowHeight)
	{
		// Clean up old resources
		if(renderTexture.Handle != 0)
		{
			glDeleteTextures(1, &renderTexture.Handle);
			glDeleteFramebuffers(1, &renderFramebuffer.Handle);
		}

		// Create new texture and framebuffer
		renderTexture = Renderer::CreateTexture(windowWidth, windowHeight);
		renderFramebuffer = Renderer::CreateFramebufferWithTexture(renderTexture);
	}

	ImVec2 pos = ImGui::GetCursorScreenPos();

	ImGui::GetWindowDrawList()->AddImage(
		(void*)static_cast<uintptr_t>(renderTexture.Handle),
		ImVec2(pos.x, pos.y),
		ImVec2(pos.x + windowWidth, pos.y + windowHeight),
		ImVec2(0, 1),
		ImVec2(1, 0));

	ImGui::End();

	// Drawing stuff
	glBindFramebuffer(GL_FRAMEBUFFER, renderFramebuffer.Handle);
	glViewport(0, 0, static_cast<GLsizei>(windowWidth), static_cast<GLsizei>(windowHeight));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	double relativeCursorX = 0.0;
	double relativeCursorY = 0.0;
	glfwGetCursorPos(Core::Application::Get().GetWindow(), &relativeCursorX, &relativeCursorY);

	glUseProgram(m_Shader);
	glUniform1f(0, Core::Application::GetTime());
	glUniform2f(1, windowWidth, windowHeight);

	// Render
	glBindVertexArray(m_VertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace Application
