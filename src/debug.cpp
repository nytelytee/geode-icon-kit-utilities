#include <Geode/loader/GameEvent.hpp>

#include <geode.devtools/include/API.hpp>

#include <constants.hpp>

$on_mod(Loaded) {
    devtools::waitForDevTools([] {
        devtools::registerNode<CharacterColorPage>([](CharacterColorPage* node) {
            devtools::label("Icon Kit Utilities\n");
            devtools::label(fmt::format("Current mode: {}", node->m_colorMode).c_str());
        });
    });
}


