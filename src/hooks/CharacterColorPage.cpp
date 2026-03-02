#include <hooks/CharacterColorPage.hpp>

using namespace geode::prelude;

void HookedCharacterColorPage::onModify(auto& self) {
  // this may skip calling the original entirely (holding shift shows the item info popup, regardless of select status),
  // so make sure they don't unintentionally call any hooks if we don't intend to call the original
  Result<> result = self.setHookPriority("CharacterColorPage::onPlayerColor", Priority::First);
  if (!result) log::error("Failed to set hook priority, some mods may not run their functions.");
}

void HookedCharacterColorPage::onPlayerColor(CCObject *sender) {
  if (Mod::get()->getSettingValue<bool>("disable-shift-clicking-on-icons")) return CharacterColorPage::onPlayerColor(sender);

  CCMenuItemSpriteExtra *item = static_cast<CCMenuItemSpriteExtra *>(sender);
  int itemID = item ? item->getTag() : -1;

  // hold shift when clicking an icon to always open the icon popup, rather than selecting it, even if the icon is unlocked and unselected
  if (!CCDirector::get()->getKeyboardDispatcher()->getShiftKeyPressed()) return CharacterColorPage::onPlayerColor(sender);
  
  m_delegate->showUnlockPopup(itemID, m_colorMode == 0 ? UnlockType::Col1 : UnlockType::Col2);
}
