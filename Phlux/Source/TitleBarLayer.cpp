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
	: m_Scene(Core::Application::Get().GetScene()),
	m_ImportObjDialog(ImGui::FileBrowser()),
	m_SaveSceneDialog(ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir)),
	m_LoadSceneDialog(ImGui::FileBrowser())
{
	m_SaveSceneDialog.SetTitle("Save Scene");

	m_LoadSceneDialog.SetTitle("Load Scene");
	m_LoadSceneDialog.SetTypeFilters({ ".json" });

	m_ImportObjDialog.SetTitle("Import Mesh");
	m_ImportObjDialog.SetTypeFilters({ ".obj" });
}

void TitleBarLayer::OnUpdate(float deltaTime)
{
	if (Core::Application::Get().GetInputManager().IsKeyPressed(GLFW_KEY_EQUAL))
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

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				m_SaveSceneDialog.Open();
			}

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
			{

			}

			if (ImGui::MenuItem("Load"))
			{
				m_LoadSceneDialog.Open();
			}

			if (ImGui::BeginMenu("Import"))
			{
				if (ImGui::MenuItem("OBJ"))
				{
					m_ImportObjDialog.Open();
				}

				if (ImGui::MenuItem("GLTF"))
				{

				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

		// Save scene
		m_SaveSceneDialog.Display();
		if (m_SaveSceneDialog.HasSelected())
		{
			m_Scene.Save(m_SaveSceneDialog.GetSelected().string());
			m_SaveSceneDialog.ClearSelected();
		}

		// Load scene
		m_LoadSceneDialog.Display();
		if (m_LoadSceneDialog.HasSelected())
		{
			m_Scene.Load(m_LoadSceneDialog.GetSelected().string());
			m_LoadSceneDialog.ClearSelected();
		}

		// Import OBj
		m_ImportObjDialog.Display();
		if (m_ImportObjDialog.HasSelected())
		{
			std::cout << "Selected filename " << m_ImportObjDialog.GetSelected().string() << std::endl;
			Renderer::Model model = Renderer::Model(m_ImportObjDialog.GetSelected().string());

			m_Scene.AddActor<Actor::ModelActor>(std::move(model));
			m_ImportObjDialog.ClearSelected();
		}
	}

	if (m_ShowDemoWindow)
	{
		ImGui::ShowDemoWindow();
	}
}