#pragma once
#include "Macros.hpp"
#include "OptionalBaseSetting.hpp"

namespace optional_settings {
    class OPTIONAL_SETTINGS_DLL OptionalColor3BSetting final : public OptionalBaseSetting<cocos2d::ccColor3B> {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        class PrivateMarker {};
    public:
        OptionalColor3BSetting(PrivateMarker);
        static geode::Result<std::shared_ptr<SettingV3>> parse(const std::string& key, const std::string& id, const matjson::Value& json);

        geode::Result<> isValid(cocos2d::ccColor3B value) const override;

        geode::SettingNodeV3* createNode(float width) override;
    };
}

namespace geode {
    template <>
    struct SettingTypeForValueType<std::optional<cocos2d::ccColor3B>> {
        using SettingType = optional_settings::OptionalColor3BSetting;
    };
}
