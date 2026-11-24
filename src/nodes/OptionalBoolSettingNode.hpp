#include <OptionalBaseSettingNode.hpp>
#include <OptionalBoolSetting.hpp>

class OptionalBoolSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalBoolSetting> {
protected:
    geode::Toggler* m_toggle;

    bool init(std::shared_ptr<optional_settings::OptionalBoolSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
public:
    static OptionalBoolSettingNode* create(std::shared_ptr<optional_settings::OptionalBoolSetting> setting, float width);
};
