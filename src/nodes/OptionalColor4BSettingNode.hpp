#include <OptionalBaseSettingNode.hpp>
#include <OptionalColor4BSetting.hpp>
#include <Geode/ui/ColorPickPopup.hpp>

class OptionalColor4BSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalColor4BSetting> {
protected:
    geode::Button* m_colorBtn;

    bool init(std::shared_ptr<optional_settings::OptionalColor4BSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onSelectColor(cocos2d::CCObject*);
public:
    static OptionalColor4BSettingNode* create(std::shared_ptr<optional_settings::OptionalColor4BSetting> setting, float width);
};
