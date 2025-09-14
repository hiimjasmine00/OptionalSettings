#pragma once
#include "Macros.hpp"
#include "OptionalBaseSetting.hpp"

namespace optional_settings {
    class OPTIONAL_SETTINGS_DLL OptionalIntSetting final : public OptionalBaseSetting<int64_t> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        class PrivateMarker {};
    public:
        OptionalIntSetting(PrivateMarker);
        static geode::Result<std::shared_ptr<SettingV3>> parse(const std::string& key, const std::string& id, const matjson::Value& json);

        geode::Result<> isValid(int64_t value) const override;

        std::optional<int64_t> getMinValue() const;
        std::optional<int64_t> getMaxValue() const;

        bool isArrowsEnabled() const;
        bool isBigArrowsEnabled() const;
        size_t getArrowStepSize() const;
        size_t getBigArrowStepSize() const;
        bool isSliderEnabled() const;
        int64_t getSliderSnap() const;
        bool isInputEnabled() const;

        geode::SettingNodeV3* createNode(float width) override;
    };
}

namespace geode {
    template <std::integral T>
    struct SettingTypeForValueType<std::optional<T>> {
        using SettingType = optional_settings::OptionalIntSetting;
    };
}
