#include <OptionalBaseSettingNode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/ui/TextInput.hpp>

template <class T>
class OptionalNumberSettingNode : public optional_settings::OptionalBaseSettingNode<T> {
protected:
    geode::TextInput* m_input;
    Slider* m_slider;
    CCMenuItemSpriteExtra* m_arrowLeftBtn;
    CCMenuItemSpriteExtra* m_bigArrowLeftBtn;
    CCMenuItemSpriteExtra* m_arrowRightBtn;
    CCMenuItemSpriteExtra* m_bigArrowRightBtn;
    cocos2d::CCSprite* m_arrowLeftBtnSpr;
    cocos2d::CCSprite* m_bigArrowLeftBtnSpr;
    cocos2d::CCSprite* m_arrowRightBtnSpr;
    cocos2d::CCSprite* m_bigArrowRightBtnSpr;

    float valueToSlider(typename T::ValueType value) {
        auto setting = this->getSetting();
        double min = setting->getMinValue().value_or(-100);
        double max = setting->getMaxValue().value_or(100);
        auto range = max - min;
        return std::clamp((value - min) / range, 0.0, 1.0);
    }

    typename T::ValueType valueFromSlider(float num) {
        auto setting = this->getSetting();
        auto min = setting->getMinValue().value_or(-100);
        auto max = setting->getMaxValue().value_or(100);
        auto range = max - min;
        typename T::ValueType value = num * range + min;
        auto step = setting->getSliderSnap();
        if (step > 0) value = round(value / step) * step;
        return value;
    }

