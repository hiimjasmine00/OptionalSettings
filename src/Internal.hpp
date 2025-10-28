#include <Geode/loader/SettingV3.hpp>

class Internal {
public:
    static void registerCustomSettingType(std::string_view type, geode::SettingGeneratorV3 generator);
};
