
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(YTAutoDecoEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        constexpr float R = 20.f;

        auto bg = CCNode::create();
        bg->setContentSize({R * 2, R * 2});

        auto draw = CCDrawNode::create();

        draw->drawCircle(
            {R, R},
            R - 1.5f,
            {0.85f, 0.10f, 0.10f, 1.f},
            2.5f,
            {1.f, 1.f, 1.f, 1.f},
            48
        );

        bg->addChild(draw);

        auto label = CCLabelBMFont::create("YT", "bigFont.fnt");
        label->setScale(0.55f);
        label->setPosition({R, R + 1});
        bg->addChild(label);

        auto btn = CCMenuItemSpriteExtra::create(
            bg,
            this,
            menu_selector(YTAutoDecoEditorUI::onYTClick)
        );

        auto menu = CCMenu::create();
        menu->addChild(btn);
        this->addChild(menu);

        auto winSize = CCDirector::get()->getWinSize();

        CCPoint pos = ccp(95.f, winSize.height / 2 + 75.f);

        if (auto playback = this->getChildByID("playback-menu")) {
            if (auto music = playback->getChildByID("music-playback-button")) {
                auto world = playback->convertToWorldSpace(music->getPosition());

                pos = ccp(
                    world.x + music->getScaledContentWidth() / 2 + 32.f,
                    world.y
                );
            }
        }

        menu->setPosition(pos);

        log::info("YT Auto Deco button initialized!");

        return true;
    }

    void onYTClick(CCObject*) {
        FLAlertLayer::create(
            "YT Auto Deco",
            "The red YT button is working!",
            "OK"
        )->show();
    }
};
