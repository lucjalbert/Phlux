#pragma once

#include <imgui.h>

#include <string>

namespace UI {

	void AlignNextItemRight(float itemWidth);
	std::string TruncateTextEllipsis(const char* text, float maxWidth);
}