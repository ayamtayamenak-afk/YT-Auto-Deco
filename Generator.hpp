#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace ytautodeco {

struct BlockPoint {
    float x;
    float y;
};

// Creates a Geometry Dash object string containing non-hazard rod decorations.
// The returned string can be passed to LevelEditorLayer::createObjectsFromString.
std::string makeDecorationString(
    const std::vector<BlockPoint>& blocks,
    int styleIndex,
    std::size_t maxObjects = 900
);

std::size_t countSerializedObjects(const std::string& objectString);

} // namespace ytautodeco
