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
        auto textLabel = m_input->getInputNode()->m_textLabel;
        textLabel->setOpacity(255);
        textLabel->setColor({ 255, 255, 255 });

        m_arrowLeftSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        m_arrowLeftSpr->setFlipX(true);
        m_arrowLeftSpr->setScale(0.4f);
        m_arrowLeftBtn = CCMenuItemSpriteExtra::create(m_arrowLeftSpr, this, menu_selector(OptionalStringSettingNode::onArrow));
        m_arrowLeftBtn->setTag(-1);
        buttonMenu->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, { 5.0f, 0.0f });

        m_arrowRightSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        m_arrowRightSpr->setScale(0.4f);
        m_arrowRightBtn = CCMenuItemSpriteExtra::create(m_arrowRightSpr, this, menu_selector(OptionalStringSettingNode::onArrow));
        m_arrowRightBtn->setTag(1);
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
        m_arrowRightSpr->setOpacity(enable ? 255 : 155);
        m_arrowRightSpr->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
        m_arrowRightBtn->setEnabled(enable);
        m_arrowLeftSpr->setOpacity(enable ? 255 : 155);
        m_arrowLeftSpr->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
        m_arrowLeftBtn->setEnabled(enable);
    }
}

void OptionalStringSettingNode::onArrow(CCObject* sender) {
    if (auto enumOptions = getSetting()->getEnumOptions()) {
        auto options = enumOptions.value();
        auto it = std::ranges::find(options, getStoredValue());
        auto index = it != options.end() ? it - options.begin() : 0;
        if (sender->getTag() > 0) index = index < options.size() - 1 ? index + 1 : 0;
        else index = index > 0 ? index - 1 : options.size() - 1;
        setStoredValue(options[index], static_cast<CCNode*>(sender));
    }
}
