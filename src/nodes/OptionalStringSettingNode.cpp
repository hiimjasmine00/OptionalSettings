#include "OptionalStringSettingNode.hpp"

using namespace geode::prelude;
using namespace optional_settings;

OptionalStringSettingNode* OptionalStringSettingNode::create(std::shared_ptr<OptionalStringSetting> setting, float width) {
    auto ret = new OptionalStringSettingNode();
    if (ret->init(std::move(setting), width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionalStringSettingNode::init(std::shared_ptr<OptionalStringSetting> setting, float width) {
    if (!OptionalBaseSettingNode::init(setting, width)) return false;

    auto buttonMenu = getButtonMenu();

    auto hasEnumOptions = setting->getEnumOptions().has_value();

    m_input = TextInput::create(width / 2.0f - (hasEnumOptions ? 50.0f : 0.0f), "Text");
    m_input->setCallback([this](const std::string& str) {
        setStoredValue(str, m_input);
    });
    m_input->setScale(0.7f);
    m_input->setString(setting->getStoredValue());
    if (auto filter = setting->getAllowedCharacters()) m_input->setFilter(filter.value());

    buttonMenu->addChildAtPosition(m_input, Anchor::Center);

    if (hasEnumOptions) {
        m_input->getBGSprite()->setVisible(false);
        m_input->setEnabled(false);
        m_input->setTextColor({ 255, 255, 255, 255 });

        m_arrowLeftBtn = Button::createWithSpriteFrameName("navArrowBtn_001.png", [this](Button* sender) {
            onArrow(sender, -1);
        });
        m_arrowLeftBtn->setScale(0.4f);
        buttonMenu->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, { 5.0f, 0.0f });
        static_cast<CCSprite*>(m_arrowLeftBtn->getDisplayNode())->setFlipX(true);

        m_arrowRightBtn = Button::createWithSpriteFrameName("navArrowBtn_001.png", [this](Button* sender) {
            onArrow(sender, 1);
        });
        m_arrowRightBtn->setScale(0.4f);
        buttonMenu->addChildAtPosition(m_arrowRightBtn, Anchor::Right, { -5.0f, 0.0f });
    }

    updateState(nullptr);

    return true;
}

void OptionalStringSettingNode::updateState(CCNode* invoker) {
    OptionalBaseSettingNode::updateState(invoker);

    if (invoker != m_input) m_input->setString(getStoredValue());

    auto setting = getSetting();
    auto enable = setting->shouldEnable() && isEnabled();
    if (!setting->getEnumOptions().has_value()) m_input->setEnabled(enable);
    else {
        m_arrowRightBtn->setOpacity(enable ? 255 : 155);
        m_arrowRightBtn->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
        m_arrowRightBtn->setEnabled(enable);
        m_arrowLeftBtn->setOpacity(enable ? 255 : 155);
        m_arrowLeftBtn->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
        m_arrowLeftBtn->setEnabled(enable);
    }
}

void OptionalStringSettingNode::onArrow(CCObject* sender, int by) {
    if (auto enumOptions = getSetting()->getEnumOptions()) {
        auto options = enumOptions.value();
        auto it = std::ranges::find(options, getStoredValue());
        auto index = it != options.end() ? it - options.begin() : 0;
        if (by > 0) index = index < options.size() - 1 ? index + 1 : 0;
        else index = index > 0 ? index - 1 : options.size() - 1;
        setStoredValue(options[index], static_cast<CCNode*>(sender));
    }
}
