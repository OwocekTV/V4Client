#ifndef KIRAJIN_YARI_1_H
#define KIRAJIN_YARI_1_H

#include <SFML/Graphics.hpp>
#include "../../../../Config.h"
#include "../../Entity.h"

using namespace std;

class Kirajin_Yari_1 : public Entity
{
    public:
    sf::Clock death_timer;
    sf::Clock attack_timer;

    bool canThrow = true;
    bool threw = false;
    bool enemy_in_range = false;

    float hspeed = 0;
    float vspeed = 0;
    float gravity = 981;

    enum ACTIONS
    {
        HIDING = 0,
        IDLE = 1,
        ATTACK = 2,
        WALK = 3
    };

    int action = 1;

    bool talk = false;
    string talk_id = "";

    int swap_layer = 0;

    Kirajin_Yari_1();
    void LoadConfig(Config *thisConfigs);
    void Draw(sf::RenderWindow& window);
    bool doAttack();
    void die();
    void OnCollide(CollidableObject* otherObject, int collidedWith=-1, vector<string> collisionData = {});
};

#endif // KIRAJIN_YARI_1
