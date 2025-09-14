#include "OptionalBoolSetting.hpp"
#include "../nodes/OptionalBoolSettingNode.hpp"
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    auto mod = Mod::get();
    if (auto res = mod->registerCustomSettingType("optional-bool", &OptionalBoolSetting::parse); res.isErr()) {
        log::logImpl(Severity::Error, mod, "Failed to register custom setting type 'optional-bool': {}", res.unwrapErr());
    }
}

class OptionalBoolSetting::Impl final {
public:
};

OptionalBoolSetting::OptionalBoolSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalBoolSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalBoolSetting>(PrivateMarker());
    auto root = checkJson(json, "OptionalBoolSetting");
    ret->parseBaseProperties(key, id, root);
    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(ret));
}

Result<> OptionalBoolSetting::isValid(bool value) const {
    return Ok();
}

SettingNodeV3* OptionalBoolSetting::createNode(float width) {
    return OptionalBoolSettingNode::create(std::static_pointer_cast<OptionalBoolSetting>(shared_from_this()), width);
}
