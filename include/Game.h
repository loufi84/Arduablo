#include "entities/Player.h"
#include "entities/Monster.h"

enum class GameState : uint8_t {
    TITLE,
    PLAYING,
    INVENTORY,
    TOWN,
    GAME_OVER
};

class Game {
    public:
        void begin();
        void update();
        void render();

    private:
        GameState state = GameState::TITLE;

        Dungeon dungeon;
        Player player;
        Monster monsters[MAX_MONSTERS];
};
