#pragma once
#include "Macros.hpp"
#include "OptionalBaseSetting.hpp"

namespace optional_settings {
    class OPTIONAL_SETTINGS_DLL OptionalFloatSetting final : public OptionalBaseSetting<double> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        class PrivateMarker {};
    public:
        OptionalFloatSetting(PrivateMarker);
        static geode::Result<std::shared_ptr<SettingV3>> parse(const std::string& key, const std::string& id, const matjson::Value& json);

        geode::Result<> isValid(double value) const override;

        std::optional<double> getMinValue() const;
        std::optional<double> getMaxValue() const;

        bool isArrowsEnabled() const;
        bool isBigArrowsEnabled() const;
        double getArrowStepSize() const;
        double getBigArrowStepSize() const;
        bool isSliderEnabled() const;
        double getSliderSnap() const;
        bool isInputEnabled() const;

        geode::SettingNodeV3* createNode(float width) override;
    };
}

namespace geode {
    template <std::floating_point T>
    struct SettingTypeForValueType<std::optional<T>> {
        using SettingType = optional_settings::OptionalFloatSetting;
    };
}
