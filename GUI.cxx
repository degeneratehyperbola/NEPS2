#include "GUI.hxx"

#include <imgui/imgui.h>

#include "Globals.hxx"
#include "Helpers.hxx"

bool g_bGUIOpen = true;

bool g_bDemoWindowOpen = true;

void GUI::Render()
{
	// Handle menu toggle logic
	if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
		g_bGUIOpen = !g_bGUIOpen;

	if (!g_bGUIOpen) return;

	// Render the main menu bar
	ImGui::BeginMainMenuBar();

	// Logo
	ImGui::TextUnformatted("NEPS2");
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

	// Unload button
	ImGui::Separator();
	if (ImGui::MenuItem("Unload"))
		NEPS::Unload();

	ImGui::EndMainMenuBar();

	// Render opened windows
	if (g_bDemoWindowOpen)
		ImGui::ShowDemoWindow(&g_bDemoWindowOpen);
}
