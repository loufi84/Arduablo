#include <Arduino.h>
#include <Arduboy2.h>

#include "systems/AI.h"

#include "world/Dungeon.h"

#include "entities/Player.h"
#include "entities/Monster.h"

#include "data/MonsterData.h"


extern Arduboy2 arduboy;


namespace {


// Utilitaires

int8_t sign(int16_t value) {

    if (value < 0) {
        return -1;
    }

    if (value > 0) {
        return 1;
    }

    return 0;
}


uint8_t distance(
    Position a,
    Position b
) {
    return
        abs(
            static_cast<int16_t>(a.x)
            -
            static_cast<int16_t>(b.x)
        )
        +
        abs(
            static_cast<int16_t>(a.y)
            -
            static_cast<int16_t>(b.y)
        );
}


// Direction vers le joueur

Direction getDirectionToPlayer(
    Position monsterPosition,
    Position playerPosition
) {

    const int16_t dx =
        static_cast<int16_t>(playerPosition.x)
        -
        monsterPosition.x;

    const int16_t dy =
        static_cast<int16_t>(playerPosition.y)
        -
        monsterPosition.y;


    if (abs(dx) > abs(dy)) {

        if (dx < 0) {
            return Direction::LEFT;
        }

        return Direction::RIGHT;
    }


    if (dy < 0) {
        return Direction::UP;
    }

    return Direction::DOWN;
}


// Case occupée ?

bool isOccupied(
    int8_t x,
    int8_t y,
    const Monster* monsters,
    uint8_t monsterCount,
    uint8_t selfIndex
) {

    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {

        if (i == selfIndex) {
            continue;
        }

        if (!monsters[i].isAlive()) {
            continue;
        }


        const Position position =
            monsters[i].getPosition();


        if (
            position.x == x
            &&
            position.y == y
        ) {
            return true;
        }
    }


    return false;
}


// Tentative de déplacement

bool tryMove(
    Monster& monster,
    uint8_t selfIndex,
    int8_t dx,
    int8_t dy,
    const Dungeon& dungeon,
    const Player& player,
    const Monster* monsters,
    uint8_t monsterCount
) {

    const Position position =
        monster.getPosition();


    const int8_t newX =
        position.x + dx;

    const int8_t newY =
        position.y + dy;


    // Hors carte
    if (
        newX < 0
        ||
        newY < 0
        ||
        newX >= Dungeon::MAP_WIDTH
        ||
        newY >= Dungeon::MAP_HEIGHT
    ) {
        return false;
    }


    // Mur
    if (
        !dungeon.isWalkable(
            static_cast<uint8_t>(newX),
            static_cast<uint8_t>(newY)
        )
    ) {
        return false;
    }


    // Joueur
    const Position playerPosition =
        player.getPosition();


    if (
        newX == playerPosition.x
        &&
        newY == playerPosition.y
    ) {
        return false;
    }


    // Autre monstre
    if (
        isOccupied(
            newX,
            newY,
            monsters,
            monsterCount,
            selfIndex
        )
    ) {
        return false;
    }


    monster.setPosition(
        newX,
        newY
    );


    return true;
}


// Déplacement vers le joueur

void moveTowardPlayer(
    Monster& monster,
    uint8_t selfIndex,
    const Dungeon& dungeon,
    const Player& player,
    const Monster* monsters,
    uint8_t monsterCount
) {

    const Position monsterPosition =
        monster.getPosition();

    const Position playerPosition =
        player.getPosition();


    const int16_t dx =
        static_cast<int16_t>(playerPosition.x)
        -
        monsterPosition.x;

    const int16_t dy =
        static_cast<int16_t>(playerPosition.y)
        -
        monsterPosition.y;


    const int8_t stepX =
        sign(dx);

    const int8_t stepY =
        sign(dy);


    // On privilégie l'axe
    // où la distance est la plus grande.

    if (abs(dx) >= abs(dy)) {

        if (
            stepX != 0
            &&
            tryMove(
                monster,
                selfIndex,
                stepX,
                0,
                dungeon,
                player,
                monsters,
                monsterCount
            )
        ) {
            return;
        }


        if (stepY != 0) {

            tryMove(
                monster,
                selfIndex,
                0,
                stepY,
                dungeon,
                player,
                monsters,
                monsterCount
            );
        }
    }

    else {

        if (
            stepY != 0
            &&
            tryMove(
                monster,
                selfIndex,
                0,
                stepY,
                dungeon,
                player,
                monsters,
                monsterCount
            )
        ) {
            return;
        }


        if (stepX != 0) {

            tryMove(
                monster,
                selfIndex,
                stepX,
                0,
                dungeon,
                player,
                monsters,
                monsterCount
            );
        }
    }
}


// Fuite du squelette

void moveAwayFromPlayer(
    Monster& monster,
    uint8_t selfIndex,
    const Dungeon& dungeon,
    const Player& player,
    const Monster* monsters,
    uint8_t monsterCount
) {

    const Position monsterPosition =
        monster.getPosition();

    const Position playerPosition =
        player.getPosition();


    const int16_t dx =
        static_cast<int16_t>(monsterPosition.x)
        -
        playerPosition.x;

    const int16_t dy =
        static_cast<int16_t>(monsterPosition.y)
        -
        playerPosition.y;


    const int8_t stepX =
        sign(dx);

    const int8_t stepY =
        sign(dy);


    if (abs(dx) >= abs(dy)) {

        if (
            stepX != 0
            &&
            tryMove(
                monster,
                selfIndex,
                stepX,
                0,
                dungeon,
                player,
                monsters,
                monsterCount
            )
        ) {
            return;
        }


        if (stepY != 0) {

            tryMove(
                monster,
                selfIndex,
                0,
                stepY,
                dungeon,
                player,
                monsters,
                monsterCount
            );
        }
    }

    else {

        if (
            stepY != 0
            &&
            tryMove(
                monster,
                selfIndex,
                0,
                stepY,
                dungeon,
                player,
                monsters,
                monsterCount
            )
        ) {
            return;
        }


        if (stepX != 0) {

            tryMove(
                monster,
                selfIndex,
                stepX,
                0,
                dungeon,
                player,
                monsters,
                monsterCount
            );
        }
    }
}


// Ligne de vue du squelette

bool hasClearLineOfSight(
    const Dungeon& dungeon,
    Position from,
    Position to
) {

    // Même colonne

    if (from.x == to.x) {

        const int8_t step =
            to.y > from.y
            ? 1
            : -1;


        for (
            int8_t y = from.y + step;
            y != to.y;
            y += step
        ) {

            if (
                !dungeon.isWalkable(
                    static_cast<uint8_t>(from.x),
                    static_cast<uint8_t>(y)
                )
            ) {
                return false;
            }
        }


        return true;
    }


    // Même ligne

    if (from.y == to.y) {

        const int8_t step =
            to.x > from.x
            ? 1
            : -1;


        for (
            int8_t x = from.x + step;
            x != to.x;
            x += step
        ) {

            if (
                !dungeon.isWalkable(
                    static_cast<uint8_t>(x),
                    static_cast<uint8_t>(from.y)
                )
            ) {
                return false;
            }
        }


        return true;
    }


    return false;
}


// Attaque corps-à-corps

void meleeAttack(
    Monster& monster,
    Player& player
) {

    monster.startAttack(
        getDirectionToPlayer(
            monster.getPosition(),
            player.getPosition()
        )
    );


    player.takeDamage(
        monster.getDamage()
    );
}


// Zombie

void updateZombie(
    Monster& monster,
    uint8_t index,
    const Dungeon& dungeon,
    Player& player,
    Monster* monsters,
    uint8_t monsterCount
) {

    const uint8_t dist =
        distance(
            monster.getPosition(),
            player.getPosition()
        );


    if (dist == 1) {

        meleeAttack(
            monster,
            player
        );

        return;
    }


    moveTowardPlayer(
        monster,
        index,
        dungeon,
        player,
        monsters,
        monsterCount
    );
}


// Brute

void updateBrute(
    Monster& monster,
    uint8_t index,
    const Dungeon& dungeon,
    Player& player,
    Monster* monsters,
    uint8_t monsterCount
) {

    // Pour l'instant même intelligence
    // que le zombie.
    // Sa différence vient de :
    // - beaucoup plus de HP
    // - plus de dégâts
    // - actionDelay plus important

    const uint8_t dist =
        distance(
            monster.getPosition(),
            player.getPosition()
        );


    if (dist == 1) {

        meleeAttack(
            monster,
            player
        );

        return;
    }


    moveTowardPlayer(
        monster,
        index,
        dungeon,
        player,
        monsters,
        monsterCount
    );
}


// Skeleton

void updateSkeleton(
    Monster& monster,
    uint8_t index,
    const Dungeon& dungeon,
    Player& player,
    Monster* monsters,
    uint8_t monsterCount
) {

    const Position monsterPosition =
        monster.getPosition();

    const Position playerPosition =
        player.getPosition();


    const uint8_t dist =
        distance(
            monsterPosition,
            playerPosition
        );


    // Trop près :
    // il essaie de fuir.

    if (dist <= 2) {

        moveAwayFromPlayer(
            monster,
            index,
            dungeon,
            player,
            monsters,
            monsterCount
        );

        return;
    }


    // Tir

    if (
        dist <= 4
        &&
        hasClearLineOfSight(
            dungeon,
            monsterPosition,
            playerPosition
        )
    ) {

        monster.startAttack(
            getDirectionToPlayer(
                monsterPosition,
                playerPosition
            )
        );


        // TEMPORAIRE :
        // plus tard ce sera un Projectile.
        player.takeDamage(
            monster.getDamage()
        );


        return;
    }


    // Sinon il cherche
    // à se rapprocher.

    moveTowardPlayer(
        monster,
        index,
        dungeon,
        player,
        monsters,
        monsterCount
    );
}


}


