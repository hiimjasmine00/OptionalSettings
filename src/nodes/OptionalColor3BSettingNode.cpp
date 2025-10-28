#include "OptionalColor3BSettingNode.hpp"
#include <Geode/binding/ColorChannelSprite.hpp>

using namespace geode::prelude;
using namespace optional_settings;

OptionalColor3BSettingNode* OptionalColor3BSettingNode::create(std::shared_ptr<OptionalColor3BSetting> setting, float width) {
    auto ret = new OptionalColor3BSettingNode();
    if (ret->init(std::move(setting), width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionalColor3BSettingNode::init(std::shared_ptr<OptionalColor3BSetting> setting, float width) {
    if (!OptionalBaseSettingNode::init(setting, width)) return false;

    auto buttonMenu = getButtonMenu();
    buttonMenu->setContentWidth(20.0f);

    m_colorSprite = ColorChannelSprite::create();
    m_colorSprite->setScale(0.65f);
    m_colorBtn = CCMenuItemSpriteExtra::create(m_colorSprite, this, menu_selector(OptionalColor3BSettingNode::onSelectColor));
    buttonMenu->addChildAtPosition(m_colorBtn, Anchor::Right, { -10.0f, 0.0f });

    updateState(nullptr);

    return true;
}

void OptionalColor3BSettingNode::updateState(CCNode* invoker) {
    OptionalBaseSettingNode::updateState(invoker);

    m_colorSprite->setColor(getStoredValue());

    auto enable = getSetting()->shouldEnable() && isEnabled();
    m_colorSprite->setOpacity(enable ? 255 : 155);
    m_colorBtn->setEnabled(enable);
}

void OptionalColor3BSettingNode::onSelectColor(CCObject*) {
    auto popup = ColorPickPopup::create(getStoredValue());
    popup->setDelegate(this);
    popup->show();
}

void OptionalColor3BSettingNode::updateColor(const ccColor4B& color) {
    setStoredValue({ color.r, color.g, color.b }, nullptr);
}
