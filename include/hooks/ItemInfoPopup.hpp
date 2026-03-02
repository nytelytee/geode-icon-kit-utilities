#pragma once

#include<Geode/Geode.hpp>
#include<Geode/modify/ItemInfoPopup.hpp>

struct HookedItemInfoPopup : geode::Modify<HookedItemInfoPopup, ItemInfoPopup> {

  using CCLayerColor::init;

  $override
  bool init(int, UnlockType);

};
