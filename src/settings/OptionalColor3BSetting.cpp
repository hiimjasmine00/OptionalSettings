#include "OptionalColor3BSetting.hpp"
#include "../nodes/OptionalColor3BSettingNode.hpp"
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    auto mod = Mod::get();
    if (auto res = mod->registerCustomSettingType("optional-color", &OptionalColor3BSetting::parse); res.isErr()) {
        log::logImpl(Severity::Error, mod, "Failed to register custom setting type 'optional-color': {}", res.unwrapErr());
    }
    if (auto res = mod->registerCustomSettingType("optional-rgb", &OptionalColor3BSetting::parse); res.isErr()) {
        log::logImpl(Severity::Error, mod, "Failed to register custom setting type 'optional-rgb': {}", res.unwrapErr());
    }
}

class OptionalColor3BSetting::Impl final {
public:
};

OptionalColor3BSetting::OptionalColor3BSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalColor3BSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalColor3BSetting>(PrivateMarker());
    auto root = checkJson(json, "OptionalColor3BSetting");
    ret->parseBaseProperties(key, id, root);
    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(ret));
}

Result<> OptionalColor3BSetting::isValid(ccColor3B value) const {
    return Ok();
}

SettingNodeV3* OptionalColor3BSetting::createNode(float width) {
    return OptionalColor3BSettingNode::create(std::static_pointer_cast<OptionalColor3BSetting>(shared_from_this()), width);
}
