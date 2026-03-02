#include <Geode/Geode.hpp>
#include <Geode/modify/ItemInfoPopup.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCLayerColor.hpp>

#include <algorithm>
#include <numeric>

#include <constants.hpp>

//#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

#include <hooks/ItemInfoPopup.hpp>
#include <nytelyte.icon_kit_filter_and_sort/include/api/api.hpp>
namespace ikfs = nytelyte::icon_kit_filter_and_sort;

using namespace geode::prelude;

//class MoreInfoPopup : public CCObject {};


// i'm doing it weirdly like this to be able to modify More Icons popups as well
// once More Icons comes out


bool HookedItemInfoPopup::init(int icon, UnlockType unlockType) {

  if (!ItemInfoPopup::init(icon, unlockType)) return false;

  if (!Mod::get()->getSettingValue<bool>("disable-item-info-popup-animation-name-changes"))
    if (unlockType == UnlockType::GJItem && m_itemID >= 18 && m_itemID <= 20) {
      CCLabelBMFont *title = m_mainLayer->getChildByType<CCLabelBMFont>(0);
      TextArea* description = m_mainLayer->getChildByType<TextArea>(0);
      title->setString(fmt::format("Animation {}", m_itemID - 17).c_str());
      description->setString("You can <cl>buy</c> this <cg>Animation</c> at the <cp>Mechanic</c>!");
    }

  if (!Mod::get()->getSettingValue<bool>("disable-item-info-popup-wraith-icon-fixes")) {
    if (
      ((icon == 321 || icon == 351) && unlockType == UnlockType::Cube) ||
      ((icon == 71  || icon ==  57) && unlockType == UnlockType::Bird)
    ) {

      CCSize winSize = CCDirector::sharedDirector()->getWinSize();
      CCLabelBMFont* achievementLabel = static_cast<CCLabelBMFont*>(
        m_mainLayer->getChildByID("achievement-label")
      );

      TextArea* descriptionArea = static_cast<TextArea*>(
        m_mainLayer->getChildByID("description-area")
      );

      CCLabelBMFont* randomLabel = static_cast<CCLabelBMFont*>(
        m_mainLayer->getChildByID("random-label")
      );

      if (achievementLabel)
        achievementLabel->setString("Special Chest");
      else {
        achievementLabel = CCLabelBMFont::create("Special Chest", "goldFont.fnt");
        achievementLabel->setScale(0.55f);
        achievementLabel->setPosition({winSize.width/2, winSize.height/2 + 8.f});
        m_mainLayer->addChild(achievementLabel);
      }
      
      if (descriptionArea) {
        
        descriptionArea->setString(
          unlockType == UnlockType::Cube ?
            "You can <cl>find</c> this <cg>Cube</c> in a special <cy>chest</c>!" :
            "You can <cl>find</c> this <cg>UFO</c> in a special <cy>chest</c>!"
        );
        descriptionArea->setPositionY(descriptionArea->getPositionY() + 5.f);
      } else {
        TextArea* textArea = TextArea::create(
          unlockType == UnlockType::Cube ?
            "You can <cl>find</c> this <cg>Cube</c> in a special <cy>chest</c>!" :
            "You can <cl>find</c> this <cg>UFO</c> in a special <cy>chest</c>!"
          ,
          "bigFont.fnt", 1.0f, 600.f,
          {0.5f, 1.f}, 42.f, false
        );
        textArea->setAnchorPoint({0.5f, 0.5f});
        textArea->setScale(0.4f);
        textArea->setPosition({winSize.width/2, winSize.height/2 - 19.f});
        m_mainLayer->addChild(textArea);
      }

      if (!randomLabel) {
        const char* randomTextArray[] = {
          "IMPOSSIBRU!",
          "ehehehehehe...",
          "A wild lock appeared!",
          "Prove that you are worthy.",
          "Need. More. Power.",
          "That wont work.",
          "You must do something.",
          "It cannot be done.",
          "You are not ready.",
          "Player used tap.\nIt's not very effective."
        };
        const char* randomText = randomTextArray[
          geode::utils::random::generate(0, sizeof(randomTextArray)/sizeof(const char*))
        ];
        randomLabel = CCLabelBMFont::create(randomText, "chatFont.fnt");
        randomLabel->setScale(0.6f);
        randomLabel->setColor({0, 0, 0});
        randomLabel->setOpacity(75);
        randomLabel->setPosition({winSize.width/2, winSize.height/2 - 57.f});
        m_mainLayer->addChild(randomLabel);
      }
    }
  }

  return true;

}
