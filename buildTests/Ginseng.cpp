#include <ginseng/ginseng.hpp>
#include <iostream>

using DB = Ginseng::Database<>;
using Ginseng::Not;
using Ginseng::Tag;

// Components can be any value type.

struct NameCom {
    std::string name;
};

struct PositionCom {
    double x;
    double y;
};

// Tag components will not contain a value (no allocation).

struct IsEnemy {};
using IsEnemyTag = Tag<IsEnemy>;

struct Game {
    DB db; // Databases are value types.

    Game() {
        // db.makeEntity() returns an entity ID (similar to an iterator).
        auto player = db.makeEntity();

        // db.makeComponent() copies the given component into the entity.
        db.makeComponent(player, NameCom{"The Player"});
        db.makeComponent(player, PositionCom{12, 42});

        auto enemy = db.makeEntity();
        db.makeComponent(enemy, NameCom{"An Enemy"});
        db.makeComponent(enemy, PositionCom{7, 53});
        db.makeComponent(enemy, IsEnemyTag{});
    }

    void run_game() {
        // db.visit() automatically detects visitor parameters.
        db.visit([](const NameCom& name, const PositionCom& pos){
            std::cout << "Entity " << name.name
                      << " is at (" << pos.x << "," << pos.y << ")."
                      << std::endl;
        });

        // The Not<> annotation can be used to skip unwanted entities.
        db.visit([](const NameCom& name, Not<IsEnemyTag>){
            std::cout << name.name << " is not an enemy." << std::endl;
        });
    }
};

int main(int argc, char** argv)
{
    Game g{};
    g.run_game();
    return 0;
}