// Update général

void AI::updateMonsters(
    const Dungeon& dungeon,
    Player& player,
    Monster* monsters,
    uint8_t monsterCount
) {

    for (
        uint8_t i = 0;
        i < monsterCount;
        ++i
    ) {

        Monster& monster =
            monsters[i];


        if (!monster.isAlive()) {
            continue;
        }


        // Si le joueur vient de mourir,
        // inutile que tout le bestiaire
        // lui saute encore dessus xD
        if (!player.isAlive()) {
            return;
        }


        // Chaque type dispose de sa
        // propre fréquence d'action.
        if (
            !arduboy.everyXFrames(
                monster.getActionDelay()
            )
        ) {
            continue;
        }


        switch (
            monster.getType()
        ) {

            case MonsterType::ZOMBIE:

                updateZombie(
                    monster,
                    i,
                    dungeon,
                    player,
                    monsters,
                    monsterCount
                );

                break;


            case MonsterType::SKELETON:

                updateSkeleton(
                    monster,
                    i,
                    dungeon,
                    player,
                    monsters,
                    monsterCount
                );

                break;


            case MonsterType::BRUTE:
            case MonsterType::BONE_WARDEN:

                updateBrute(
                    monster,
                    i,
                    dungeon,
                    player,
                    monsters,
                    monsterCount
                );

                break;
        }
    }
}
