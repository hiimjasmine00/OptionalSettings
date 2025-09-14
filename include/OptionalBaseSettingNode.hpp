#include <Geode/loader/SettingV3.hpp>

namespace optional_settings {
    template <class T>
    class OptionalBaseSettingNode : public geode::SettingNodeV3 {
    private:
        class Impl final {
        private:
            typename T::ValueType value;
            bool enabled;
            CCMenuItemToggler* mainToggle = nullptr;
            friend class OptionalBaseSettingNode;
        };
        std::shared_ptr<Impl> m_impl;
    protected:
        bool init(std::shared_ptr<T> setting, float width) {
            if (!SettingNodeV3::init(setting, width)) return false;

            m_impl = std::make_shared<Impl>();
            m_impl->value = setting->getStoredValue();
            m_impl->enabled = setting->isEnabled();

            m_impl->mainToggle = CCMenuItemToggler::createWithStandardSprites(
                this, menu_selector(OptionalBaseSettingNode::onMainToggle), 0.55f
            );
            m_impl->mainToggle->m_onButton->setContentSize({ 25.0f, 25.0f });
            m_impl->mainToggle->m_onButton->getNormalImage()->setPosition({ 12.5f, 12.5f });
            m_impl->mainToggle->m_offButton->setContentSize({ 25.0f, 25.0f });
            m_impl->mainToggle->m_offButton->getNormalImage()->setPosition({ 12.5f, 12.5f });
            m_impl->mainToggle->m_notClickable = true;
            m_impl->mainToggle->toggle(setting->isEnabled());
            getButtonMenu()->addChildAtPosition(m_impl->mainToggle, geode::Anchor::Left, { -15.0f, 0.0f });

            auto nameMenu = getNameMenu();
            nameMenu->setContentWidth(width / 2.0f);
            nameMenu->updateLayout();

            return true;
        }

        void onMainToggle(cocos2d::CCObject*) {
            auto button = m_impl->mainToggle->m_toggled ? m_impl->mainToggle->m_onButton : m_impl->mainToggle->m_offButton;
            button->stopActionByTag(0);
            button->setScale(button->m_baseScale);
            setEnabled(!m_impl->mainToggle->m_toggled, m_impl->mainToggle);
        }

        CCMenuItemToggler* getMainToggle() const {
            return m_impl->mainToggle;
        }

        void updateState(cocos2d::CCNode* invoker) override {
            SettingNodeV3::updateState(invoker);

            auto nameMenu = getNameMenu();
            nameMenu->setContentWidth(getContentWidth() / 2.0f);
            nameMenu->updateLayout();

            auto setting = getSetting();
            auto enable = setting->shouldEnable();
            m_impl->mainToggle->toggle(m_impl->enabled);
            m_impl->mainToggle->setCascadeColorEnabled(true);
            m_impl->mainToggle->setCascadeOpacityEnabled(true);
            m_impl->mainToggle->setColor(enable ? cocos2d::ccColor3B { 255, 255, 255 } : cocos2d::ccColor3B { 166, 166, 166 });
            m_impl->mainToggle->setOpacity(enable ? 255 : 155);
            m_impl->mainToggle->setEnabled(enable);

            if (m_impl->enabled) {
                if (auto validate = setting->isValid(m_impl->value); validate.isErr()) {
                    auto statusLabel = getStatusLabel();
                    statusLabel->setVisible(true);
                    statusLabel->setString(validate.unwrapErr().c_str());
                    statusLabel->setColor({ 235, 35, 52 });
                }
            }
        }

        void onCommit() override {
            auto setting = getSetting();
            setting->setStoredValue(m_impl->value);
            setting->setEnabled(m_impl->enabled);
            setStoredValue(setting->getStoredValue(), nullptr);
            setEnabled(setting->isEnabled(), nullptr);
        }

        bool hasUncommittedChanges() const override {
            auto setting = getSetting();
            return m_impl->enabled != setting->isEnabled() || m_impl->value != setting->getStoredValue();
        }

        bool hasNonDefaultValue() const override {
            auto setting = getSetting();
            return m_impl->enabled != setting->hasDefaultValue() || m_impl->value != setting->getStoredDefaultValue();
        }

        void onResetToDefault() override {
            auto setting = getSetting();
            setStoredValue(setting->getStoredDefaultValue(), nullptr);
            setEnabled(setting->hasDefaultValue(), nullptr);
        }
    public:
        typename std::optional<typename T::ValueType> getValue() const {
            return m_impl->enabled ? std::optional<typename T::ValueType>(m_impl->value) : std::nullopt;
        }

        typename T::ValueType getStoredValue() const {
            return m_impl->value;
        }

        bool isEnabled() const {
            return m_impl->enabled;
        }

        void setValue(const std::optional<typename T::ValueAssignType>& value, cocos2d::CCNode* invoker) {
            m_impl->enabled = value.has_value();
            if (m_impl->enabled) m_impl->value = *value;
            markChanged(invoker);
        }

        void setStoredValue(typename T::ValueAssignType value, cocos2d::CCNode* invoker) {
            m_impl->value = value;
            markChanged(invoker);
        }

        void setEnabled(bool enabled, cocos2d::CCNode* invoker) {
            m_impl->enabled = enabled;
            markChanged(invoker);
        }

        std::shared_ptr<T> getSetting() const {
            return std::static_pointer_cast<T>(SettingNodeV3::getSetting());
        }
    };
}
