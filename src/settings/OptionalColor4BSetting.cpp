#include "OptionalColor4BSetting.hpp"
#include "../nodes/OptionalColor4BSettingNode.hpp"
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    auto mod = Mod::get();
    if (auto res = mod->registerCustomSettingType("optional-rgba", &OptionalColor4BSetting::parse); res.isErr()) {
        log::logImpl(Severity::Error, mod, "Failed to register custom setting type 'optional-rgba': {}", res.unwrapErr());
    }
}

class OptionalColor4BSetting::Impl final {
public:
};

OptionalColor4BSetting::OptionalColor4BSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalColor4BSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalColor4BSetting>(PrivateMarker());
    auto root = checkJson(json, "OptionalColor4BSetting");
    ret->parseBaseProperties(key, id, root);
    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(ret));
}

Result<> OptionalColor4BSetting::isValid(ccColor4B value) const {
    return Ok();
}

SettingNodeV3* OptionalColor4BSetting::createNode(float width) {
    return OptionalColor4BSettingNode::create(std::static_pointer_cast<OptionalColor4BSetting>(shared_from_this()), width);
}
