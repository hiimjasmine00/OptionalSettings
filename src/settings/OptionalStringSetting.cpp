#include "OptionalStringSetting.hpp"
#include "../Internal.hpp"
#include "../nodes/OptionalStringSettingNode.hpp"
#include <matjson/std.hpp>
#include <regex>

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    Internal::registerCustomSettingType("optional-string", &OptionalStringSetting::parse);
}

class OptionalStringSetting::Impl final {
public:
    std::optional<std::string> match;
    std::optional<std::string> filter;
    std::optional<std::vector<std::string>> oneOf;
};

OptionalStringSetting::OptionalStringSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalStringSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalStringSetting>(PrivateMarker());

    auto root = checkJson(json, "OptionalStringSetting");
    ret->parseBaseProperties(key, id, root);

    root.has("match").into(ret->m_impl->match);
    root.has("filter").into(ret->m_impl->filter);
    root.has("one-of").into(ret->m_impl->oneOf);
    if (ret->m_impl->oneOf.has_value() && ret->m_impl->oneOf.value().empty()) {
        return Err("Setting '{}' in mod {} - \"one-of\" may not be empty!", key, id);
    }

    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(std::move(ret)));
}

Result<> OptionalStringSetting::isValid(std::string_view value) const {
    if (m_impl->match.has_value()) {
        auto match = m_impl->match.value();
        if (!std::regex_match(std::string(value), std::regex(match))) {
            return Err("Value must match regex {}", match);
        }
    }
    else if (m_impl->oneOf.has_value()) {
        auto oneOf = m_impl->oneOf.value();
        if (!std::ranges::contains(oneOf, std::string(value))) {
            return Err("Value must be one of {}", fmt::join(oneOf, ", "));
        }
    }
    return Ok();
}

std::optional<std::string> OptionalStringSetting::getRegexValidator() const {
    return m_impl->match;
}

std::optional<std::string> OptionalStringSetting::getAllowedCharacters() const {
    return m_impl->filter;
}

std::optional<std::vector<std::string>> OptionalStringSetting::getEnumOptions() const {
    return m_impl->oneOf;
}

SettingNodeV3* OptionalStringSetting::createNode(float width) {
    return OptionalStringSettingNode::create(std::static_pointer_cast<OptionalStringSetting>(shared_from_this()), width);
}
