#include "ImGuiUtils.h"

#include <imgui.h>

#include <string>

namespace UI {

	void AlignNextItemRight(float itemWidth)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - itemWidth);
	}

	std::string TruncateTextEllipsis(const char* text, float maxWidth)
	{
		if (ImGui::CalcTextSize(text).x <= maxWidth)
			return text;

		const char* ellipsis = "...";
		float ellipsisWidth = ImGui::CalcTextSize(ellipsis).x;
		std::string result = text;

		while (!result.empty() && ImGui::CalcTextSize(result.c_str()).x + ellipsisWidth > maxWidth)
			result.pop_back();

		return result + ellipsis;
	}
}