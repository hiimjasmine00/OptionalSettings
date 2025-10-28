#include "OptionalBoolSettingNode.hpp"

using namespace geode::prelude;
using namespace optional_settings;

OptionalBoolSettingNode* OptionalBoolSettingNode::create(std::shared_ptr<OptionalBoolSetting> setting, float width) {
    auto ret = new OptionalBoolSettingNode();
    if (ret->init(std::move(setting), width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionalBoolSettingNode::init(std::shared_ptr<OptionalBoolSetting> setting, float width) {
    if (!OptionalBaseSettingNode::init(setting, width)) return false;

    auto buttonMenu = getButtonMenu();
    buttonMenu->setContentWidth(20.0f);

    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(OptionalBoolSettingNode::onToggle), 0.55f
    );
    m_toggle->m_onButton->setContentSize({ 25.0f, 25.0f });
    m_toggle->m_onButton->getNormalImage()->setPosition({ 12.5f, 12.5f });
    m_toggle->m_offButton->setContentSize({ 25.0f, 25.0f });
    m_toggle->m_offButton->getNormalImage()->setPosition({ 12.5f, 12.5f });
    m_toggle->m_notClickable = true;
    m_toggle->toggle(setting->getStoredValue());
    buttonMenu->addChildAtPosition(m_toggle, Anchor::Right, { -10.0f, 0.0f });

    updateState(nullptr);

    return true;
}

void OptionalBoolSettingNode::updateState(CCNode* invoker) {
    OptionalBaseSettingNode::updateState(invoker);

    auto enable = getSetting()->shouldEnable() && isEnabled();
    m_toggle->toggle(getStoredValue());
    m_toggle->setCascadeColorEnabled(true);
    m_toggle->setCascadeOpacityEnabled(true);
    m_toggle->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
    m_toggle->setOpacity(enable ? 255 : 155);
    m_toggle->setEnabled(enable);
}

void OptionalBoolSettingNode::onToggle(CCObject*) {
    auto button = m_toggle->m_toggled ? m_toggle->m_onButton : m_toggle->m_offButton;
    button->stopActionByTag(0);
    button->setScale(button->m_baseScale);
    setStoredValue(!m_toggle->m_toggled, m_toggle);
}
