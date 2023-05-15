#include "GUI.hxx"

#include <imgui/imgui.h>
#include <MemoryTools.hxx>

#include "CS2/General.hxx"

#include "Globals.hxx"
#include "Helpers.hxx"

static bool s_isGUIOpen = true;

static bool s_isDemoWindowOpen = true;
static bool s_isDebugWindowOpen = true;

void RenderDebugWindow();
void RenderContextMenu();

bool GUI::IsOpen() { return s_isGUIOpen; }

void GUI::Render()
{
	// Handle menu toggle logic
	if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
		s_isGUIOpen = !s_isGUIOpen;

	if (!s_isGUIOpen) return;

	// Context menu logic
	if (!ImGui::GetIO().WantCaptureMouse && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		ImGui::OpenPopup("##ContextMenu");

	if (ImGui::BeginPopup("##ContextMenu"))
	{
		RenderContextMenu();
		ImGui::EndPopup();
	}

	// Render the main menu bar
	if (ImGui::BeginMainMenuBar())
	{
		// Logo
		if (ImGui::BeginMenu("NEPS2"))
		{
			RenderContextMenu();
			ImGui::EndMenu();
		}

		ImGui::Separator();

		#ifdef _DEBUG
		ImGui::TextDisabled("%s Debug Build", __DATE__);
		#else
		ImGui::TextDisabled("%s Build", __DATE__);
		#endif

		ImGui::Separator();

		// Menu bar items
		#ifdef _DEBUG
		ImGui::MenuItem("Demo Window", nullptr, &s_isDemoWindowOpen);
		ImGui::MenuItem("Debug Window", nullptr, &s_isDebugWindowOpen);
		#endif
		
		ImGui::EndMainMenuBar();
	}

	// Render opened windows
	#ifdef _DEBUG
	if (s_isDemoWindowOpen) ImGui::ShowDemoWindow(&s_isDemoWindowOpen);
	RenderDebugWindow();
	#endif
}

void RenderContextMenu()
{
	if (ImGui::MenuItem("Unload"))
		NEPS::Unload();
}

void RenderDebugWindow()
{
	if (!s_isDebugWindowOpen) return;

	if (ImGui::CollapsingHeader("Game Interfaces Tests"))
	{
		// if (ImGui::TreeNode("Input System [" _STRINGIFY(CS2::IInputSystem) "]"))
		// {
		// 	ImGui::TreePop();
		// }
	}
}
