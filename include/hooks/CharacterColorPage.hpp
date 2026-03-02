#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CharacterColorPage.hpp>

struct HookedCharacterColorPage : geode::Modify<HookedCharacterColorPage, CharacterColorPage> {
  static void onModify(auto&);
  $override void onPlayerColor(cocos2d::CCObject*);
};
