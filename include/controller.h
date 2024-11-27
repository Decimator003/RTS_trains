#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "station.h"
#include "scheduler.h"
#include "visualization.h"
#include <map>
#include <string>

class Controller {
public:
    // Constructor
    Controller(TrainStation& station, Scheduler& scheduler);
   
    // Main control methods
    void update();
    void run();
    void reset();

private:
    // Helper method for time management
    void updateTime();

    // Core components
    TrainStation& station;
    Scheduler& scheduler;
    Visualization vis;
   
    // State tracking
    std::map<int, float> trainPositions;  // Maps train IDs to their positions
    std::string current_time;             // Current simulation time in "HH:MM" format
};

#endif
