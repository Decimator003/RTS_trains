#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "train.h"
#include <vector>
#include <string>

class Scheduler {
public:
    // Schedule loading and management
    bool loadSchedule(const std::string& filename);
    void updateSchedule();
    void reset();
    bool isScheduleComplete() const;

    // Train management
    Train* getNextTrain();
    std::vector<Train*> getReadyTrains();
    std::vector<Train*> getPendingTrains();

    // Time management
    void setCurrentTime(const std::string& time);
    std::string getCurrentTime() const;
    void advanceTime();

	Train* getTrainById(int id) {
        for(auto& train : trains) {
            if(train.getId() == id) {
                return &train;
            }
        }
        return nullptr;
    }

private:
    // Helper methods
    bool isTimeToProcess(const std::string& scheduled_time) const;

    // Member variables
    std::vector<Train> trains;
    size_t current_index = 0;
    std::string current_time = "00:00";  // Format: "HH:MM"
};

#endif
