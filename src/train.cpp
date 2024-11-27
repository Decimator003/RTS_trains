#include "../include/train.h"

Train::Train(int id, int priority, std::string entry, std::string exit,
             std::string arrival, std::string departure)
    : train_id(id), priority(priority), entry_point(entry), exit_point(exit),
      arrival_time(arrival), departure_time(departure) {}
