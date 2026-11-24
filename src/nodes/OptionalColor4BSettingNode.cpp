#include "OptionalColor4BSettingNode.hpp"
#include <Geode/binding/ColorChannelSprite.hpp>

using namespace geode::prelude;
using namespace optional_settings;

OptionalColor4BSettingNode* OptionalColor4BSettingNode::create(std::shared_ptr<OptionalColor4BSetting> setting, float width) {
    auto ret = new OptionalColor4BSettingNode();
    if (ret->init(std::move(setting), width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionalColor4BSettingNode::init(std::shared_ptr<OptionalColor4BSetting> setting, float width) {
    if (!OptionalBaseSettingNode::init(setting, width)) return false;

    auto buttonMenu = getButtonMenu();
    buttonMenu->setContentWidth(20.0f);

    m_colorBtn = Button::createWithNode(ColorChannelSprite::create(), [this](Button* sender) {
        onSelectColor(sender);
    });
    m_colorBtn->setScale(0.65f);
    buttonMenu->addChildAtPosition(m_colorBtn, Anchor::Right, { -10.0f, 0.0f });

    updateState(nullptr);

    return true;
}

void OptionalColor4BSettingNode::updateState(CCNode* invoker) {
    OptionalBaseSettingNode::updateState(invoker);

    auto value = getStoredValue();
    m_colorBtn->setColor({ value.r, value.g, value.b });
    static_cast<ColorChannelSprite*>(m_colorBtn->getDisplayNode())->updateOpacity(value.a / 255.0f);

    auto enable = getSetting()->shouldEnable() && isEnabled();
    m_colorBtn->setOpacity(enable ? 255 : 155);
    m_colorBtn->setEnabled(enable);
}

void OptionalColor4BSettingNode::onSelectColor(CCObject*) {
    auto popup = ColorPickPopup::create(getStoredValue());
    popup->setCallback([this](const ccColor4B& color) {
        setStoredValue(color, nullptr);
    });
    popup->show();
}
