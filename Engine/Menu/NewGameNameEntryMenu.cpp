#include "NewGameNameEntryMenu.h"
#include "ButtonList.h"
#include "iostream"
#include "../V4Core.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include "MenuButton.h"
NewGameNameEntryMenu::NewGameNameEntryMenu()
{
    //ctor
    f_font.loadFromFile("resources/fonts/p4kakupop-pro.ttf");
    //f_font.loadFromFile("resources/fonts/arial.ttf");
    t_title.setFont(f_font);

    t_title.setCharacterSize(55);
    //t_title.setColor(sf::Color::White);
    t_title.setFillColor(sf::Color::White);
    t_title.setString("Greetings Almighty! What is your name?");
    t_title.setOrigin(t_title.getGlobalBounds().width/2,t_title.getGlobalBounds().height/2);

    t_enteredtext.setFont(f_font);

    t_enteredtext.setCharacterSize(35);
    //t_enteredtext.setColor(sf::Color::White);
    t_enteredtext.setFillColor(sf::Color::White);
    t_enteredtext.setString("");
    t_enteredtext.setOrigin(t_enteredtext.getGlobalBounds().width/2,t_enteredtext.getGlobalBounds().height/2);

    //t_pressToContinue.setFont(f_font);
    //t_pressToContinue.setCharacterSize(32);
    //t_pressToContinue.setColor(sf::Color::White);
    //t_pressToContinue.setFillColor(sf::Color::White);
    //t_pressToContinue.setString("Press any key to continue...");
    //t_pressToContinue.setOrigin(t_pressToContinue.getGlobalBounds().width/2,t_pressToContinue.getGlobalBounds().height/2);

    mm_bigBox.setSize(sf::Vector2f(100,10));
    mm_bigBox.setFillColor(sf::Color(4,0,90));

    mm_titleBox.setSize(sf::Vector2f(100,10));
    mm_titleBox.setFillColor(sf::Color::Red);
    isActive=true;
}
void NewGameNameEntryMenu::Initialise(Config *thisConfigs,std::map<int,bool> *keymap,V4Core *parent,Menu* parentMenu){
    Scene::Initialise(thisConfigs,keymap,parent);
    v4core->menus.push_back(&optionsMenu);
    buttonList.Initialise(&f_font,*thisConfig,keymap,&(v4core->currentController),this);
}
void NewGameNameEntryMenu::EventFired(sf::Event event){
    if(isActive){
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code==sf::Keyboard::Enter) {
                buttonList.SelectButton(0);
            }
            if(event.key.code==sf::Keyboard::Backspace) {
                sf::String currentText = t_enteredtext.getString();
                if (currentText.getSize()>0){
                    currentText.erase(t_enteredtext.getString().getSize() - 1, 1);
                    t_enteredtext.setString(currentText);
                    t_enteredtext.setOrigin(t_enteredtext.getGlobalBounds().width/2,t_enteredtext.getGlobalBounds().height/2);
                }
            }
        }
        if(event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode != '\b' && event.text.unicode != '\n' && event.text.unicode != '\r' && event.text.unicode != '\r\n') {
                t_enteredtext.setString(t_enteredtext.getString()+event.text.unicode);
                t_enteredtext.setOrigin(t_enteredtext.getGlobalBounds().width/2,t_enteredtext.getGlobalBounds().height/2);
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            // We use mouse released so a user can change their mind by keeping the mouse held and moving away.
            buttonList.MouseReleasedEvent(event);
        }
    }
}
void NewGameNameEntryMenu::Update(sf::RenderWindow &window, float fps)
{
    if(isActive){

        mm_bigBox.setSize(sf::Vector2f(window.getSize().x,window.getSize().y-200));
        //mm_smallerBox.setSize(sf::Vector2f(100,10));
        //mm_titleBox.setSize(sf::Vector2f(100,10));

        mm_bigBox.setPosition(0,85);
        //mm_smallerBox.setPosition(100,10);
        //mm_titleBox.setPosition(100,10);


        window.draw(mm_bigBox);
        //window.draw(mm_smallerBox);
        //window.draw(mm_titleBox);

        t_title.setPosition(window.getSize().x/2,200);
        window.draw(t_title);

        t_enteredtext.setPosition(window.getSize().x/2,350);
        window.draw(t_enteredtext);

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        auto lastView = window.getView();
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos,lastView);

        //t_pressToContinue.setPosition(window.getSize().x/2,300);
        //window.draw(t_pressToContinue);
        buttonList.Update(window, fps, &worldPos);
        window.setView(window.getDefaultView());

    } else {
        if (v4core->currentController.isInitialized){
            v4core->currentController.Update(window, fps);
        }
    }

}
void NewGameNameEntryMenu::UpdateButtons(){
    /// this should update the text on all the buttons
    buttonList.UpdateButtons();
}
void NewGameNameEntryMenu::OnExit(){
    /// when we exit the main menu, we do nothing for now.
    /// perhaps we would want to unload sprites or songs etc
}
NewGameNameEntryMenu::~NewGameNameEntryMenu()
{
    //dtor
}
