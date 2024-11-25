#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>

class Visualization {
public:
    Visualization();
    ~Visualization();
   
    void initializeDisplay();
    void drawStation();
    void drawTrain(int trackNumber, float position);
    void drawTrack(int trackNumber, bool occupied, bool lightStatus);
    void drawSwitch(int switchId, bool position);
    void updateDisplay();
    void clearScreen();

private:
    ALLEGRO_DISPLAY* display;
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int TRACK_SPACING = 60;
    const int TRACK_Y_OFFSET = 100;
};

#endif