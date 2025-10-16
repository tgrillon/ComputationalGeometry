#pragma once

#include "glm/glm.hpp"

#include <cstdint>

namespace BaseType
{
using EdgeIndex = uint8_t;
using VertexIndex = uint32_t;
using FaceIndex = uint32_t;
using ScalarType = double;

using iVec2 = glm::ivec2;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
} // namespace BaseType
