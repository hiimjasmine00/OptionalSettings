#include "Internal.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

using namespace geode::prelude;

void Internal::registerCustomSettingType(std::string_view type, SettingGeneratorV3 generator) {
    static ModSettingsManager* manager = ModSettingsManager::from(getMod());
    if (auto res = manager->registerCustomSettingType(type, std::move(generator)); res.isErr()) {
        log::error("{}", res.unwrapErr());
    }
}
