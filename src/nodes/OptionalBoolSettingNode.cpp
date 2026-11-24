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

    m_toggle = Toggler::createWithStandardSprites([this](Toggler* sender, bool toggled) {
        setStoredValue(toggled, sender);
        markChanged(sender);
    });
    m_toggle->setScale(0.55f);
    m_toggle->setToggled(setting->getStoredValue());
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
