#include "Core/Application.h"

#include "Core/Renderer/GLUtils.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cassert>
#include <iostream>

namespace Core
{

static Application* s_Application = nullptr;

static void GLFWErrorCallback(int error, const char* description)
{
	std::cerr << "[GLFW Error]: " << description << std::endl;
}

Application::Application(const ApplicationSpecification& specification)
	: m_Specification(specification)
{
	s_Application = this;

	glfwSetErrorCallback(GLFWErrorCallback);
	glfwInit();

	// Set window title to app name if empty
	if(m_Specification.WindowSpec.Title.empty())
		m_Specification.WindowSpec.Title = m_Specification.Name;

	m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
	m_Window->Create();

	Renderer::Utils::InitOpenGLDebugMessageCallback();

	// ImGui initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

Application::~Application()
{
	// ImGui clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	m_Window->Destroy();

	m_LayerStack.clear();

	glfwTerminate();

	s_Application = nullptr;
}

void Application::Run()
{
	m_Running = true;

	float lastTime = GetTime();

	// Main Application loop
	while(m_Running)
	{
		glfwPollEvents();

		if(m_Window->ShouldClose())
		{
			Stop();
			break;
		}

		float currentTime = GetTime();
		float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		// Main layer update here
		for(const std::unique_ptr<Layer>& layer : m_LayerStack)
			layer->OnUpdate(timestep);

		// NOTE: rendering can be done elsewhere (eg. render thread)
		for(const std::unique_ptr<Layer>& layer : m_LayerStack)
			layer->OnRender();

		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		m_Window->Update();
	}
}

void Application::Stop()
{
	m_Running = false;
}

glm::vec2 Application::GetFramebufferSize() const
{
	return m_Window->GetFramebufferSize();
}

Application& Application::Get()
{
	assert(s_Application);
	return *s_Application;
}

GLFWwindow* Application::GetWindow() const
{
	return m_Window->GetHandle();
}

float Application::GetTime()
{
	return (float)glfwGetTime();
}

} // namespace Core
