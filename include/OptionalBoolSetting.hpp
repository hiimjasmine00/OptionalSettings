#pragma once
#include "Macros.hpp"
#include "OptionalBaseSetting.hpp"

namespace optional_settings {
    class OPTIONAL_SETTINGS_DLL OptionalBoolSetting final : public OptionalBaseSetting<bool> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        class PrivateMarker {};
    public:
        OptionalBoolSetting(PrivateMarker);
        static geode::Result<std::shared_ptr<SettingV3>> parse(const std::string& key, const std::string& id, const matjson::Value& json);

        geode::Result<> isValid(bool value) const override;

        geode::SettingNodeV3* createNode(float width) override;
    };
}

namespace geode {
    template <>
    struct SettingTypeForValueType<std::optional<bool>> {
        using SettingType = optional_settings::OptionalBoolSetting;
    };
}
