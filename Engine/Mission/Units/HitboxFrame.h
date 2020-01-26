#ifndef HITBOXFRAME_H
#define HITBOXFRAME_H

#include <SFML/Graphics.hpp>

///Class for managing object hitboxes defined by V4Anim format

class HitboxFrame
{
    private:
        ///timestamp
        float time;
        HitboxFrame();
        ~HitboxFrame();
        ///global position, so the hitbox can be moved around as a whole

        ///hitboxes' rect that react with other collidables
        std::vector<sf::Vector2f> vertices;

        //float x;
        //float y;
        //float width;
        //float height;
    public:
        float g_x=0;
        float g_y=0;
        float rotation=0;
        float scaleX=1;
        float scaleY=1;

        void addVertex(float relX, float relY);
        std::vector<sf::Vector2f> getCurrentVertices();
        float minProjection(float axisAngle);
        float maxProjection(float axisAngle);
};

#endif // HITBOXFRAME_H
