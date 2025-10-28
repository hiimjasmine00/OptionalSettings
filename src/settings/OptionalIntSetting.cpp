#include "OptionalIntSetting.hpp"
#include "../Internal.hpp"
#include "../nodes/OptionalNumberSettingNode.hpp"

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    Internal::registerCustomSettingType("optional-int", &OptionalIntSetting::parse);
}

class OptionalIntSetting::Impl final {
public:
    std::optional<int64_t> minValue;
    std::optional<int64_t> maxValue;

    struct {
        size_t arrowStepSize = 1;
        size_t bigArrowStepSize = 5;
        bool sliderEnabled = true;
        int64_t sliderSnap = 1;
        bool textInputEnabled = true;
    } controls;
};

OptionalIntSetting::OptionalIntSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalIntSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalIntSetting>(PrivateMarker());

    auto root = checkJson(json, "OptionalIntSetting");
    ret->parseBaseProperties(key, id, root);

    root.has("min").into(ret->m_impl->minValue);
    root.has("max").into(ret->m_impl->maxValue);
    if (auto controls = root.has("control")) {
        controls.has("arrows");
        controls.has("big-arrows");
        controls.has("arrow-step").into(ret->m_impl->controls.arrowStepSize);
        controls.has("big-arrow-step").into(ret->m_impl->controls.bigArrowStepSize);
        controls.has("slider").into(ret->m_impl->controls.sliderEnabled);
        controls.has("slider-step").into(ret->m_impl->controls.sliderSnap);
        controls.has("input").into(ret->m_impl->controls.textInputEnabled);
        controls.checkUnknownKeys();
    }

    if (!root.has("control").has("arrows").get<bool>(true)) ret->m_impl->controls.arrowStepSize = 0;
    if (!root.has("control").has("big-arrows").get<bool>()) ret->m_impl->controls.bigArrowStepSize = 0;

    if (!ret->m_impl->minValue || !ret->m_impl->maxValue) {
        if (ret->m_impl->controls.sliderEnabled && root.has("control").has("slider")) {
            log::warn(
                "Setting '{}' has \"controls.slider\" enabled but doesn't "
                "have both \"min\" and \"max\" defined - the slider has "
                "been force-disabled!",
                key
            );
        }
        ret->m_impl->controls.sliderEnabled = false;
    }

    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(std::move(ret)));
}

Result<> OptionalIntSetting::isValid(int64_t value) const {
    if (m_impl->minValue && value < *m_impl->minValue) {
        return Err("Value must be at least {}", *m_impl->minValue);
    }
    if (m_impl->maxValue && value > *m_impl->maxValue) {
        return Err("Value must be at most {}", *m_impl->maxValue);
    }
    return Ok();
}

std::optional<int64_t> OptionalIntSetting::getMinValue() const {
    return m_impl->minValue;
}

std::optional<int64_t> OptionalIntSetting::getMaxValue() const {
    return m_impl->maxValue;
}

bool OptionalIntSetting::isArrowsEnabled() const {
    return m_impl->controls.arrowStepSize > 0;
}

bool OptionalIntSetting::isBigArrowsEnabled() const {
    return m_impl->controls.bigArrowStepSize > 0;
}

size_t OptionalIntSetting::getArrowStepSize() const {
    return m_impl->controls.arrowStepSize;
}

size_t OptionalIntSetting::getBigArrowStepSize() const {
    return m_impl->controls.bigArrowStepSize;
}

bool OptionalIntSetting::isSliderEnabled() const {
    return m_impl->controls.sliderEnabled;
}

int64_t OptionalIntSetting::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}

bool OptionalIntSetting::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
}

SettingNodeV3* OptionalIntSetting::createNode(float width) {
    return OptionalNumberSettingNode<OptionalIntSetting>::create(std::static_pointer_cast<OptionalIntSetting>(shared_from_this()), width);
}
