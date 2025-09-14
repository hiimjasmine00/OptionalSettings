#include <OptionalBaseSettingNode.hpp>
#include <OptionalFileSetting.hpp>

class OptionalFileSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalFileSetting> {
protected:
    cocos2d::CCSprite* m_fileIcon;
    cocos2d::CCLabelBMFont* m_nameLabel;
    geode::EventListener<geode::Task<geode::Result<std::filesystem::path>>> m_pickListener;
    CCMenuItemSpriteExtra* m_selectBtn;
    cocos2d::CCSprite* m_selectBtnSpr;

    bool init(std::shared_ptr<optional_settings::OptionalFileSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onPickFile(cocos2d::CCObject*);
public:
    static OptionalFileSettingNode* create(std::shared_ptr<optional_settings::OptionalFileSetting> setting, float width);
};
