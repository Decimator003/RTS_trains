#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>
#include <string>
#include <unordered_map>
#include "scheduler.h"
#include "station.h"

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
    void updateAndDrawTrains(const std::unordered_map<int, float>& trainPositions, 
                             const Scheduler& scheduler, 
                             const std::string& current_time, 
                             const TrainStation& station, // Updated to TrainStation
                             float speedMultiplier);
    void drawTime(const std::string& time);

private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_FONT* font;  // Add font member
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int TRACK_SPACING = 60;
    const int TRACK_Y_OFFSET = 100;
};

#endif

