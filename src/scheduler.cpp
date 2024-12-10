#include "../include/scheduler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

// Helper function to convert time string to minutes since midnight
int convertTimeToMinutes(const std::string& time) {
    std::istringstream ss(time);
    int hours, minutes;
    char delimiter;
    ss >> hours >> delimiter >> minutes;
    return hours * 60 + minutes;
}

// Helper function to compare times
bool Scheduler::isTimeToProcess(const std::string& scheduled_time) const {
    return convertTimeToMinutes(current_time) >= convertTimeToMinutes(scheduled_time);
}
static bool compareByDeadline(Train* a, Train* b) {
        return a->getDepartureTime() < b->getDepartureTime();
    }

bool Scheduler::loadSchedule(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Failed to open schedule file: " << filename << "\n";
        return false;
    }

    std::string line;
    // Skip header
    std::getline(file, line);
   
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;
       
        while(std::getline(ss, item, ',')) {
            row.push_back(item);
        }
       
        if(row.size() >= 6) {
            trains.emplace_back(
                std::stoi(row[0]),  // id
                std::stoi(row[1]),  // priority
                row[4],            // entry_point
                row[5],            // exit_point
                row[2],            // arrival_time
                row[3]             // departure_time
            );
        }
    }
        // Sort trains by new arrival times and priority
    std::sort(trains.begin(), trains.end(),
        [](const Train& a, const Train& b) {
            if (a.getArrivalTime() == b.getArrivalTime()) {
                return a.getPriority() < b.getPriority();
            }
            return a.getArrivalTime() < b.getArrivalTime();
        });

    
   

    return true;

}

void Scheduler::setCurrentTime(const std::string& time) {
    current_time = time;
}

std::string Scheduler::getCurrentTime() const {
    return current_time;
}


std::vector<Train*> Scheduler::getReadyTrains() {
    std::vector<Train*> ready_trains;
    
    // Get all trains that have arrived but not departed
    for(auto& train : trains) {
        if(train.getArrivalTime() <= current_time && !train.hasDeparted()) {
            ready_trains.push_back(&train);
        }
    }
    
    // Sort trains by earliest deadline (departure time) - EDF scheduling
    std::sort(ready_trains.begin(), ready_trains.end(), compareByDeadline);
    
    return ready_trains;
}


bool Scheduler::hasConflict(const std::string& start_time, const std::string& end_time, const Train& train) {
    int start_minutes = convertTimeToMinutes(start_time);
    int end_minutes = convertTimeToMinutes(end_time);
    int train_start = convertTimeToMinutes(train.getArrivalTime());
    int train_end = convertTimeToMinutes(train.getDepartureTime());
   
    return !(end_minutes <= train_start || start_minutes >= train_end);
}

bool Scheduler::isTimeSlotAvailable(const std::string& start_time, const std::string& end_time) {
    for (const auto& train : trains) {
        if (hasConflict(start_time, end_time, train)) {
            return false;
        }
    }
    return true;
}



std::string Scheduler::findNextAvailableSlot(const std::string& after_time) {
    int current_minutes = convertTimeToMinutes(after_time);

    while (current_minutes < 24 * 60) {  // Within 24 hours
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << (current_minutes / 60) << ":"
           << std::setfill('0') << std::setw(2) << (current_minutes % 60);

        if (isTimeSlotAvailable(ss.str(),
            // Add 15 minutes for standard slot size
            std::to_string((current_minutes + 15) / 60) + ":" +
            std::to_string((current_minutes + 15) % 60))) {
            return ss.str();
        }

        current_minutes += 5;  // Try next 5-minute slot
    }
    return "";  // No available slot found
}

















/*std::vector<Train*> Scheduler::getReadyTrains() {
    std::vector<Train*> ready_trains;
   
    for (auto& train : trains) {
        // Skip trains that have already departed
        if (train.hasDeparted()) {
            continue;
        }
       
        // Check if it's time for the train to arrive
        if (!train.hasArrived() && isTimeToProcess(train.getArrivalTime())) {
            ready_trains.push_back(&train);
        }
        // Check if it's time for the train to depart
        else if (train.hasArrived() && !train.hasDeparted() &&
                 isTimeToProcess(train.getDepartureTime())) {
            ready_trains.push_back(&train);
        }
    }
   
    return ready_trains;
}
*/












Train* Scheduler::getNextTrain() {
    while (current_index < trains.size()) {
        Train& train = trains[current_index];
       
        // Skip trains that have already departed
        if (train.hasDeparted()) {
            current_index++;
            continue;
        }
       
        // Check if it's time for this train
        if (!train.hasArrived() && isTimeToProcess(train.getArrivalTime())) {
            return &trains[current_index++];
        }
       
        break; // Wait for the next time update if we haven't found a ready train
    }
   
    return nullptr;
}

void Scheduler::updateSchedule() {
    // Update train statuses based on current time
    for (auto& train : trains) {
        if (!train.hasArrived() && isTimeToProcess(train.getArrivalTime())) {
            train.setArrived(true);
        }
       
        if (train.hasArrived() && !train.hasDeparted() &&
            isTimeToProcess(train.getDepartureTime())) {
            train.setDeparted(true);
        }
    }
}

// Helper function to advance time by one minute
void Scheduler::advanceTime() {
    int minutes = convertTimeToMinutes(current_time);
    minutes++;
   
    int hours = minutes / 60;
    int mins = minutes % 60;
   
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << mins;
   
    current_time = ss.str();
}

// Get all trains that haven't departed yet
std::vector<Train*> Scheduler::getPendingTrains() {
    std::vector<Train*> pending_trains;
    for (auto& train : trains) {
        if (!train.hasDeparted()) {
            pending_trains.push_back(&train);
        }
    }
    return pending_trains;
}

// Check if all trains have departed
bool Scheduler::isScheduleComplete() const {
    return std::all_of(trains.begin(), trains.end(),
        [](const Train& train) { return train.hasDeparted(); });
}

// Reset the scheduler to initial state
void Scheduler::reset() {
    current_index = 0;
    current_time = "00:00";
    for (auto& train : trains) {
        train.setArrived(false);
        train.setDeparted(false);
    }
}
