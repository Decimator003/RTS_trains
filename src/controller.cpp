#include "../include/controller.h"
#include "../include/station.h"
#include <thread>
#include <chrono>
#include <iostream>

Controller::Controller(TrainStation& s, Scheduler& sch)
    : station(s), scheduler(sch) {
    current_time = "00:00";
}

void Controller::update() {
    vis.clearScreen();
   
    // Update time
    updateTime();
   
    // Draw station infrastructure
    vis.drawStation();
    vis.drawTime(current_time);
   
    // Draw all tracks
    for(int i = 0; i < 8; i++) {
        vis.drawTrack(i, station.isTrackOccupied(i), station.getTrackLightStatus(i));
    }
   
    // Draw switches
    for(int i = 0; i < 4; i++) {
        vis.drawSwitch(i, station.getSwitchPosition(i));
    }
   
    // Update scheduler with current time
    scheduler.setCurrentTime(current_time);
   
    // Get and process ready trains
    auto ready_trains = scheduler.getReadyTrains();
    for(Train* train : ready_trains) {
        // Handle train arrival
        if(!train->hasArrived()) {
            if(station.allocateTrack(train->getId())) {
                trainPositions[train->getId()] = 0.0f;
                train->setArrived(true);
                std::cout << "Train " << train->getId() << " arrived at "
                         << current_time << std::endl;
            }
        }
        // Handle train departure
        else if(train->hasArrived() && !train->hasDeparted() &&
                current_time >= train->getDepartureTime()) {
            int trackNum = station.getTrainTrack(train->getId());
            if(trackNum >= 0) {
                station.releaseTrack(trackNum);
                train->setDeparted(true);
                trainPositions.erase(train->getId());
                std::cout << "Train " << train->getId() << " departed at "
                         << current_time << std::endl;
            }
        }
    }
   
	// Update and draw trains
     for(auto& [trainId, position] : trainPositions) {
        // Adjust movement direction based on entry point
        int trackNum = station.getTrainTrack(trainId);
        if(trackNum >= 0) {
            if(trackNum < 4) { // Left tracks (WEST entry)
                position += 2.0f;
            } else { // Right tracks (EAST entry)
                position -= 2.0f;
            }
            vis.drawTrain(trackNum, position);
        }
    }

	 // Process next train
    Train* next_train = scheduler.getNextTrain();
    if(next_train) {
        if(station.allocateTrack(next_train->getId(), next_train->getEntryPoint())) {
            // Set initial position based on entry point
            float initial_position = (next_train->getEntryPoint() == "WEST") ? 0.0f : 800.0f; // Adjust 800.0f to match your screen width
            trainPositions[next_train->getId()] = initial_position;
        }
    }
   
    vis.updateDisplay();
}

void Controller::updateTime() {
    static int frame_count = 0;
    if(++frame_count >= 20) { // Advance time every 20 frames
        frame_count = 0;
       
        // Parse current time
        int hours, minutes;
        sscanf(current_time.c_str(), "%d:%d", &hours, &minutes);
       
        // Advance by one minute
        minutes++;
        if(minutes >= 60) {
            minutes = 0;
            hours++;
            if(hours >= 24) {
                hours = 0;
            }
        }
       
        // Format new time
        char time_str[6];
        snprintf(time_str, sizeof(time_str), "%02d:%02d", hours, minutes);
        current_time = time_str;
    }
}

void Controller::run() {
    bool simulation_complete = false;
   
    while(!simulation_complete) {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
       
        // Check if all trains have departed
        if(scheduler.isScheduleComplete()) {
            simulation_complete = true;
            std::cout << "Simulation completed at " << current_time << std::endl;
        }
       
        // Optional: Add a maximum time limit
        if(current_time >= "23:59") {
            simulation_complete = true;
            std::cout << "Simulation time limit reached" << std::endl;
        }
    }
}

void Controller::reset() {
    trainPositions.clear();
    current_time = "00:00";
    scheduler.reset();
    station.reset();
}
