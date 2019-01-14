#include <iostream>
#include <sstream>
#include "Rhythm.h"
#include "SongController.h"
#include <cmath>
using namespace std;

Rhythm::Rhythm()
{
    r_rhythm.setSize(sf::Vector2f(100,100));
    r_rhythm.setFillColor(sf::Color::White);

    r_fever.setSize(sf::Vector2f(100,10));
    r_fever.setFillColor(sf::Color::Red);

    r_fever_meter.setSize(sf::Vector2f(100,10));
    r_fever_meter.setFillColor(sf::Color::Yellow);

    b_fever_start.loadFromFile("resources/sfx/bgm/fever_start.ogg");
    b_fever_fail.loadFromFile("resources/sfx/bgm/fever_fail.ogg");
}

void Rhythm::LoadTheme(string theme)
{
    ///Load the BGM
    songController = new SongController();
    songController->LoadSongByName(theme);
    this->currentThemeName = theme;

    cycle = 0;
    cycle_mode = 0;

    bgm_cycle = 0;

    combo = 0;

    ///Restart the Rhythm clocks
    rhythmClock.restart();

    s_theme[0].setBuffer(songController->GetSongByNumber(0,0));
    s_theme[0].play();
}

void Rhythm::BreakCombo()
{
    cout << "Oops! You broke your combo!" << endl;

    rhythmClock.restart();

    s_chant.stop();
    if(combo >= 11) ///Play dying fever sound when BGM is in fever state
    {
        ///Dying fever sound
        s_fever_fail.setBuffer(b_fever_fail);
        s_fever_fail.play();
    }

    ///Reset combo to idle BGM point
    combo = 1;

    ///Reset Perfects table
    rhythmController.perfect = 0;
    rhythmController.perfects.clear();

    ///Reset command input
    rhythmController.commandInput.clear();

    ///Stop the theme
    s_theme[0].stop();
    s_theme[1].stop();

    ///Play the idle loop
    s_theme[0].setBuffer(songController->GetSongByNumber(0,1));
    s_theme[0].play();

    ///Stop any current action
    current_song = "";

    ///Beat cycles
    cycle = 4;
    cycle_mode = 1;

    bgm_cycle = 0;
}

void Rhythm::checkRhythmController(sf::RenderWindow& window)
{
    ///RHYTHM CONTROLLER SETUP
    rhythmController.combo = combo;

    rhythmController.masterTimer = abs(rhythmClock.getElapsedTime().asMilliseconds() - 250);
    rhythmController.av_commands = av_commands;

    rhythmController.low_range = low_range;
    rhythmController.high_range = high_range;

    if(rhythmController.checkForInput())
    {
        Drum temp;
        temp.Load(rhythmController.drumToLoad,rhythmController.drum_perfection,window);
        temp.pattern = rhythmController.currentPattern;
        drums.push_back(temp);

        if(rhythmController.breakCombo)
        {
            BreakCombo();
        }
    }

    rhythmController.resetValues();
}

