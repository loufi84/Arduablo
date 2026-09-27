#include "Config.h"

#include "entities/Player.h"
#include "entities/Monster.h"
#include "world/Dungeon.h"
#include "rendering/Renderer.h"
#include "rendering/Hud.h"
#include "rendering/Camera.h"
#include "items/GroundItem.h"

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
        GroundItem groundItems[MAX_ITEMS];

    private:
        void startNewGame();
        void generateLevel();
        void tryDropLoot(Position position);
        void tryPickupItem();

        uint8_t depth = 1;

        GameState state = GameState::TITLE;

        Dungeon dungeon;
        Player player;
        Monster monsters[MAX_MONSTERS];

        Renderer renderer;
        Hud hud;
        Camera camera;
};
