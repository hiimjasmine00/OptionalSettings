#include <OptionalBaseSettingNode.hpp>
#include <OptionalBoolSetting.hpp>

class OptionalBoolSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalBoolSetting> {
protected:
    CCMenuItemToggler* m_toggle;

    bool init(std::shared_ptr<optional_settings::OptionalBoolSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onToggle(cocos2d::CCObject*);
public:
    static OptionalBoolSettingNode* create(std::shared_ptr<optional_settings::OptionalBoolSetting> setting, float width);
};
