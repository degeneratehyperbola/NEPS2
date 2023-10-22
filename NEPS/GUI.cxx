#include "GUI.hxx"

#include <PCH.hpp>

#include "CS2/General.hxx"
#include "General.hxx"


static bool s_DemoWindowOpen = false;
static bool s_DebugWindowOpen = false;
static bool s_ScriptManagerWindowOpen = false;


void RenderContextMenu()
{
	if (ImGui::MenuItem("Unload"))
		NEPS::Unload();
}

void RenderDebugWindow()
{
	if (!s_DebugWindowOpen) return;

	if (ImGui::CollapsingHeader("Game interface tests"))
	{
		if (ImGui::TreeNode("Input System [" STRINGIFY(CS2::IInputSystem) "]"))
		{
			bool dummy[] = {
				CS2::InputSystem->WantToCaptureMouse()
			};
			ImGui::BeginDisabled();
			ImGui::Checkbox("Want to capture mouse", dummy);
			ImGui::EndDisabled();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Engine Client [" STRINGIFY(CS2::IEngineClient) "]"))
		{
			bool dummy[] = {
				CS2::EngineClient->IsInGame()
			};
			ImGui::BeginDisabled();
			ImGui::Checkbox("Is in game", dummy);
			ImGui::EndDisabled();

			int w, h;
			CS2::EngineClient->GetScreenSize(w, h);
			ImGui::Text("Screen size (WxH): %dx%d", w, h);

			ImGui::TreePop();
		}
	}
}

void RenderScriptManagerWindow()
{
	if (!s_ScriptManagerWindowOpen) return;

	if (ImGui::Begin("Script Manager"))
	{
		namespace SM = ScriptManager;
		static size_t selectedScript = 0;

		selectedScript = std::min(selectedScript, SM::TrackedScripts.size());

		if (ImGui::Button("Scan directory"))
			SM::ScanDirectory();

		ImGui::SameLine();

		if (ImGui::Button("Load"))
		{
			SM::AcquireGIL _;
			SM::TrackedScripts[selectedScript].Load();
		}

		if (ImGui::BeginListBox("##ScriptList", ImVec2(200 * GUI::GuiScale, 300 * GUI::GuiScale)))
		{
			for (size_t i = 0; i < SM::TrackedScripts.size(); i++)
			{
				if (ImGui::Selectable(SM::TrackedScripts[i].Path.filename().string().c_str(), i == selectedScript))
					selectedScript = i;
			}

			ImGui::EndListBox();
		}
	}

	ImGui::End();
}


void GUI::Render()
{
	if (!IsOpen) return;

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
		ImGui::MenuItem("Demo", nullptr, &s_DemoWindowOpen);
		ImGui::MenuItem("Debug", nullptr, &s_DebugWindowOpen);
		#endif
		ImGui::MenuItem("Scripts", nullptr, &s_ScriptManagerWindowOpen);
		
		ImGui::EndMainMenuBar();
	}

	// Render opened windows
	#ifdef _DEBUG
	if (s_DemoWindowOpen) ImGui::ShowDemoWindow(&s_DemoWindowOpen);
	RenderDebugWindow();
	RenderScriptManagerWindow();
	#endif
}
