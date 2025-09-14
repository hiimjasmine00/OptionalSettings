#include "OptionalFloatSetting.hpp"
#include "../nodes/OptionalNumberSettingNode.hpp"

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    auto mod = Mod::get();
    if (auto res = mod->registerCustomSettingType("optional-float", &OptionalFloatSetting::parse); res.isErr()) {
        log::logImpl(Severity::Error, mod, "Failed to register custom setting type 'optional-float': {}", res.unwrapErr());
    }
}

class OptionalFloatSetting::Impl final {
public:
    std::optional<double> minValue;
    std::optional<double> maxValue;
    struct {
        double arrowStepSize = 1.0;
        double bigArrowStepSize = 5.0;
        bool sliderEnabled = true;
        double sliderSnap = 0.1;
        bool textInputEnabled = true;
    } controls;
};

OptionalFloatSetting::OptionalFloatSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalFloatSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalFloatSetting>(PrivateMarker());

    auto root = checkJson(json, "OptionalFloatSetting");
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

    if (!root.has("control").has("arrows").get<bool>(true)) ret->m_impl->controls.arrowStepSize = 0.0;
    if (!root.has("control").has("big-arrows").get<bool>()) ret->m_impl->controls.bigArrowStepSize = 0.0;

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
    return root.ok(std::static_pointer_cast<SettingV3>(ret));
}

Result<> OptionalFloatSetting::isValid(double value) const {
    if (m_impl->minValue && value < *m_impl->minValue) {
        return Err("Value must be at least {}", *m_impl->minValue);
    }
    if (m_impl->maxValue && value > *m_impl->maxValue) {
        return Err("Value must be at most {}", *m_impl->maxValue);
    }
    return Ok();
}

std::optional<double> OptionalFloatSetting::getMinValue() const {
    return m_impl->minValue;
}

std::optional<double> OptionalFloatSetting::getMaxValue() const {
    return m_impl->maxValue;
}

bool OptionalFloatSetting::isArrowsEnabled() const {
    return m_impl->controls.arrowStepSize > 0.0;
}

bool OptionalFloatSetting::isBigArrowsEnabled() const {
    return m_impl->controls.bigArrowStepSize > 0.0;
}

double OptionalFloatSetting::getArrowStepSize() const {
    return m_impl->controls.arrowStepSize;
}

double OptionalFloatSetting::getBigArrowStepSize() const {
    return m_impl->controls.bigArrowStepSize;
}

bool OptionalFloatSetting::isSliderEnabled() const {
    return m_impl->controls.sliderEnabled;
}

double OptionalFloatSetting::getSliderSnap() const {
    return m_impl->controls.sliderSnap;
}

bool OptionalFloatSetting::isInputEnabled() const {
    return m_impl->controls.textInputEnabled;
}

SettingNodeV3* OptionalFloatSetting::createNode(float width) {
    return OptionalNumberSettingNode<OptionalFloatSetting>::create(std::static_pointer_cast<OptionalFloatSetting>(shared_from_this()), width);
}