///TO BE PORTED TO AN EXTERNAL CLASS
void Rhythm::doVisuals(sf::RenderWindow& window)
{
    auto lastView = window.getView();

    window.setView(window.getDefaultView());

    /**
    clock250 - full white
    clock400 - faded white
    clock
    */

    int rhythmAlpha = 250 - (rhythmClock.getElapsedTime().asMilliseconds() / 2);

    ///Visuals
    if(true)
    {
        ///Calculate the ratio for other window sizes (default is 1280x720)
        float ratio_X = window.getSize().x / float(1280);
        float ratio_Y = window.getSize().y / float(720);
        float ratio_universal = (window.getSize().x * window.getSize().y) / (float(1280) * float(720));

        /// Beat frame
        if((combo <= 1) or ((combo > 1) and (combo < 11) and (cycle_mode == 0)))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(3 * ratio_universal));
            r_rhythm.setOutlineColor(sf::Color(255,255,255,rhythmAlpha));
            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (24 * ratio_X), (720 * ratio_Y) - (24 * ratio_Y)));
            r_rhythm.setPosition(12*ratio_X,12*ratio_Y);

            r_rhythm2.setOutlineColor(sf::Color(0,0,0,0));
        }
        else if((combo > 1) and (cycle_mode == 1))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(2 * ratio_universal));
            r_rhythm.setOutlineColor(sf::Color(64,64,64,rhythmAlpha));
            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
            r_rhythm.setPosition(10*ratio_X,10*ratio_Y);

            r_rhythm2.setFillColor(sf::Color(0,0,0,0));
            r_rhythm2.setOutlineThickness(-ceil(2 * ratio_universal));
            r_rhythm2.setOutlineColor(sf::Color(64,64,64,rhythmAlpha));
            r_rhythm2.setSize(sf::Vector2f((1280 * ratio_X) - (30 * ratio_X), (720 * ratio_Y) - (30 * ratio_Y)));
            r_rhythm2.setPosition(15*ratio_X,15*ratio_Y);

            if(cycle == 4)
            {
                if(floor(flicker) == 0)
                {
                    r_rhythm.setOutlineColor(sf::Color(64,64,64,rhythmAlpha));
                    r_rhythm2.setOutlineColor(sf::Color(64,64,64,rhythmAlpha));
                }
                else if(floor(flicker) == 1)
                {
                    r_rhythm.setOutlineColor(sf::Color(220,220,220,rhythmAlpha));
                    r_rhythm2.setOutlineColor(sf::Color(220,220,220,rhythmAlpha));
                }

                flicker += float(1) / fps * 30;

                if(flicker >= 2)
                flicker = 0;
            }
        }
        else if((combo >= 11) and (cycle_mode == 0))
        {
            r_rhythm.setFillColor(sf::Color(0,0,0,0));
            r_rhythm.setOutlineThickness(-ceil(7 * ratio_universal));

            r_rhythm.setSize(sf::Vector2f((1280 * ratio_X) - (20 * ratio_X), (720 * ratio_Y) - (20 * ratio_Y)));
            r_rhythm.setPosition(10*ratio_X,10*ratio_Y);

            r_rhythm2.setOutlineColor(sf::Color(0,0,0,0));

            int flick = floor(flicker);

            switch(flick)
            {
                case 0:
                r_rhythm.setOutlineColor(sf::Color(255,255,0,rhythmAlpha));
                break;

                case 1:
                r_rhythm.setOutlineColor(sf::Color(255,255,255,rhythmAlpha));
                break;

                case 2:
                r_rhythm.setOutlineColor(sf::Color(0,255,255,rhythmAlpha));
                break;

                case 3:
                r_rhythm.setOutlineColor(sf::Color(0,255,0,rhythmAlpha));
                break;
            }

            flicker += float(1) / fps * 30;

            if(flicker >= 4)
            flicker = 0;
        }

        /// Fever meter - now with 16 segments!
        int feverMeterWidth = 10*(combo-12);
        if(feverMeterWidth > 160)
        {
            feverMeterWidth = 160;
        }

        int sizeMod = rhythmAlpha/float(80);

        r_fever.setSize(sf::Vector2f(160+sizeMod*2,10+sizeMod*2));
        r_fever_meter.setSize(sf::Vector2f(feverMeterWidth+sizeMod*2,10+sizeMod*2));

        r_fever.setPosition(50*ratio_X-sizeMod,50*ratio_Y-sizeMod);
        r_fever_meter.setPosition(50*ratio_X-sizeMod,50*ratio_Y-sizeMod);
    }

    window.draw(r_rhythm);
    window.draw(r_rhythm2);
    window.draw(r_fever);

    if(combo > 12)
    {
        window.draw(r_fever_meter);
    }

    for(int i=0; i<drums.size(); i++)
    {
        drums[i].fps = fps;
        drums[i].Draw(window);

        if(drums[i].alpha <= 0)
        {
            drums.erase(drums.begin() + i);
        }
    }

    window.setView(lastView);
}

