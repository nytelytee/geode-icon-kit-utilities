#include <hooks/FLAlertLayer.hpp>
#include <nytelyte.icon_kit_filter_and_sort/include/api/api.hpp>
#include <constants.hpp>
#include <numeric>
namespace ikfs = nytelyte::icon_kit_filter_and_sort;

using namespace geode::prelude;

enum ItemInfoPopupButtonAction { Previous, Next };

void HookedFLAlertLayer::addItemInfoPopupArrows(bool isGeodePopup) {
  FLAlertLayer* to = static_cast<FLAlertLayer*>(this);
  CCNode* background = to->m_mainLayer->getChildByID("background");
  if (!background) return;

  CCSprite *prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
  CCSprite *nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
  nextSprite->setFlipX(true);

  CCMenuItemSpriteExtra *prev = CCMenuItemSpriteExtra::create(
    prevSprite, to, menu_selector(HookedFLAlertLayer::onPrevNext)
  );
  CCMenuItemSpriteExtra *next = CCMenuItemSpriteExtra::create(
    nextSprite, to, menu_selector(HookedFLAlertLayer::onPrevNext)
  );
  prev->setTag(ItemInfoPopupButtonAction::Previous);
  next->setTag(ItemInfoPopupButtonAction::Next);

  CCSize size = background->getContentSize();
  float bottomBG = background->boundingBox().getMinY();
  float bottomMenu = to->m_buttonMenu->boundingBox().getMinY();
  float pad = 25.f;
  if (isGeodePopup) {
    prev->setPosition({-pad, size.height/2 - (bottomMenu - bottomBG)});
    next->setPosition({size.width + pad, size.height/2 - (bottomMenu - bottomBG)});
  } else {
    prev->setPosition({-size.width/2 - pad, size.height/2 - (bottomMenu - bottomBG)});
    next->setPosition({size.width/2 + pad, size.height/2 - (bottomMenu - bottomBG)});
  }
  
  to->m_buttonMenu->addChild(prev);
  to->m_buttonMenu->addChild(next);
}

void HookedFLAlertLayer::onPrevNext(CCObject *sender) {
  CCMenuItemSpriteExtra* button = static_cast<CCMenuItemSpriteExtra*>(sender);
  ItemInfoPopupButtonAction action = static_cast<ItemInfoPopupButtonAction>(button->getTag());
  
  CCScene* scene = CCScene::get();
  GJGarageLayer *garage = scene->getChildByType<GJGarageLayer>(0);
  GJPathPage *pathPage = scene->getChildByType<GJPathPage>(-1);
  ShardsPage *shardsPage = scene->getChildByType<ShardsPage>(-1);

  if (!garage && !pathPage && !shardsPage) return;
  
  GJPathSprite *pathSprite = nullptr;
  if (pathPage)
    pathSprite = pathPage->m_mainLayer->getChildByType<GJPathSprite>(-1);
  
  UnlockType unlockType = *m_fields->unlockType;
  int itemID = *m_fields->itemID;

  // this is rebuilt on each new popup; we don't keep track of any state
  // it shouldn't be too much of a problem, there are like 500 or so vanilla icons
  // and i doubt that MI users--once i add MI support--are going to have thousands
  // of icons for an optimization that keeps track of state to become necessary
  // i may add it if it does but, for now, this entire thing is O(n),
  // where n is the number of icons
  std::span<const std::pair<const int, const UnlockType>> iconSpan;
  std::vector<std::pair<const int, const UnlockType>> dynamicList;

  if (pathSprite)
    iconSpan = PATHS_UNLOCK_ORDER[pathSprite->m_pathNumber];
  else if (shardsPage)
    iconSpan = SHARDS_OF_POWER_UNLOCK_ORDER;
  else if (!IS_NORMAL_UNLOCK_TYPE(unlockType))
    iconSpan = SPECIAL_UNLOCK_ORDER[static_cast<int>(unlockType)];
  else {
    // default case, the garage
    // if IKF&S is installed, get the order from it, otherwise it's just the integers in order
    // the special ones are handled above this, so it shouldn't be a problem to use iota from 1
    for (
      int orderedIconID : (
        ikfs::getVanillaIconsInOrder(unlockType).unwrapOrElse([unlockType](){
          int count = GameManager::get()->countForType(GameManager::get()->unlockTypeToIconType(int(unlockType)));
          std::vector<int> order(count);
          std::iota(order.begin(), order.end(), 1);
          return order;
        })
      )
    )
      dynamicList.push_back({orderedIconID, unlockType});
    iconSpan = dynamicList;
  }

  std::pair<int, UnlockType> newItem;
  std::span<const std::pair<const int, const UnlockType>>::iterator p_first = iconSpan.begin();
  std::span<const std::pair<const int, const UnlockType>>::iterator p_last = iconSpan.end() - 1;
  std::span<const std::pair<const int, const UnlockType>>::iterator p_current = std::find(p_first, p_last + 1, std::pair<int, UnlockType>{itemID, unlockType});
  if (action == ItemInfoPopupButtonAction::Next) {
    if (p_current != p_last) newItem = *(p_current+1);
    if (p_current == p_last) newItem = *p_first;
  } else {
    if (p_current != p_first) newItem = *(p_current-1);
    if (p_current == p_first) newItem = *p_last;
  }
  
  removeFromParent();
  ItemInfoPopup* popup = ItemInfoPopup::create(newItem.first, newItem.second);
  popup->m_noElasticity = true;
  popup->show();
}
