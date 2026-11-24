#include "OptionalFileSettingNode.hpp"
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/NineSlice.hpp>

using namespace geode::prelude;
using namespace optional_settings;

OptionalFileSettingNode* OptionalFileSettingNode::create(std::shared_ptr<OptionalFileSetting> setting, float width) {
    auto ret = new OptionalFileSettingNode();
    if (ret->init(std::move(setting), width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool OptionalFileSettingNode::init(std::shared_ptr<OptionalFileSetting> setting, float width) {
    if (!OptionalBaseSettingNode::init(setting, width)) return false;

    auto buttonMenu = getButtonMenu();

    auto labelBG = NineSlice::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    labelBG->setScale(0.25f);
    labelBG->setColor({ 0, 0, 0 });
    labelBG->setOpacity(90);
    labelBG->setContentSize({ 420.0f, 80.0f });
    buttonMenu->addChildAtPosition(labelBG, Anchor::Center, { -10.0f, 0.0f });

    m_fileIcon = CCSprite::create();
    buttonMenu->addChildAtPosition(m_fileIcon, Anchor::Left, { 5.0f, 0.0f });

    m_nameLabel = Label::create("", "bigFont.fnt");
    buttonMenu->addChildAtPosition(m_nameLabel, Anchor::Left, { 13.0f, 0.0f }, { 0.0f, 0.5f });

    m_selectBtn = Button::createWithSpriteFrameName("GJ_plus2Btn_001.png", [this](Button* sender) {
        onPickFile(sender);
    });
    m_selectBtn->setScale(0.7f);
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
        if (def.size() <= 20) isTextualDefaultValue = folder ? !def.isDir() : !def.isFile();
    }

    OptionalBaseSettingNode::updateState(invoker);

    m_fileIcon->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(
        folder ? "folderIcon_001.png" : "geode.loader/file.png"));
    limitNodeSize(m_fileIcon, { 10.0f, 10.0f }, 1.0f, 0.1f);

    auto value = getStoredValue();
    if (value.empty() || isTextualDefaultValue) {
        if (isTextualDefaultValue) m_nameLabel->setText(def.string());
        else if (folder) m_nameLabel->setText("No Folder Selected");
        else m_nameLabel->setText("No File Selected");
        m_nameLabel->setColor({ 166, 166, 166 });
        m_nameLabel->setOpacity(155);
    }
    else {
        auto filename = value.filename();
        m_nameLabel->setText(filename.has_value() ? filename->string() : "");
        m_nameLabel->setColor({ 255, 255, 255 });
        m_nameLabel->setOpacity(255);
    }
    m_nameLabel->setLimitLabelWidth(75.0f, 0.35f, 0.1f);

    auto enable = setting->shouldEnable() && isEnabled();
    m_selectBtn->setOpacity(enable ? 255 : 155);
    m_selectBtn->setColor(enable ? ccColor3B { 255, 255, 255 } : ccColor3B { 166, 166, 166 });
    m_selectBtn->setEnabled(enable);
}

void OptionalFileSettingNode::onPickFile(CCObject*) {
    auto setting = getSetting();
    auto value = getStoredValue();
    std::error_code ec;
    m_pickListener.spawn(file::pick(
        setting->isFolder() ? file::PickMode::OpenFolder : (setting->useSaveDialog() ? file::PickMode::SaveFile : file::PickMode::OpenFile),
        {
            value.empty() || !value.parent().value_or("").exists() ? dirs::getGameDir() : value,
            setting->getFilters().value_or(std::vector<file::FilePickOptions::Filter>())
        }
    ), [this](Result<std::optional<asp::fs::Path>> value) {
        if (value.isOk()) {
            auto path = std::move(value).unwrap();
            if (path.has_value()) setStoredValue(path.value(), nullptr);
        }
        else if (value.isErr()) {
            FLAlertLayer::create("Failed", fmt::format("Failed to pick file: {}", value.unwrapErr()), "OK")->show();
        }
    });
}
