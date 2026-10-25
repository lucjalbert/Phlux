#include "TitleBarLayer.h"

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Renderer/Model.h"
#include "Core/Scene/Actors/ModelActor.h"

#include "GLFW/glfw3.h"

#include <imgui.h>
#include <imfilebrowser.h>

#include <iostream>

TitleBarLayer::TitleBarLayer()
	: m_FileDialog(ImGui::FileBrowser())
{
	m_FileDialog.SetTitle("Import Mesh");
	m_FileDialog.SetTypeFilters({ ".obj" });
}

void TitleBarLayer::OnUpdate(float deltaTime)
{
	if (Core::Application::Get().GetInputManager()->IsKeyPressed(GLFW_KEY_EQUAL))
	{
		m_ShowDemoWindow = !m_ShowDemoWindow;
	}
}

void TitleBarLayer::OnImGuiRender()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{

			}

			if (ImGui::MenuItem("Open Scene"))
			{

			}

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{

			}

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
			{

			}

			if (ImGui::BeginMenu("Import"))
			{
				if (ImGui::MenuItem("OBJ"))
				{
					m_FileDialog.Open();
				}

				if (ImGui::MenuItem("GLTF"))
				{

				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		m_FileDialog.Display();

		if (m_FileDialog.HasSelected())
		{
			std::cout << "Selected filename " << m_FileDialog.GetSelected().string() << std::endl;
			Renderer::Model model = Renderer::Model(m_FileDialog.GetSelected().string());

			Actor::ModelActor modelActor = Actor::ModelActor(model);
			Core::Application::Get().GetScene()->AddActor<Actor::ModelActor>(std::move(model));
			m_FileDialog.ClearSelected();
		}
	}

	if (m_ShowDemoWindow)
	{
		ImGui::ShowDemoWindow();
	}
}