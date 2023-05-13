#include "GUI.hxx"

#include <imgui/imgui.h>

#include "Globals.hxx"
#include "Helpers.hxx"

bool g_bGUIOpen = true;

bool g_bDemoWindowOpen = true;

void RenderContextMenu()
{
	if (ImGui::MenuItem("Unload"))
		NEPS::Unload();
}

void GUI::Render()
{
	// Handle menu toggle logic
	if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
		g_bGUIOpen = !g_bGUIOpen;

	if (!g_bGUIOpen) return;

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
		ImGui::MenuItem("Debug Window", nullptr, &g_bDemoWindowOpen);
		#endif

		ImGui::EndMainMenuBar();
	}


	// Render opened windows
	if (g_bDemoWindowOpen)
		ImGui::ShowDemoWindow(&g_bDemoWindowOpen);
}
