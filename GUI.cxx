#include "GUI.hxx"

#include <imgui/imgui.h>

bool g_bDemoWindowOpen = true;

void GUI::Render()
{
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

	ImGui::EndMainMenuBar();

	// Render opened windows
	if (g_bDemoWindowOpen)
		ImGui::ShowDemoWindow(&g_bDemoWindowOpen);
}
