#include <OptionalBaseSettingNode.hpp>
#include <OptionalStringSetting.hpp>
#include <Geode/ui/TextInput.hpp>

class OptionalStringSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalStringSetting> {
protected:
    geode::TextInput* m_input;
    cocos2d::CCSprite* m_arrowLeftSpr = nullptr;
    cocos2d::CCSprite* m_arrowRightSpr = nullptr;
    CCMenuItemSpriteExtra* m_arrowLeftBtn = nullptr;
    CCMenuItemSpriteExtra* m_arrowRightBtn = nullptr;

    bool init(std::shared_ptr<optional_settings::OptionalStringSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onArrow(cocos2d::CCObject* sender);
public:
    static OptionalStringSettingNode* create(std::shared_ptr<optional_settings::OptionalStringSetting> setting, float width);
};
