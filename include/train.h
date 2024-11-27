#ifndef TRAIN_H
#define TRAIN_H

#include <string>

class Train {
public:
    Train(int id, int priority, std::string entry, std::string exit,
          std::string arrival, std::string departure);
   
    int getId() const { return train_id; }
    int getPriority() const { return priority; }
    std::string getEntryPoint() const { return entry_point; }
    std::string getExitPoint() const { return exit_point; }

	std::string getArrivalTime() const { return arrival_time; }
    std::string getDepartureTime() const { return departure_time; }
    bool hasArrived() const { return arrived; }
    bool hasDeparted() const { return departed; }

	void setArrived(bool status) { arrived = status; }
    void setDeparted(bool status) { departed = status; }

private:
    int train_id;
    int priority;
    std::string entry_point;
    std::string exit_point;

	std::string arrival_time;   // Add this
    std::string departure_time; // Add this
    bool arrived = false;       // Add this
    bool departed = false;      // Add this
};

#endif
