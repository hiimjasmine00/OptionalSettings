#include <OptionalBaseSettingNode.hpp>
#include <OptionalFileSetting.hpp>

class OptionalFileSettingNode : public optional_settings::OptionalBaseSettingNode<optional_settings::OptionalFileSetting> {
protected:
    cocos2d::CCSprite* m_fileIcon;
    geode::Label* m_nameLabel;
    geode::async::TaskHolder<geode::Result<std::optional<asp::fs::Path>>> m_pickListener;
    geode::Button* m_selectBtn;

    bool init(std::shared_ptr<optional_settings::OptionalFileSetting> setting, float width);
    void updateState(cocos2d::CCNode* invoker) override;
    void onPickFile(cocos2d::CCObject*);
public:
    static OptionalFileSettingNode* create(std::shared_ptr<optional_settings::OptionalFileSetting> setting, float width);
};
