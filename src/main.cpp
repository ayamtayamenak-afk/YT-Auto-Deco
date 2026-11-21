
#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(YTAutoDecoEditor, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelEditorLayer::init(level, p1))
            return false;

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_button_01.png");

        if (!sprite)
            return true;

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(YTAutoDecoEditor::onYTButton)
        );

        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({0, 0});

        button->setPosition({35, 35});
        addChild(menu, 100);

        return true;
    }

    void onYTButton(CCObject*) {
        FLAlertLayer::create(
            "YT Auto Deco",
            "Welcome to YT Auto Deco!",
            "OK"
        )->show();
    }
};
