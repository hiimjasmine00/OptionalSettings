#include "Internal.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

using namespace geode::prelude;

void Internal::registerCustomSettingType(std::string_view type, SettingGeneratorV3 generator) {
    if (auto res = ModSettingsManager::from(getMod())->registerCustomSettingType(type, std::move(generator)); res.isErr()) {
        log::error("Failed to register custom setting type '{}': {}", type, std::move(res).unwrapErr());
    }
}
