#include "TitleBarLayer.h"

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Renderer/Model.h"
#include "Core/Renderer/AssetRegistry/ModelLibrary.h"
#include "Core/Scene/Actors/ModelActor.h"
#include "Core/ImGui/ImGuiUtils.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <windows.h>

#include <imgui.h>
#include <imfilebrowser.h>

#include <iostream>

TitleBarLayer::TitleBarLayer()
	: m_Scene(Core::Application::Get().GetScene()),
	m_Window(Core::Application::Get().GetWindow().GetHandle()),
	m_AssetRegistry(Core::Application::Get().GetAssetRegistry()),
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, 10.0f));
	if (ImGui::BeginMainMenuBar())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(ImGui::GetStyle().ItemSpacing.x, 10.0f));
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

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo Window"))
			{
				m_ShowDemoWindow = true;
			}

			ImGui::EndMenu();
		}
		ImGui::PopStyleVar();


		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		float totalButtonsWidth = m_RightSideButtonsWidth * 3;
		float dragRegionWidth = ImGui::GetContentRegionAvail().x - totalButtonsWidth;

		// Custom titlebar dragging
		if (dragRegionWidth > 0.0f)
		{
			ImGui::InvisibleButton("##drag_region", ImVec2(dragRegionWidth, ImGui::GetFrameHeight()));

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED))
					glfwRestoreWindow(m_Window);
				else
					glfwMaximizeWindow(m_Window);
			}
			else if (ImGui::IsItemActivated())
			{
				HWND hwnd = glfwGetWin32Window(m_Window);
				ReleaseCapture();
				SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
				ImGui::GetIO().AddMouseButtonEvent(0, false);
			}
		}

		// Right side window management buttons
		if (ImGui::Button(m_MinimizeButtonLabel.c_str(), ImVec2(m_RightSideButtonsWidth, 0.0f)))
		{
			glfwIconifyWindow(Core::Application::Get().GetWindow().GetHandle());
		}

		if (ImGui::Button(m_MaximizeButtonLabel.c_str(), ImVec2(m_RightSideButtonsWidth, 0.0f)))
		{
			if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED))
			{
				glfwRestoreWindow(m_Window);
			}
			else
			{
				glfwMaximizeWindow(m_Window);
			}
		}

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		if (ImGui::Button(m_CloseButtonLabel.c_str(), ImVec2(m_RightSideButtonsWidth, 0.0f)))
		{
			Core::Application::Get().Stop();
		}
		ImGui::PopStyleColor(); // X button red color
		ImGui::PopStyleVar(); // Remove button padding

		ImGui::EndMainMenuBar();
		ImGui::PopStyleVar(); // Title bar height override

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
			AssetRegistry::ModelId modelId = m_AssetRegistry.GetModelLibrary().Create((m_ImportObjDialog.GetSelected().string()));

			m_Scene.AddActor<Actor::ModelActor>(modelId);
			m_ImportObjDialog.ClearSelected();
		}
	}

	if (m_ShowDemoWindow)
	{
		ImGui::ShowDemoWindow();
	}
}