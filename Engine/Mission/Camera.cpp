#include "Camera.h"
#include <iostream>

using namespace std;


Camera::Camera()
{
    //cameraView.setSize(1280, 720);
    //cameraView.setCenter(0, 610);
}

void Camera::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	offsetCoords.x += camera_xspeed;
	view.move(offsetCoords);
	window.setView(view);
}

void Camera::Work(sf::RenderWindow& window)
{
    zoom = 1;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    {
        zoom = 1.002;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        zoom = 0.998;
    }


    if(walk)
    followobject_x += 0.25;

    if(camera_x < followobject_x - 10 - 400)
    {
        camera_xspeed += (followobject_x - camera_x) / 10000;
    }
    else if(camera_x > followobject_x + 10 + 400)
    {
        camera_xspeed -= (camera_x - followobject_x) / 10000;
    }
    else
    {
        camera_xspeed = 0;
    }


    if(camera_xspeed >= 1)
    camera_xspeed = 1;

    camera_x += camera_xspeed;

    zoomViewAt(sf::Vector2i(420,610),window,zoom);

    //cameraView.move(0, (cameraView.getSize().x / float(1280)));
    //cameraView.setCenter(640,cameraView.getSize().y/2);
    //window.setView(cameraView);
}