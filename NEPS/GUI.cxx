#include "GUI.hxx"

#include <PCH.hpp>

#include "CS2/General.hxx"
#include "General.hxx"


static bool s_DemoWindowOpen = false;
static bool s_DebugWindowOpen = false;
static bool s_ScriptManagerWindowOpen = false;

void RenderDebugWindow();
void RenderContextMenu();
void RenderScriptsWindow();


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
		ImGui::MenuItem("Demo Window", nullptr, &s_DemoWindowOpen);
		ImGui::MenuItem("Debug Window", nullptr, &s_DebugWindowOpen);
		ImGui::MenuItem("Scripts", nullptr, &s_ScriptsWindowOpen);
		#endif
		
		ImGui::EndMainMenuBar();
	}

	// Render opened windows
	#ifdef _DEBUG
	if (s_DemoWindowOpen) ImGui::ShowDemoWindow(&s_DemoWindowOpen);
	RenderDebugWindow();
	RenderScriptsWindow();
	#endif
}

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
				CS2::EngineClient->IsConnected(),
				CS2::EngineClient->IsInGame()
			};
			ImGui::BeginDisabled();
			ImGui::Checkbox("Is connected", dummy);
			ImGui::Checkbox("Is in game", dummy + 1);
			ImGui::EndDisabled();

			ImGui::Text("Level name: %s", CS2::EngineClient->GetLevelName());
			ImGui::Text("Level name short: %s", CS2::EngineClient->GetLevelNameShort());

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Console Variable Index [" STRINGIFY(CS2::IConVarIndex) "]"))
		{
			static CS2::ConVar* var;
			static size_t idx;

			if (ImGui::InputInt("Variable index", (int*)&idx))
				var = CS2::ConVarIndex->GetVar(idx);

			ImGui::SeparatorText("Info");
			if (var)
			{
				ImGui::Text("Name: %s", var->Name());

				if (ImGui::BeginTabBar("##VarValues"))
				{
					if (ImGui::BeginTabItem("Integer"))
					{
						ImGui::Text("%i", var->GetInt());
						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Float"))
					{
						ImGui::Text("%f", var->GetFloat());
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
			}
			else ImGui::TextDisabled("nullptr");

			ImGui::TreePop();
		}
	}
}

void RenderScriptsWindow()
{
	if (!s_ScriptManagerWindowOpen) return;

	if (ImGui::Begin("Script Manager"))
	{
		namespace SM = ScriptManager;
		static size_t selectedScript = 0;

		selectedScript = std::min(selectedScript, SM::TrackedScripts.size());

		if (ImGui::Button("Scan directory"))
		{
			SM::GIL lock;
			SM::ScanDirectory();
		}

		ImGui::SameLine();

		if (ImGui::Button("Load"))
		{
			SM::GIL lock;
			SM::LoadScript(SM::TrackedScripts[selectedScript]);
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
