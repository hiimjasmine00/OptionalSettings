#include <OptionalBaseSettingNode.hpp>
#include <OptionalColor3BSetting.hpp>
#include <Geode/ui/ColorPickPopup.hpp>

class OptionalColor3BSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalColor3BSetting> {
protected:
    CCMenuItemSpriteExtra* m_colorBtn;
    ColorChannelSprite* m_colorSprite;

    bool init(std::shared_ptr<optional_settings::OptionalColor3BSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onSelectColor(cocos2d::CCObject*);
public:
    static OptionalColor3BSettingNode* create(std::shared_ptr<optional_settings::OptionalColor3BSetting> setting, float width);
};
