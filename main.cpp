#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

#include "Generator.hpp"
#include "Styles.hpp"

#include <algorithm>
#include <new>
#include <string>
#include <vector>

using namespace geode::prelude;

namespace {

constexpr float PANEL_WIDTH = 320.0f;
constexpr float PANEL_HEIGHT = 260.0f;
constexpr std::size_t MAX_DECO_OBJECTS = 900;

LevelEditorLayer* g_editorLayer = nullptr;

bool isDecoratableBlock(int objectID) {
    // Only attach decoration to common solid block IDs. Spikes, portals,
    // pads, orbs, and triggers are intentionally excluded.
    return (objectID >= 1 && objectID <= 7) || objectID == 83 || objectID == 502;
}

CCMenuItemLabel* makeLabelButton(
    const char* text,
    CCObject* target,
    SEL_MenuHandler selector,
    ccColor3B color = ccc3(255, 255, 255),
    float scale = 0.48f
) {
    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    if (!label) return nullptr;
    label->setColor(color);
    label->setScale(scale);
    return CCMenuItemLabel::create(label, target, selector);
}

class StylePanel final : public CCLayer {
public:
    static StylePanel* create(LevelEditorLayer* editor) {
        auto ret = new (std::nothrow) StylePanel();
        if (ret && ret->initWithEditor(editor)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool initWithEditor(LevelEditorLayer* editor) {
        if (!CCLayer::init() || !editor) return false;
        m_editor = editor;

        const auto winSize = CCDirector::get()->getWinSize();
        this->setContentSize(winSize);
        this->setTouchEnabled(true);

        m_panel = CCLayerColor::create(ccc4(23, 25, 36, 248), PANEL_WIDTH, PANEL_HEIGHT);
        if (!m_panel) return false;
        m_panel->setAnchorPoint({0.5f, 0.5f});
        m_panel->setPosition({winSize.width / 2.0f, winSize.height / 2.0f});
        this->addChild(m_panel, 1);

        auto header = CCLayerColor::create(ccc4(180, 35, 48, 255), PANEL_WIDTH, 35.0f);
        header->setAnchorPoint({0.5f, 0.5f});
        header->setPosition({PANEL_WIDTH / 2.0f, PANEL_HEIGHT - 17.5f});
        m_panel->addChild(header);

        auto title = CCLabelBMFont::create("YT AUTO DECO", "bigFont.fnt");
        title->setScale(0.52f);
        title->setPosition({PANEL_WIDTH / 2.0f, PANEL_HEIGHT - 17.0f});
        m_panel->addChild(title, 3);

        m_status = CCLabelBMFont::create("Choose a style (33 available)", "bigFont.fnt");
        m_status->setScale(0.36f);
        m_status->setColor(ccc3(210, 220, 235));
        m_status->setPosition({PANEL_WIDTH / 2.0f, PANEL_HEIGHT - 51.0f});
        m_panel->addChild(m_status, 3);

        m_controls = CCMenu::create();
        m_controls->setPosition({0, 0});
        m_panel->addChild(m_controls, 5);

        auto close = makeLabelButton("X", this, menu_selector(StylePanel::onClose), ccc3(255, 255, 255), 0.5f);
        if (close) {
            close->setPosition({PANEL_WIDTH - 18.0f, PANEL_HEIGHT - 17.0f});
            m_controls->addChild(close);
        }

        auto prev = makeLabelButton("< PREV", this, menu_selector(StylePanel::onPrevPage), ccc3(220, 225, 240), 0.38f);
        if (prev) {
            prev->setPosition({55.0f, 24.0f});
            m_controls->addChild(prev);
        }

        auto next = makeLabelButton("NEXT >", this, menu_selector(StylePanel::onNextPage), ccc3(220, 225, 240), 0.38f);
        if (next) {
            next->setPosition({PANEL_WIDTH - 55.0f, 24.0f});
            m_controls->addChild(next);
        }

        auto apply = makeLabelButton("AUTO DECO", this, menu_selector(StylePanel::onApply), ccc3(255, 230, 230), 0.48f);
        if (apply) {
            apply->setPosition({PANEL_WIDTH / 2.0f, 24.0f});
            m_controls->addChild(apply);
        }

        m_pageLabel = CCLabelBMFont::create("1 / 3", "bigFont.fnt");
        m_pageLabel->setScale(0.34f);
        m_pageLabel->setColor(ccc3(165, 175, 195));
        m_pageLabel->setPosition({PANEL_WIDTH / 2.0f, 54.0f});
        m_panel->addChild(m_pageLabel, 3);

        rebuildStyleGrid();
        return true;
    }

    void registerWithTouchDispatcher() override {
        CCDirector::get()->getTouchDispatcher()->addTargetedDelegate(
            this, kCCMenuHandlerPriority + 1, true
        );
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent*) override {
        if (!m_panel || !touch) return false;
        const auto local = m_panel->convertToNodeSpace(touch->getLocation());
        const CCRect panelRect(0, 0, PANEL_WIDTH, PANEL_HEIGHT);
        if (!panelRect.containsPoint(local)) return false;

        const CCRect titleRect(0, PANEL_HEIGHT - 35.0f, PANEL_WIDTH, 35.0f);
        if (titleRect.containsPoint(local)) {
            m_dragging = true;
            m_dragOffset = touch->getLocation() - m_panel->getPosition();
        }
        // Swallow taps on the panel background so they do not place blocks
        // in the editor behind this window. Child CCMenu buttons are handled
        // before this layer because they have the higher menu touch priority.
        return true;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent*) override {
        if (!m_dragging || !touch || !m_panel) return;
        auto next = touch->getLocation() - m_dragOffset;
        const auto win = CCDirector::get()->getWinSize();
        next.x = std::clamp(next.x, PANEL_WIDTH / 2.0f, win.width - PANEL_WIDTH / 2.0f);
        next.y = std::clamp(next.y, PANEL_HEIGHT / 2.0f, win.height - PANEL_HEIGHT / 2.0f);
        m_panel->setPosition(next);
    }

    void ccTouchEnded(CCTouch*, CCEvent*) override {
        m_dragging = false;
    }

private:
    void rebuildStyleGrid() {
        if (m_gridMenu) {
            m_gridMenu->removeFromParentAndCleanup(true);
            m_gridMenu = nullptr;
        }

        m_gridMenu = CCMenu::create();
        m_gridMenu->setPosition({0, 0});
        m_panel->addChild(m_gridMenu, 4);

        constexpr int pageSize = 12;
        const int start = m_page * pageSize;
        const int finish = std::min(start + pageSize, static_cast<int>(ytautodeco::STYLE_COUNT));
        const float xs[] = {55.0f, 160.0f, 265.0f};
        const float ys[] = {178.0f, 145.0f, 112.0f, 79.0f};

        for (int index = start; index < finish; ++index) {
            auto color = index == m_selected ? ccc3(255, 110, 110) : ccc3(230, 235, 245);
            auto label = CCLabelBMFont::create(ytautodeco::STYLES[static_cast<std::size_t>(index)].name, "bigFont.fnt");
            if (!label) continue;
            label->setColor(color);
            label->setScale(0.39f);
            auto item = CCMenuItemLabel::create(label, this, menu_selector(StylePanel::onChooseStyle));
            item->setTag(index);
            const int column = (index - start) % 3;
            const int row = (index - start) / 3;
            item->setPosition({xs[column], ys[row]});
            m_gridMenu->addChild(item);
        }

        if (m_pageLabel) {
            m_pageLabel->setString(CCString::createWithFormat("%d / 3", m_page + 1)->getCString());
        }
        if (m_status) {
            m_status->setString(CCString::createWithFormat(
                "Selected: %s", ytautodeco::STYLES[static_cast<std::size_t>(m_selected)].name
            )->getCString());
        }
    }

    void onChooseStyle(CCObject* sender) {
        if (!sender) return;
        m_selected = std::clamp(sender->getTag(), 0, static_cast<int>(ytautodeco::STYLE_COUNT) - 1);
        rebuildStyleGrid();
    }

    void onPrevPage(CCObject*) {
        m_page = (m_page + 2) % 3;
        rebuildStyleGrid();
    }

    void onNextPage(CCObject*) {
        m_page = (m_page + 1) % 3;
        rebuildStyleGrid();
    }

    void onClose(CCObject*) {
        this->removeFromParentAndCleanup(true);
    }

    void onApply(CCObject*) {
        if (!m_editor) {
            FLAlertLayer::create("YT Auto Deco", "The level editor is no longer available.", "OK")->show();
            return;
        }

        auto objects = m_editor->getAllObjects();
        if (!objects || objects->count() == 0) {
            FLAlertLayer::create("YT Auto Deco", "This level has no blocks to decorate yet.", "OK")->show();
            return;
        }

        std::vector<ytautodeco::BlockPoint> blocks;
        blocks.reserve(objects->count());
        for (unsigned int i = 0; i < objects->count(); ++i) {
            auto object = typeinfo_cast<GameObject*>(objects->objectAtIndex(i));
            if (!object || !isDecoratableBlock(object->m_objectID)) continue;
            blocks.push_back({object->getPositionX(), object->getPositionY()});
        }

        auto objectString = ytautodeco::makeDecorationString(blocks, m_selected, MAX_DECO_OBJECTS);
        const auto amount = ytautodeco::countSerializedObjects(objectString);
        if (amount == 0) {
            FLAlertLayer::create("YT Auto Deco", "I couldn't find any supported solid blocks. Add some blocks first.", "OK")->show();
            return;
        }

        m_editor->createObjectsFromString(gd::string(objectString), true);
        FLAlertLayer::create(
            "YT Auto Deco",
            CCString::createWithFormat("Added %zu decoration objects using %s style.",
                amount, ytautodeco::STYLES[static_cast<std::size_t>(m_selected)].name)->getCString(),
            "OK"
        )->show();
        this->removeFromParentAndCleanup(true);
    }

    LevelEditorLayer* m_editor = nullptr;
    CCLayerColor* m_panel = nullptr;
    CCMenu* m_controls = nullptr;
    CCMenu* m_gridMenu = nullptr;
    CCLabelBMFont* m_status = nullptr;
    CCLabelBMFont* m_pageLabel = nullptr;
    int m_selected = 0;
    int m_page = 0;
    bool m_dragging = false;
    CCPoint m_dragOffset = CCPointZero;
};

} // namespace

class $modify(YTAutoDecoEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;
        g_editorLayer = editorLayer;

        auto winSize = CCDirector::get()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition({31.0f, winSize.height - 48.0f});
        menu->setID("yt-auto-deco-menu"_spr);

        auto image = CCSprite::create("yt-button.png"_spr);
        if (!image) {
            log::error("YT Auto Deco button texture could not be loaded");
            return true;
        }

        auto button = CCMenuItemSpriteExtra::create(
            image,
            this,
            menu_selector(YTAutoDecoEditorUI::onOpenYTAutoDeco)
        );
        button->setID("yt-auto-deco-button"_spr);
        menu->addChild(button);
        this->addChild(menu, 10000);
        return true;
    }

    void onOpenYTAutoDeco(CCObject*) {
        if (!g_editorLayer) {
            FLAlertLayer::create("YT Auto Deco", "Open a level in the editor first.", "OK")->show();
            return;
        }
        auto scene = CCDirector::get()->getRunningScene();
        if (scene) {
            if (auto panel = StylePanel::create(g_editorLayer)) {
                scene->addChild(panel, 100000);
            }
        }
    }
};
