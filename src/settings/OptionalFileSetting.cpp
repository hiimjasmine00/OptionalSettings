#include "OptionalFileSetting.hpp"
#include "../Internal.hpp"
#include "../nodes/OptionalFileSettingNode.hpp"
#include <Geode/loader/Dirs.hpp>
#include <matjson/std.hpp>

using namespace geode::prelude;
using namespace optional_settings;

$on_mod(Loaded) {
    Internal::registerCustomSettingType("optional-file", &OptionalFileSetting::parse);
    Internal::registerCustomSettingType("optional-folder", &OptionalFileSetting::parse);
}

class OptionalFileSetting::Impl final {
public:
    bool folder = false;
    bool useSaveDialog = false;
    std::optional<std::vector<file::FilePickOptions::Filter>> filters;
};

OptionalFileSetting::OptionalFileSetting(PrivateMarker) : m_impl(std::make_shared<Impl>()) {}

Result<std::shared_ptr<SettingV3>> OptionalFileSetting::parse(const std::string& key, const std::string& id, const matjson::Value& json) {
    auto ret = std::make_shared<OptionalFileSetting>(PrivateMarker());

    auto root = checkJson(json, "OptionalFileSetting");
    ret->parseBaseProperties(key, id, root);

    if (ret->hasDefaultValue()) {
        ret->setStoredDefaultValue(ret->getStoredDefaultValue().make_preferred());
        try {
            ret->setStoredDefaultValue(fmt::format(
                fmt::runtime(string::pathToString(ret->getStoredDefaultValue())),
                fmt::arg("gd_dir", dirs::getGameDir()),
                fmt::arg("gd_save_dir", dirs::getSaveDir()),
                fmt::arg("mod_config_dir", dirs::getModConfigDir() / id),
                fmt::arg("mod_save_dir", dirs::getModsSaveDir() / id),
                fmt::arg("temp_dir", dirs::getTempDir()),
                fmt::arg("gd_resources_dir", dirs::getResourcesDir()),
                fmt::arg("mod_runtime_dir", dirs::getModRuntimeDir() / id),
                fmt::arg("mod_resources_dir", dirs::getModRuntimeDir() / id / "resources" / id)
            ));
        }
        catch (const fmt::format_error& e) {
            return Err("Invalid format string for file setting path: {}", e.what());
        }
        ret->setStoredValue(ret->getStoredDefaultValue());
    }

    std::string type;
    root.needs("type").into(type);
    if (type.ends_with("folder")) {
        ret->m_impl->folder = true;
    }
    else if (type.ends_with("file")) {
        if (auto controls = root.has("control")) {
            std::string dialogType;
            controls.has("dialog").into(dialogType);
            if (dialogType == "save") ret->m_impl->useSaveDialog = true;
            else if (dialogType == "open") ret->m_impl->useSaveDialog = false;
            else if (!dialogType.empty()) {
                return Err("Setting '{}' in mod {}: unknown \"dialog\" type \"{}\"", key, id, dialogType);
            }

            auto filters = std::vector<file::FilePickOptions::Filter>();
            for (auto& item : controls.has("filters").items()) {
                file::FilePickOptions::Filter filter;
                item.has("description").into(filter.description);
                item.has("files").into(filter.files);
                filters.push_back(filter);
            }
            if (!filters.empty()) ret->m_impl->filters = std::move(filters);
        }
    }

    root.checkUnknownKeys();
    return root.ok(std::static_pointer_cast<SettingV3>(std::move(ret)));
}

Result<> OptionalFileSetting::isValid(const std::filesystem::path& value) const {
    if (value == getStoredDefaultValue()) return Ok();
    std::error_code code;
    if (m_impl->folder) {
        if (!std::filesystem::is_directory(value, code)) return Err("Value must be a folder");
    }
    else {
        if (!std::filesystem::is_regular_file(value, code)) return Err("Value must be a file");
    }
    return Ok();
}

bool OptionalFileSetting::isFolder() const {
    return m_impl->folder;
}

bool OptionalFileSetting::useSaveDialog() const {
    return m_impl->useSaveDialog;
}

std::optional<std::vector<file::FilePickOptions::Filter>> OptionalFileSetting::getFilters() const {
    return m_impl->filters;
}

SettingNodeV3* OptionalFileSetting::createNode(float width) {
    return OptionalFileSettingNode::create(std::static_pointer_cast<OptionalFileSetting>(shared_from_this()), width);
}
