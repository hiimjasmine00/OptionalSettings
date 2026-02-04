#pragma once
#include <Geode/loader/SettingV3.hpp>

namespace optional_settings {
    template <class T, class V = T>
    class OptionalBaseSetting : public geode::SettingV3 {
    private:
        class Impl final {
        private:
            T defaultValue;
            bool defaultEnabled;
            T value;
            bool enabled;
            friend class OptionalBaseSetting;
        };
        std::shared_ptr<Impl> m_impl;
    protected:
        void parseDefaultValue(geode::JsonExpectedValue& json) {
            auto defaultValue = json.needs("default");
            if (defaultValue.isObject() && defaultValue.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) {
                defaultValue.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH).into(m_impl->defaultValue);
            }
            else {
                defaultValue.into(m_impl->defaultValue);
            }
            m_impl->value = m_impl->defaultValue;

            auto defaultEnabled = json.needs("default-enabled");
            if (defaultEnabled.isObject() && defaultEnabled.has(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH)) {
                defaultEnabled.needs(GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH).into(m_impl->defaultEnabled);
            }
            else {
                defaultEnabled.into(m_impl->defaultEnabled);
            }
            m_impl->enabled = m_impl->defaultEnabled;
        }

        void parseBaseProperties(std::string key, std::string id, geode::JsonExpectedValue& json) {
            SettingV3::parseBaseProperties(std::move(key), std::move(id), json);
            parseDefaultValue(json);
        }

        geode::Result<> parseBaseProperties(std::string key, std::string id, const matjson::Value& json) {
            auto root = geode::checkJson(json, "OptionalBaseSetting");
            parseBaseProperties(std::move(key), std::move(id), root);
            return root.ok();
        }

        void setDefaultValue(const std::optional<T>& value) {
            m_impl->defaultEnabled = value.has_value();
            if (m_impl->defaultEnabled) m_impl->defaultValue = value.value();
        }

        void setStoredDefaultValue(V value) {
            m_impl->defaultValue = value;
        }

        void setHasDefaultValue(bool hasDefault) {
            m_impl->defaultEnabled = hasDefault;
        }
    public:
        OptionalBaseSetting() : m_impl(std::make_shared<Impl>()) {}

        using ValueType = T;
        using ValueAssignType = V;

        std::optional<T> getDefaultValue() const {
            return m_impl->defaultEnabled ? std::optional<T>(m_impl->defaultValue) : std::nullopt;
        }

        T getStoredDefaultValue() const {
            return m_impl->defaultValue;
        }

        bool hasDefaultValue() const {
            return m_impl->defaultEnabled;
        }

        std::optional<T> getValue() const {
            return m_impl->enabled ? std::optional<T>(m_impl->value) : std::nullopt;
        }

        T getStoredValue() const {
            return m_impl->value;
        }

        bool isEnabled() const {
            return m_impl->enabled;
        }

        void setValue(const std::optional<T>& value) {
            if (m_impl->enabled == value.has_value() && (!m_impl->enabled || m_impl->value == value.value())) return;
            m_impl->enabled = value.has_value();
            if (m_impl->enabled) m_impl->value = value.value();
            markChanged();
        }

        void setStoredValue(V value) {
            if (m_impl->value == value) return;
            m_impl->value = value;
            markChanged();
        }

        void setEnabled(bool enabled) {
            if (m_impl->enabled == enabled) return;
            m_impl->enabled = enabled;
            markChanged();
        }

        virtual geode::Result<> isValid(V value) const {
            return geode::Ok();
        }

        bool isDefaultValue() const override {
            return m_impl->enabled == m_impl->defaultEnabled && m_impl->value == m_impl->defaultValue;
        }

        void reset() override {
            setStoredValue(m_impl->defaultValue);
            setEnabled(m_impl->defaultEnabled);
        }

        bool load(const matjson::Value& json) override {
            auto root = geode::checkJson(json, "OptionalBaseSetting");
            if (!root.has("enabled") || !root.has("value")) return false;
            root.needs("enabled").into(m_impl->enabled);
            root.needs("value").into(m_impl->value);
            return true;
        }

        bool save(matjson::Value& json) const override {
            json = matjson::Value::object();
            json.set("enabled", m_impl->enabled);
            json.set("value", m_impl->value);
            return true;
        }
    };
}
