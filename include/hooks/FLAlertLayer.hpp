#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

struct HookedFLAlertLayer : geode::Modify<HookedFLAlertLayer, FLAlertLayer> {

  struct Fields {
    // if this is not nullopt, then we are either an ItemInfoPopup or a More Icons MoreInfoPopup (MI support not added atm)
    std::optional<UnlockType> unlockType;
    std::optional<int> itemID;
    //std::optional<std::string> itemIDMI;
  };

  void addItemInfoPopupArrows(bool);
  void onPrevNext(CCObject*);

};
