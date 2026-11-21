#include "Generator.hpp"
#include "Styles.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    using namespace ytautodeco;

    static_assert(STYLE_COUNT == 33, "YT Auto Deco must expose 33 styles");
    assert(std::string(STYLES[0].name) == "Nightmare");
    assert(std::string(STYLES[10].name) == "Dessert");
    assert(std::string(STYLES[32].name) == "Classic");

    const std::vector<BlockPoint> blocks{{0.f, 0.f}, {30.f, 0.f}, {60.f, 30.f}};
    for (int i = 0; i < static_cast<int>(STYLE_COUNT); ++i) {
        const auto generated = makeDecorationString(blocks, i, 100);
        assert(!generated.empty());
        assert(countSerializedObjects(generated) > 0);
        assert(countSerializedObjects(generated) <= 100);
    }

    const auto capped = makeDecorationString(blocks, 2, 1);
    assert(countSerializedObjects(capped) == 1);
    assert(makeDecorationString({}, 0).empty());
    assert(makeDecorationString(blocks, 0, 0).empty());

    std::cout << "YT Auto Deco generator tests passed (33 styles).\n";
    return 0;
}
