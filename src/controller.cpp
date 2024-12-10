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
    updateTime();
    vis.drawStation();
    vis.drawTime(current_time);

    // Draw infrastructure
    for(int i = 0; i < 8; i++) {
        vis.drawTrack(i, station.isTrackOccupied(i), station.getTrackLightStatus(i));
    }
    for(int i = 0; i < 4; i++) {
        vis.drawSwitch(i, station.getSwitchPosition(i));
    }

    scheduler.setCurrentTime(current_time);

    // Get and process ready trains
    auto ready_trains = scheduler.getReadyTrains();
    for(Train* train : ready_trains) {
        if(!train->hasArrived()) {
            if(station.allocateTrack(train->getId(), train->getEntryPoint())) {
                // Set initial position based on entry point
                if(train->getEntryPoint() == "EAST") {
                    trainPositions[train->getId()] = 800.0f;
                } else {
                    trainPositions[train->getId()] = 0.0f;
                }
                train->setArrived(true);
                std::cout << "Train " << train->getId() << " arrived at "
                         << current_time << std::endl;
            }
        }
    }

    // Update train positions based on time
    for(auto& [trainId, position] : trainPositions) {
        int trackNum = station.getTrainTrack(trainId);
        if(trackNum >= 0) {
            Train* train = scheduler.getTrainById(trainId);
            if(train) {
                // Calculate progress based on arrival and departure times
                int arrivalMinutes = convertTimeToMinutes(train->getArrivalTime());
                int departureMinutes = convertTimeToMinutes(train->getDepartureTime());
                int currentMinutes = convertTimeToMinutes(current_time);
               
                // Calculate time-based progress (0.0 to 1.0)
                float timeProgress = 0.0f;
                if(currentMinutes >= arrivalMinutes && currentMinutes <= departureMinutes) {
                    timeProgress = static_cast<float>(currentMinutes - arrivalMinutes) /
                                 static_cast<float>(departureMinutes - arrivalMinutes);
                }

                // Update position based on direction and progress
                if(train->getEntryPoint() == "WEST") {
                    position = timeProgress * 800.0f; // Move from left to right
                } else {
                    position = 800.0f - (timeProgress * 800.0f); // Move from right to left
                }
               
                vis.drawTrain(trackNum, position);
            }
        }
    }

    // Handle departures
    for(Train* train : ready_trains) {
        if(train->hasArrived() && !train->hasDeparted() &&
           current_time >= train->getDepartureTime()) {
            int trackNum = station.getTrainTrack(train->getId());
            if(trackNum >= 0) {
                station.releaseTrack(trackNum);
                train->setDeparted(true);
                trainPositions.erase(train->getId());
                std::cout << "Train " << train->getId() << " departed at "
                         << current_time << std::endl;
                if(current_time>train->getDepartureTime())std::cout<<"deadline missed train set not schedulable";
            }
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
int Controller::convertTimeToMinutes(const std::string& time) {
    int hours, minutes;
    sscanf(time.c_str(), "%d:%d", &hours, &minutes);
    return hours * 60 + minutes;
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
