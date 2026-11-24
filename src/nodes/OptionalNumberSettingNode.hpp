#include <OptionalBaseSettingNode.hpp>
#include <Geode/ui/SliderNode.hpp>
#include <Geode/ui/TextInput.hpp>

template <class T>
class OptionalNumberSettingNode : public optional_settings::OptionalBaseSettingNode<T> {
protected:
    geode::TextInput* m_input;
    geode::SliderNode* m_slider;
    geode::Button* m_arrowLeftBtn;
    geode::Button* m_bigArrowLeftBtn;
    geode::Button* m_arrowRightBtn;
    geode::Button* m_bigArrowRightBtn;

    bool init(std::shared_ptr<T> setting, float width) {
        if (!optional_settings::OptionalBaseSettingNode<T>::init(setting, width)) return false;

        auto buttonMenu = this->getButtonMenu();
        buttonMenu->addChildAtPosition(this->getMainToggle(), geode::Anchor::Left, { -15.0f, 0.0f });

        auto bigArrowLeftBtnSpr = cocos2d::CCSprite::create();
        bigArrowLeftBtnSpr->setCascadeOpacityEnabled(true);

        auto bigArrowLeftSpr1 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto bigArrowLeftSpr2 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowLeftBtnSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + cocos2d::CCPoint { 20.0f, 0.0f });
        bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr2, geode::Anchor::Center, { 10.0f, 0.0f });
        bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr1, geode::Anchor::Center, { -10.0f, 0.0f });

        m_bigArrowLeftBtn = geode::Button::createWithNode(bigArrowLeftBtnSpr, [this](geode::Button* sender) {
            this->onArrow(sender);
        });
        m_bigArrowLeftBtn->setScale(0.3f);
        m_bigArrowLeftBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(-setting->getBigArrowStepSize()));
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
        buttonMenu->addChildAtPosition(m_bigArrowLeftBtn, geode::Anchor::Left, { 5.0f, 0.0f });

        m_arrowLeftBtn = geode::Button::createWithSpriteFrameName("GJ_arrow_01_001.png", [this](geode::Button* sender) {
            this->onArrow(sender);
        });
        m_arrowLeftBtn->setScale(0.5f);
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
            m_input->setTextColor({ 255, 255, 255, 255 });
        }
        buttonMenu->addChildAtPosition(m_input, geode::Anchor::Center);

        m_arrowRightBtn = geode::Button::createWithSpriteFrameName("GJ_arrow_01_001.png", [this](geode::Button* sender) {
            this->onArrow(sender);
        });
        m_arrowRightBtn->setScale(0.5f);
        m_arrowRightBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(setting->getArrowStepSize()));
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled());
        static_cast<cocos2d::CCSprite*>(m_arrowRightBtn->getDisplayNode())->setFlipX(true);
        buttonMenu->addChildAtPosition(m_arrowRightBtn, geode::Anchor::Right, { -22.0f, 0.0f });

        auto bigArrowRightBtnSpr = cocos2d::CCSprite::create();
        bigArrowRightBtnSpr->setCascadeOpacityEnabled(true);
        auto bigArrowRightSpr1 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr1->setFlipX(true);
        auto bigArrowRightSpr2 = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr2->setFlipX(true);

        bigArrowRightBtnSpr->setContentSize(bigArrowRightSpr1->getContentSize() + cocos2d::CCPoint { 20.0f, 0.0f });
        bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr1, geode::Anchor::Center, { -10.0f, 0.0f });
        bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr2, geode::Anchor::Center, { 10.0f, 0.0f });

        m_bigArrowRightBtn = geode::Button::createWithNode(bigArrowRightBtnSpr, [this](geode::Button* sender) {
            this->onArrow(sender);
        });
        m_bigArrowRightBtn->setScale(0.3f);
        m_bigArrowRightBtn->setUserObject("arrow-step-size", geode::ObjWrapper<typename T::ValueType>::create(setting->getBigArrowStepSize()));
        m_bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
        buttonMenu->addChildAtPosition(m_bigArrowRightBtn, geode::Anchor::Right, { -5.0f, 0.0f });

        if (setting->isSliderEnabled()) {
            this->setContentHeight(45.0f);
            buttonMenu->updateAnchoredPosition(geode::Anchor::Right, { -10.0f, 7.0f });
            this->getMainToggle()->updateAnchoredPosition(geode::Anchor::Left, { -15.0f, -7.0f });

            m_slider = geode::SliderNode::create([this](geode::SliderNode* node, float v) {
                typename T::ValueType value = v;

                auto storedValue = this->getStoredValue();
                if (value != storedValue || std::fabs(value - storedValue) >= 0.0001) this->setStoredValue(value, m_slider);
            });
            m_slider->setScale(0.5f);
            m_slider->setMin(setting->getMinValue().value_or(-100.0f));
            m_slider->setMax(setting->getMaxValue().value_or(+100.0f));
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

        if (invoker != m_input && (invoker != nullptr || !m_input->isFocused())) {
            m_input->setString(fmt::to_string(round(this->getStoredValue() * 100000.0) / 100000.0));
        }

        auto min = setting->getMinValue();
        auto enableLeft = enable && (!min.has_value() || this->getStoredValue() > min.value());
        m_arrowLeftBtn->setEnabled(enableLeft);
        m_bigArrowLeftBtn->setEnabled(enableLeft);
        m_arrowLeftBtn->setOpacity(enableLeft ? 255 : 155);
        m_arrowLeftBtn->setColor(enableLeft ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
        m_bigArrowLeftBtn->setOpacity(enableLeft ? 255 : 155);
        m_bigArrowLeftBtn->setColor(enableLeft ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });

        auto max = setting->getMaxValue();
        auto enableRight = enable && (!max.has_value() || this->getStoredValue() < max.value());
        m_arrowRightBtn->setEnabled(enableRight);
        m_bigArrowRightBtn->setEnabled(enableRight);
        m_arrowRightBtn->setOpacity(enableRight ? 255 : 155);
        m_arrowRightBtn->setColor(enableRight ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
        m_bigArrowRightBtn->setOpacity(enableRight ? 255 : 155);
        m_bigArrowRightBtn->setColor(enableRight ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });

        if (m_slider && invoker != m_slider) {
            m_slider->setValue(this->getStoredValue());
            m_slider->getBar()->setColor(enable ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
            m_slider->getThumb()->setColor(enable ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
            m_slider->setEnabled(enable);
        }
    }

    void onArrow(cocos2d::CCNode* sender) {
        auto setting = this->getSetting();
        auto value = this->getStoredValue() + static_cast<geode::ObjWrapper<typename T::ValueType>*>(
            sender->getUserObject("arrow-step-size"))->getValue();
        if (auto min = setting->getMinValue()) value = std::max(min.value(), value);
        if (auto max = setting->getMaxValue()) value = std::min(max.value(), value);
        this->setStoredValue(value, sender);
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
