#include <OptionalBaseSettingNode.hpp>
#include <OptionalColor4BSetting.hpp>
#include <Geode/ui/ColorPickPopup.hpp>

class OptionalColor4BSettingNode
    : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalColor4BSetting>
    , public geode::ColorPickPopupDelegate {
protected:
    CCMenuItemSpriteExtra* m_colorBtn;
    ColorChannelSprite* m_colorSprite;

    bool init(std::shared_ptr<optional_settings::OptionalColor4BSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onSelectColor(cocos2d::CCObject*);
    void updateColor(const cocos2d::ccColor4B& color) override;
public:
    static OptionalColor4BSettingNode* create(std::shared_ptr<optional_settings::OptionalColor4BSetting> setting, float width);
};
