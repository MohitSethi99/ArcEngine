#include "SettingsPanel.h"

#include "Arc/Physics/Physics2D.h"

#include "../Utils/UI.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace ArcEngine
{
	void SettingsPanel::OnImGuiRender()
	{
		ImGui::Begin("Settings");

		UI::BeginPropertyGrid();
		ImGui::PushID("Gravity2D");
		UI::Property("Gravity 2D", Physics2D::Gravity);
		ImGui::PopID();
		UI::EndPropertyGrid();

		UI::BeginPropertyGrid();
		ImGui::PushID("2DPhysicsTimestep");
		UI::Property("2D Physics Timestep", Physics2D::Timestep, 0.1f, 0.001f, "%.4f");
		ImGui::PopID();
		UI::EndPropertyGrid();

		UI::BeginPropertyGrid();
		ImGui::PushID("VelocityIterations");
		UI::Property("Velocity Iterations", Physics2D::VelocityIterations, 1, 50);
		ImGui::PopID();
		UI::EndPropertyGrid();

		UI::BeginPropertyGrid();
		ImGui::PushID("PositionIterations");
		UI::Property("Position Iterations", Physics2D::PositionIterations, 1, 50);
		ImGui::PopID();
		UI::EndPropertyGrid();
		
		ImGui::End();
	}
}
