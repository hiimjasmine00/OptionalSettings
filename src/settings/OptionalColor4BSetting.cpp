#include "OptionalColor4BSetting.hpp"
#include "../Internal.hpp"
#include "../nodes/OptionalColor4BSettingNode.hpp"

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    Internal::registerCustomSettingType("optional-rgba", &OptionalColor4BSetting::parse);
}

class OptionalColor4BSetting::Impl final {
public:
};

OptionalColor4BSetting::OptionalColor4BSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalColor4BSetting::parse(std::string key, std::string id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalColor4BSetting>(PrivateMarker());
    auto root = checkJson(json, "OptionalColor4BSetting");
    ret->parseBaseProperties(std::move(key), std::move(id), root);
    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(std::move(ret)));
}

Result<> OptionalColor4BSetting::isValid(ccColor4B value) const {
    return Ok();
}

SettingNodeV3* OptionalColor4BSetting::createNode(float width) {
    return OptionalColor4BSettingNode::create(std::static_pointer_cast<OptionalColor4BSetting>(shared_from_this()), width);
}
