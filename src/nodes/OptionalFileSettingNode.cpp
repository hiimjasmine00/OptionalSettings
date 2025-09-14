#include "OptionalFileSettingNode.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/loader/Dirs.hpp>

using namespace geode::prelude;
using namespace optional_settings;

OptionalFileSettingNode* OptionalFileSettingNode::create(std::shared_ptr<OptionalFileSetting> setting, float width) {
    auto ret = new OptionalFileSettingNode();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionalFileSettingNode::init(std::shared_ptr<OptionalFileSetting> setting, float width) {
    if (!OptionalBaseSettingNode::init(setting, width)) return false;

    auto buttonMenu = getButtonMenu();

    auto labelBG = extension::CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    labelBG->setScale(.25f);
    labelBG->setColor({ 0, 0, 0 });
    labelBG->setOpacity(90);
    labelBG->setContentSize({ 420, 80 });
    buttonMenu->addChildAtPosition(labelBG, Anchor::Center, { -10.0f, 0.0f });

    m_fileIcon = CCSprite::create();
    buttonMenu->addChildAtPosition(m_fileIcon, Anchor::Left, { 5.0f, 0.0f });

    m_nameLabel = CCLabelBMFont::create("", "bigFont.fnt");
    buttonMenu->addChildAtPosition(m_nameLabel, Anchor::Left, { 13.0f, 0.0f }, { 0.0f, 0.5f });

    m_selectBtnSpr = CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png");
    m_selectBtnSpr->setScale(.7f);
    m_selectBtn = CCMenuItemSpriteExtra::create(m_selectBtnSpr, this, menu_selector(OptionalFileSettingNode::onPickFile));
    buttonMenu->addChildAtPosition(m_selectBtn, Anchor::Right, { -5.0f, 0.0f });

    updateState(nullptr);

    return true;
}

void OptionalFileSettingNode::updateState(CCNode* invoker) {
    auto setting = getSetting();
    auto def = setting->getStoredDefaultValue();
    auto folder = setting->isFolder();
    auto isTextualDefaultValue = false;
    if (!hasNonDefaultValue()) {
        if (string::pathToString(def).size() <= 20) {
            std::error_code ec;
            isTextualDefaultValue = folder ? !std::filesystem::is_directory(def, ec) : !std::filesystem::is_regular_file(def, ec);
        }
    }

    OptionalBaseSettingNode::updateState(invoker);

    m_fileIcon->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(
        folder ? "folderIcon_001.png" : "geode.loader/file.png"));
    limitNodeSize(m_fileIcon, { 10.0f, 10.0f }, 1.0f, 0.1f);

    auto value = getStoredValue();
    if (value.empty() || isTextualDefaultValue) {
        if (isTextualDefaultValue) m_nameLabel->setString(string::pathToString(def).c_str());
        else m_nameLabel->setString(folder ? "No Folder Selected" : "No File Selected");
        m_nameLabel->setColor({ 166, 166, 166 });
        m_nameLabel->setOpacity(155);
    }
    else {
        m_nameLabel->setString(string::pathToString(value.filename()).c_str());
        m_nameLabel->setColor({ 255, 255, 255 });
        m_nameLabel->setOpacity(255);
    }
    m_nameLabel->limitLabelWidth(75.0f, 0.35f, 0.1f);

    auto enable = setting->shouldEnable() && isEnabled();
    m_selectBtnSpr->setOpacity(enable ? 255 : 155);
    m_selectBtnSpr->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
    m_selectBtn->setEnabled(enable);
}

void OptionalFileSettingNode::onPickFile(CCObject*) {
    m_pickListener.bind([this](Task<Result<std::filesystem::path>>::Event* event) {
        if (auto value = event->getValue()) {
            if (value->isOk()) setStoredValue(value->unwrap(), nullptr);
            else if (value->isErr()) FLAlertLayer::create("Failed", fmt::format("Failed to pick file: {}", value->unwrapErr()), "OK")->show();
        }
    });

    auto setting = getSetting();
    auto value = getStoredValue();
    std::error_code ec;
    m_pickListener.setFilter(file::pick(
        setting->isFolder() ? file::PickMode::OpenFolder : (setting->useSaveDialog() ? file::PickMode::SaveFile : file::PickMode::OpenFile),
        {
            value.empty() || !std::filesystem::exists(value.parent_path(), ec) ? dirs::getGameDir() : value,
            setting->getFilters().value_or(std::vector<file::FilePickOptions::Filter>())
        }
    ));
}
