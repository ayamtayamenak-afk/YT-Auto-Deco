
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(YTAutoDeco, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        auto winSize = CCDirector::get()->getWinSize();

        auto buttonSprite = ButtonSprite::create(
            "YT",
            "goldFont.fnt",
            "GJ_button_01.png",
            0.8f
        );

        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(YTAutoDeco::onYTClick)
        );

        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({0, 0});

        button->setPosition({
            45.f,
            winSize.height - 45.f
        });

        this->addChild(menu, 999);

        log::info("YT Auto Deco button created!");

        return true;
    }

    void onYTClick(CCObject*) {
        FLAlertLayer::create(
            "YT Auto Deco",
            "The YT button is working!",
            "OK"
        )->show();
    }
};
