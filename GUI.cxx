#include "GUI.hxx"

#include <PCH.hpp>

#include "CS2/General.hxx"

#include "Globals.hxx"
#include "Helpers.hxx"

static bool s_isDemoWindowOpen = true;
static bool s_isDebugWindowOpen = true;

void RenderDebugWindow();
void RenderContextMenu();

void GUI::Render()
{
	if (!isOpen) return;

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

	if (ImGui::CollapsingHeader("Game interfaces tests"))
	{
		if (ImGui::TreeNode("Input System [" _STRINGIFY(CS2::IInputSystem) "]"))
		{
			bool dummy[] = {
				CS2::InputSystem->IsRelativeMouseMode()
			};
			ImGui::BeginDisabled();
			ImGui::Checkbox("Want to capture mouse", dummy);
			ImGui::EndDisabled();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Engine Client [" _STRINGIFY(CS2::IEngineClient) "]"))
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

		if (ImGui::TreeNode("Console Variable Index [" _STRINGIFY(CS2::IConVarIndex) "]"))
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
