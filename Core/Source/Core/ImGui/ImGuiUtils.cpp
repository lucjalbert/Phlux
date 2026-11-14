#include "ImGuiUtils.h"

#include <imgui.h>

namespace UI {

	void AlignNextItemRight(float itemWidth)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - itemWidth);
	}
}