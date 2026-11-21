#include "Generator.hpp"
#include "Styles.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace ytautodeco {
namespace {

void appendObject(
    std::ostringstream& out,
    int objectID,
    float x,
    float y,
    int rotation,
    int primaryChannel,
    int detailChannel
) {
    // Property 1 = object ID, 2 = X, 3 = Y, 6 = rotation,
    // 21/22 = primary/detail colour channels. The negative Z layer
    // places the decorative pieces behind the main gameplay blocks.
    out << "1," << objectID
        << ",2," << std::fixed << std::setprecision(2) << x
        << ",3," << y
        << ",6," << rotation
        << ",21," << primaryChannel
        << ",22," << detailChannel
        << ",24,-1;";
}

} // namespace

std::string makeDecorationString(
    const std::vector<BlockPoint>& blocks,
    int styleIndex,
    std::size_t maxObjects
) {
    if (blocks.empty() || maxObjects == 0) {
        return {};
    }

    const auto safeIndex = static_cast<std::size_t>(
        std::clamp(styleIndex, 0, static_cast<int>(STYLE_COUNT - 1))
    );
    const auto& style = STYLES[safeIndex];
    const int primary = style.swapChannels ? 2 : 1;
    const int detail = style.swapChannels ? 1 : 2;
    const int motif = style.pattern % 6;
    const int rotationBase = static_cast<int>((style.pattern / 3) % 4) * 15;

    std::ostringstream out;
    std::size_t added = 0;
    const std::size_t stride = 1 + ((style.pattern / 6) % 3);

    for (std::size_t i = 0; i < blocks.size() && added < maxObjects; ++i) {
        // Reduce clutter on some themes by skipping alternating blocks.
        if (stride > 1 && (i % stride) != 0) {
            continue;
        }

        const auto& block = blocks[i];
        const float topX = block.x + 15.0f;
        const float topY = block.y + 30.0f;

        switch (motif) {
            case 0:
                appendObject(out, 15, topX, topY, rotationBase, primary, detail);
                ++added;
                break;
            case 1:
                appendObject(out, 16, topX, topY, 90 + rotationBase, primary, detail);
                ++added;
                break;
            case 2:
                appendObject(out, 15, topX, topY, rotationBase, primary, detail);
                ++added;
                if (added < maxObjects) {
                    appendObject(out, 16, topX, topY, 90 + rotationBase, detail, primary);
                    ++added;
                }
                break;
            case 3:
                appendObject(out, 17, topX, topY, 45 + rotationBase, primary, detail);
                ++added;
                break;
            case 4:
                appendObject(out, 15, block.x + 7.5f, topY, rotationBase, primary, detail);
                ++added;
                if (added < maxObjects) {
                    appendObject(out, 16, block.x + 22.5f, topY, 90 + rotationBase, detail, primary);
                    ++added;
                }
                break;
            case 5:
            default:
                appendObject(out, 17, topX, topY + 4.0f, 90 + rotationBase, primary, detail);
                ++added;
                if (added < maxObjects) {
                    appendObject(out, 15, topX, topY - 4.0f, rotationBase, detail, primary);
                    ++added;
                }
                break;
        }
    }

    return out.str();
}

std::size_t countSerializedObjects(const std::string& objectString) {
    return static_cast<std::size_t>(std::count(
        objectString.begin(), objectString.end(), ';'
    ));
}

} // namespace ytautodeco