void Rhythm::Draw(sf::RenderWindow& window)
{
    checkRhythmController(window);
    doVisuals(window);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        LoadTheme("Freakout Rock Theme");
    }

    if(rhythmClock.getElapsedTime().asMilliseconds() > 245)
    {
        if(!count_cycle)
        {
            cycle++;

            if(rhythmController.hit)
            {
                if(combo >= 2)
                {
                    if(cycle_mode == 1)
                    {
                        BreakCombo();
                    }
                }

            }
            else
            {
                if(combo >= 2)
                {
                    if(cycle_mode == 0)
                    {
                        BreakCombo();
                    }
                }
            }

            rhythmController.hit = false;
            count_cycle = true;
        }
    }

    if(rhythmClock.getElapsedTime().asMilliseconds() > 495)
    {
        count_cycle = false;
        bgm_cycle++;

        cout << "bgm_cycle: " << bgm_cycle << endl;

        rhythmClock.restart();

        if(combo <= 1) ///start anytime function
        {
            if(rhythmController.hit == false)
            {
                if(rhythmController.commandInput.size() == 4) ///If user input is 4 drums
                {
                    string fullcom = rhythmController.commandInput[0]+rhythmController.commandInput[1]+rhythmController.commandInput[2]+rhythmController.commandInput[3]; ///Create a full command using 4 individual hits

                    if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end()) ///Check if the command exists in available commands
                    {
                        ///Clear user input
                        rhythmController.commandInput.clear();

                        ///Push the amount of perfect hits to the table and reset them
                        rhythmController.perfects.push_back(rhythmController.perfect);
                        rhythmController.perfect = 0;

                        cout << "command found!" << endl;

                        combo += 1;

                        if(combo >= 28)
                        combo = 12;

                        s_theme[combo%2].setBuffer(songController->GetSongByNumber(0,combo));

                        s_theme[combo%2].stop();
                        s_theme[combo%2].play();

                        cycle_mode = 1;
                        cycle = 0;

                        bgm_cycle = 0;
                    }
                }
            }
        }

        if(cycle >= 4)
        {
            cycle = 0;

            cycle_mode += 1;
            cycle_mode = cycle_mode%2;
        }

        if(bgm_cycle >= 8)
        {
            cout << cycle_mode << " " << combo << endl;

            bgm_cycle = 0;

            if(combo == 0)
            combo = 1;

            if(combo == 1)
            {
                cout << "this?" << endl;

                s_theme[0].setBuffer(songController->GetSongByNumber(0,combo));

                s_theme[0].stop();
                s_theme[1].stop();

                s_theme[0].play();
            }

            if(combo >= 2) /// If combo is not idle bgm
            {
                if(cycle_mode == 1)
                {
                    if(rhythmController.hit == false)
                    {
                        if(rhythmController.commandInput.size() == 4) ///If user input is 4 drums
                        {
                            string fullcom = rhythmController.commandInput[0]+rhythmController.commandInput[1]+rhythmController.commandInput[2]+rhythmController.commandInput[3]; ///Create a full command using 4 individual hits

                            if(std::find(av_commands.begin(), av_commands.end(), fullcom) != av_commands.end()) ///Check if the command exists in available commands
                            {
                                ///Clear user input
                                rhythmController.commandInput.clear();

                                ///Push the amount of perfect hits to the table and reset them
                                rhythmController.perfects.push_back(rhythmController.perfect);
                                rhythmController.perfect = 0;

                                cout << "command found!" << endl;

                                combo += 1;

                                if(combo >= 28)
                                combo = 12;

                                s_theme[combo%2].setBuffer(songController->GetSongByNumber(0,combo));

                                s_theme[combo%2].stop();
                                s_theme[combo%2].play();

                                cycle_mode = 1;
                                cycle = 0;
                            }
                            else
                            {
                                BreakCombo();
                            }
                        }
                    }
                }
            }
        }
    }
}
