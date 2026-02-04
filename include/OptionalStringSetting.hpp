#pragma once
#include "Macros.hpp"
#include "OptionalBaseSetting.hpp"

namespace optional_settings {
    class OPTIONAL_SETTINGS_DLL OptionalStringSetting final : public OptionalBaseSetting<std::string, std::string_view> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        class PrivateMarker {};
    public:
        OptionalStringSetting(PrivateMarker);
        static geode::Result<std::shared_ptr<SettingV3>> parse(std::string key, std::string id, const matjson::Value& json);

        geode::Result<> isValid(std::string_view value) const override;

        std::optional<std::string> getRegexValidator() const;
        std::optional<std::string> getAllowedCharacters() const;
        std::optional<std::vector<std::string>> getEnumOptions() const;

        geode::SettingNodeV3* createNode(float width) override;
    };
}

namespace geode {
    template <>
    struct SettingTypeForValueType<std::optional<std::string>> {
        using SettingType = optional_settings::OptionalStringSetting;
    };
}