    bool init(std::shared_ptr<T> setting, float width) {
        if (!optional_settings::OptionalBaseSettingNode<T>::init(setting, width)) return false;

        auto buttonMenu = this->getButtonMenu();
        buttonMenu->addChildAtPosition(this->getMainToggle(), geode::Anchor::Left, { -15.0f, 0.0f });

        m_bigArrowLeftBtnSpr = cocos2d::CCSprite::create();
        m_bigArrowLeftBtnSpr->setCascadeColorEnabled(true);
        m_bigArrowLeftBtnSpr->setCascadeOpacityEnabled(true);

        auto bigArrowLeftSpr1 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto bigArrowLeftSpr2 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        m_bigArrowLeftBtnSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + cocos2d::CCPoint { 20.0f, 0.0f });
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr2, geode::Anchor::Center, { 10.0f, 0.0f });
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr1, geode::Anchor::Center, { -10.0f, 0.0f });
        m_bigArrowLeftBtnSpr->setScale(0.3f);

        m_bigArrowLeftBtn = CCMenuItemSpriteExtra::create(m_bigArrowLeftBtnSpr, this, menu_selector(OptionalNumberSettingNode::onArrow));
        m_bigArrowLeftBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(-setting->getBigArrowStepSize()));
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
        buttonMenu->addChildAtPosition(m_bigArrowLeftBtn, geode::Anchor::Left, { 5.0f, 0.0f });

        m_arrowLeftBtnSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        m_arrowLeftBtnSpr->setScale(0.5f);
        m_arrowLeftBtn = CCMenuItemSpriteExtra::create(m_arrowLeftBtnSpr, this, menu_selector(OptionalNumberSettingNode::onArrow));
        m_arrowLeftBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(-setting->getArrowStepSize()));
        m_arrowLeftBtn->setVisible(setting->isArrowsEnabled());
        buttonMenu->addChildAtPosition(m_arrowLeftBtn, geode::Anchor::Left, { 22.0f, 0.0f });

        m_input = geode::TextInput::create(buttonMenu->getContentWidth() - 40.0f, "Num");
        m_input->setScale(0.7f);
        m_input->setCommonFilter(std::is_floating_point_v<typename T::ValueType> ? geode::CommonFilter::Float : geode::CommonFilter::Int);
        m_input->setCallback([this, setting](const std::string& str) {
            this->setStoredValue(geode::utils::numFromString<typename T::ValueType>(str).unwrapOr(setting->getStoredDefaultValue()), m_input);
        });
        if (!setting->isInputEnabled()) {
            m_input->getBGSprite()->setVisible(false);
            m_input->setEnabled(false);
            m_input->getInputNode()->m_textLabel->setOpacity(255);
            m_input->getInputNode()->m_textLabel->setColor({ 255, 255, 255 });
        }
        buttonMenu->addChildAtPosition(m_input, geode::Anchor::Center);

        m_arrowRightBtnSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        m_arrowRightBtnSpr->setFlipX(true);
        m_arrowRightBtnSpr->setScale(0.5f);
        m_arrowRightBtn = CCMenuItemSpriteExtra::create(m_arrowRightBtnSpr, this, menu_selector(OptionalNumberSettingNode::onArrow));
        m_arrowRightBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(setting->getArrowStepSize()));
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled());
        buttonMenu->addChildAtPosition(m_arrowRightBtn, geode::Anchor::Right, { -22.0f, 0.0f });

        m_bigArrowRightBtnSpr = cocos2d::CCSprite::create();
        m_bigArrowRightBtnSpr->setCascadeColorEnabled(true);
        m_bigArrowRightBtnSpr->setCascadeOpacityEnabled(true);
        auto bigArrowRightSpr1 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr1->setFlipX(true);
        auto bigArrowRightSpr2 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr2->setFlipX(true);

        m_bigArrowRightBtnSpr->setContentSize(bigArrowRightSpr1->getContentSize() + cocos2d::CCPoint { 20.0f, 0.0f });
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr1, geode::Anchor::Center, { -10.0f, 0.0f });
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr2, geode::Anchor::Center, { 10.0f, 0.0f });
        m_bigArrowRightBtnSpr->setScale(0.3f);

        m_bigArrowRightBtn = CCMenuItemSpriteExtra::create(m_bigArrowRightBtnSpr, this, menu_selector(OptionalNumberSettingNode::onArrow));
        m_bigArrowRightBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(setting->getBigArrowStepSize()));
        m_bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
        buttonMenu->addChildAtPosition(m_bigArrowRightBtn, geode::Anchor::Right, { -5.0f, 0.0f });

        if (setting->isSliderEnabled()) {
            this->setContentHeight(45.0f);
            buttonMenu->updateAnchoredPosition(geode::Anchor::Right, { -10.0f, 7.0f });
            this->getMainToggle()->updateAnchoredPosition(geode::Anchor::Left, { -15.0f, -7.0f });

            m_slider = Slider::create(this, menu_selector(OptionalNumberSettingNode::onSlider));
            m_slider->setScale(0.5f);
            buttonMenu->addChildAtPosition(m_slider, geode::Anchor::Center, { 0.0f, -20.0f }, { 0.0f, 0.0f });
        }

        this->setEnabled(setting->isEnabled(), nullptr);
        this->setStoredValue(setting->getStoredValue(), nullptr);
        updateState(nullptr);

        return true;
    }

    void updateState(cocos2d::CCNode* invoker) override {
        optional_settings::OptionalBaseSettingNode<T>::updateState(invoker);

        auto setting = this->getSetting();
        auto enable = setting->shouldEnable() && this->isEnabled();
        if (setting->isInputEnabled()) m_input->setEnabled(enable);

        if (invoker != m_input && (invoker != nullptr || !m_input->getInputNode()->m_selected)) {
            m_input->setString(fmt::to_string(round(this->getStoredValue() * 100000.0) / 100000.0));
        }

        auto min = setting->getMinValue();
        auto enableLeft = enable && (!min.has_value() || this->getStoredValue() > min.value());
        m_arrowLeftBtn->setEnabled(enableLeft);
        m_bigArrowLeftBtn->setEnabled(enableLeft);
        m_arrowLeftBtnSpr->setOpacity(enableLeft ? 255 : 155);
        m_arrowLeftBtnSpr->setColor(enableLeft ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
        m_bigArrowLeftBtnSpr->setOpacity(enableLeft ? 255 : 155);
        m_bigArrowLeftBtnSpr->setColor(enableLeft ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });

        auto max = setting->getMaxValue();
        auto enableRight = enable && (!max.has_value() || this->getStoredValue() < max.value());
        m_arrowRightBtn->setEnabled(enableRight);
        m_bigArrowRightBtn->setEnabled(enableRight);
        m_arrowRightBtnSpr->setOpacity(enableRight ? 255 : 155);
        m_arrowRightBtnSpr->setColor(enableRight ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
        m_bigArrowRightBtnSpr->setOpacity(enableRight ? 255 : 155);
        m_bigArrowRightBtnSpr->setColor(enableRight ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });

        if (m_slider && invoker != m_slider) {
            m_slider->m_touchLogic->m_thumb->setValue(valueToSlider(this->getStoredValue()));
            m_slider->updateBar();
            m_slider->m_sliderBar->setColor(enable ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
            m_slider->m_touchLogic->m_thumb->setColor(enable ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
            m_slider->m_touchLogic->m_thumb->setEnabled(enable);
        }
    }

    void onArrow(cocos2d::CCObject* sender) {
        auto setting = this->getSetting();
        auto value = this->getStoredValue() + static_cast<geode::ObjWrapper<typename T::ValueType>*>(
            static_cast<cocos2d::CCNode*>(sender)->getUserObject("arrow-step-size"))->getValue();
        if (auto min = setting->getMinValue()) value = std::max(min.value(), value);
        if (auto max = setting->getMaxValue()) value = std::min(max.value(), value);
        this->setStoredValue(value, static_cast<cocos2d::CCNode*>(sender));
    }

    void onSlider(cocos2d::CCObject*) {
        auto value = valueFromSlider(m_slider->m_touchLogic->m_thumb->getValue());
        if (value != this->getStoredValue()) this->setStoredValue(value, m_slider);
    }
public:
    static OptionalNumberSettingNode* create(std::shared_ptr<T> setting, float width) {
        auto ret = new OptionalNumberSettingNode();
        if (ret->init(std::move(setting), width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};
