#pragma once

#include "Model.h"

#include <json_fwd.hpp>

nlohmann::json ModelToJson(const Renderer::Model& model);
Renderer::Model ModelFromJson(const nlohmann::json& j);
