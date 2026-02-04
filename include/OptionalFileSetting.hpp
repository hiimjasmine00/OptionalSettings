#pragma once
#include "Macros.hpp"
#include "OptionalBaseSetting.hpp"

namespace optional_settings {
    class OPTIONAL_SETTINGS_DLL OptionalFileSetting final : public OptionalBaseSetting<std::filesystem::path, const std::filesystem::path&> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        class PrivateMarker {};
    public:
        OptionalFileSetting(PrivateMarker);
        static geode::Result<std::shared_ptr<SettingV3>> parse(std::string key, std::string id, const matjson::Value& json);

        geode::Result<> isValid(const std::filesystem::path& value) const override;

        bool isFolder() const;
        bool useSaveDialog() const;

        std::optional<std::vector<geode::utils::file::FilePickOptions::Filter>> getFilters() const;

        geode::SettingNodeV3* createNode(float width) override;
    };
}

namespace geode {
    template <>
    struct SettingTypeForValueType<std::optional<std::filesystem::path>> {
        using SettingType = optional_settings::OptionalFileSetting;
    };
}
