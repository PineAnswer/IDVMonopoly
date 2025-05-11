#include "playrole.h"

PlayRole::PlayRole() {
    for(int i=0;i<4;i++){
        cards.emplace_back();
        cards[i].name=Card::None;
    }
}
